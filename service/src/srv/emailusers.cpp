#include <exodus/library.h>
libraryinit()

#include <emailusers.h>
#include <sendmail.h>
#include <sysmsg.h>

#include <service_common.h>

#include <srv_common.h>

var subject;
var body;
var nsent;	// num
var replyto;
var toemails;
var ccemails;
var usercode;
var userx;
var expirydate;
var emails;
var ok;	 // num
//var xx;
//var nn;
var errormsg;

function main(in /*mode0*/, in subject0, in body0, in groupids0, in /*jobids0*/, in userids0, in options, io emaillog) {

	let interactive = false;  //not(SYSTEM.f(33));

	// options
	// R = REPLYTO=@username email address if exists
	// W = Groups by Word eg user with dept MEDIA BUYING matches group MEDIA

	//let modeq = mode0.assigned() ? mode0 : "";

	if (SENTENCE.field(" ", 1) == "EMAILUSERS") {

		let modex	= SENTENCE.field(" ", 2);
		let version = SENTENCE.field(" ", 3, 9999);
		SENTENCE	= "";

		if (modex != "UPGRADE") {
			let msg = modex.quote() ^ " is invalid in EMAILUSERS";
			if (interactive) {
				call mssg(msg);
			} else {
				call sysmsg(msg);
			}
			stop();
		}

		subject = "EXODUS Upgrade: " ^ SYSTEM.f(23);
		if (SYSTEM.f(17) != SYSTEM.f(23)) {
			subject ^= " (" ^ SYSTEM.f(17) ^ ")";
		}

		body = "";
		body ^= "The EXODUS system software has been upgraded.";
		// body:=vm
		// body:=vm:'In case of errors, please follow the instructions at'
		// body:=vm:'http://userwiki.neosys.com/index.php/cache.'
		body ^= VM;
		body ^= VM ^ "Please email SUPPORT@EXODUS.COM for any assistance.";
		// body:=vm
		// body:=vm:'This is an automated email. You cannot reply to it.'

		//body.converter(VM, chr(13));
		// chr(13) no longer compatible with Postfix 3.6.4-1ubuntu1.2
		// sendmail.cpp converts FM to newline
		body.converter(VM, FM);

		// call emailusers(mode,subject,body,'','','','R',emaillog)
		// no more replyto support@neosys.com because no longer needed
		// because installations sender emails like clientxyz@neosys.com
		// are configured as aliases of support@neosys.com
		// and replyto option suppresses the email header
		// urn:schemas:mailheader:X-Auto-Response-Suppress = RN, NRN, OOF
		// thereby allowing Out of Office replies which we do not want
		call emailusers(modex, subject, body, "", "", "", "", emaillog);

		if (not emaillog) {
			emaillog = "(nobody)";
		}
		emaillog = "Upgrade Notification emailed to:" ^ VM ^ emaillog;

		//emaillog.replacer(VM, chr(13));
		// chr(13) no longer compatible with Postfix 3.6.4-1ubuntu1.2
		// sendmail.cpp converts FM to newline
		emaillog.replacer(VM, FM);

		call sysmsg(emaillog, "Upgrade to version " ^ version);

		stop();
	}

// init:
// ///
	// if target and options='' or index(options,'U',1) then
	// end
	bool groupword = options.contains("W");
	nsent		   = 0;

	subject = subject0;
	body	= body0;

	// read fromuser from users,@username else fromuser=''
	replyto = "";
	if (options.contains("R")) {
		if ((USERNAME == "EXODUS" or USERNAME == "ADAGENCY") or USERNAME == "ACCOUNTS") {
			replyto = "support@neosys.com";
		} else {
			let fromuser = xlate("USERS", USERNAME, "", "X");
			replyto		 = fromuser.f(7);
			var fromline = "From " ^ fromuser.f(1);
			if (USERNAME != fromuser.f(1)) {
				fromline ^= " (" ^ USERNAME ^ ")";
			}
			subject.prefixer(fromline ^ " : ");
		}
	}

	let usercodes	   = SECURITY.f(1);
	let nusers		   = usercodes.fcount(VM);
	emaillog		   = "";
	var alreadyemailed = "";

	//body.converter(FM ^ VM, chr(13) ^ chr(13));
	// chr(13) no longer compatible with Postfix 3.6.4-1ubuntu1.2
	// sendmail.cpp converts FM to newline
	body.converter(VM, FM);

	var groupids = groupids0;
	groupids.converter(",", VM);
	let ngroups = groupids.fcount(VM);

	var userids = userids0;
	userids.converter(",", VM);

	toemails	 = "";
	ccemails	 = "";
	var currdept = "";

	var users;
	if (not users.open("USERS", "")) {
		call mssg(lasterror());
		return 0;
	}

// ///////
//nextuser:
// ///////

	for (let usern : range(1, nusers)) {

		// skip empty users
		usercode = usercodes.f(1, usern);
		if (usercode == "") {
			continue;  // usern
		}

		// only users on file
		if (not userx.read(users, usercode)) {
			continue;  // usern;
		}

		// not expired users
		expirydate = userx.f(35);
		if (expirydate and expirydate <= date()) {
			continue;  // usern;
		}

		// skip users with no email at all
		// users may have 0 or more email addresses eg xyz@abc.com;123@345.com etc
		emails = userx.f(7);
		if (emails == "") {
			continue;  // usern;
		}

		// always email to self last
		if (usercode == USERNAME and replyto) {
			continue;  // usern;
		}

		ok = 0;
		if (not(ok) and userids) {
			if (userids.locate(usercode)) {
				ok = 1;
			}
			if (not(ok) and not(groupids)) {
				continue;  // usern;
			}
		}

		// skip users not of required type (eg FINANCE is ok in FINANCE CONTROLLER)
		// could determine user type from what menus they can access eg MEDIA
		if (not(ok) and groupids) {
			if (groupword) {
				// == search for MEDIA in user department like MEDIA BUYER
				for (const var groupn : range(1, ngroups)) {
					ok = userx.f(5).contains(groupids.f(1, groupn));
					// /BREAK;
					if (not not(ok))
						break;
				}  // groupn;
			} else {
				// exact groups
				if (groupids.locate(userx.f(21))) {
					ok = 1;
				}
			}
			if (not ok) {
				continue;  // usern;
			}
		}

		// must be last to avoid adding emails to sent list unless actually sent
		// remove any emails that have already been emailed before
		emails.converter(" ", "");
		emails.converter(";", VM);
		let nn = emails.fcount(VM);
		//for (var ii = nn; ii >= 1; --ii) {
		for (let ii : reverse_range(1, nn)) {
			let email = emails.f(1, ii);
			if (alreadyemailed.locate(email)) {
				emails.remover(1, ii);
			} else {
				alreadyemailed ^= VM ^ email;
			}
		}  // ii;
		emails.converter(VM, ";");

		// skip users that have already been emailed before
		if (emails == "") {
			continue;  // usern;
		}

	// userinit:
	// ///////
		printx(usercode, " ");

		if (currdept and userx.f(5) != currdept) {
			gosub sendemails(emaillog);
		}
		currdept = userx.f(5);

		if (toemails == "") {
			toemails = emails;

			// sending to users (or groups and users)
		} else if (userids) {
			toemails ^= ";" ^ emails;

			// sending to groups then to the first and cc the rest
		} else {
			ccemails ^= ";" ^ emails;
		}

		emaillog ^= VM ^ usercode ^ " " ^ emails;

	}  // usern;

//exit:
// ///

	gosub sendemails(emaillog);

	emaillog.trimmerlast("." _VM _FM);

	// always email to self
	if (nsent and replyto) {
		toemails = replyto;
		body ^= VM ^ VM ^ "-- Sent to --" ^ emaillog;

		//body.replacer(VM, chr(13));
		// chr(13) no longer compatible with Postfix 3.6.4-1ubuntu1.2
		// sendmail.cpp converts FM to newline
		body.replacer(VM, FM);

		gosub sendemails(emaillog);
	}

	// no longer suppress sendmail errors
	// since this hides problems with email server/configuration
	// if nsent else
	// emaillog=''
	// end

	emaillog.trimmerlast("." _VM _FM);

	return 0;
}

subroutine sendemails(io emaillog) {

	if (not toemails) {
		return;
	}

	if (ccemails.starts(";")) {
		ccemails.cutter(1);
	}

	call sendmail(toemails, ccemails, subject, body, "", "", errormsg, replyto);

	if (errormsg and errormsg != "OK") {
		emaillog ^= VM ^ errormsg;
	} else {
		nsent += 1;
	}

	emaillog ^= VM;

	toemails = "";
	ccemails = "";

	return;
}

libraryexit()
