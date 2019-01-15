#include <exodus/library.h>
libraryinit()

#include <sysmsg.h>
#include <emailusers.h>
#include <sendmail.h>

#include <gen.h>

var subject;
var body;
var nsent;//num
var replyto;
var toemails;
var ccemails;
var usercode;
var userx;
var expirydate;
var emails;
var ok;//num
var xx;
var nn;
var errormsg;

function main(in mode0, in subject0, in body0, in groupids0, in jobids0, in userids0, in options, io emaillog) {
	//c sys ,,,,,,,io

	//global nn,ok,emails,expirydate,userx,usercode,toemails,ccemails
	//global subject,body,replyto,nsent

	var interactive = not SYSTEM.a(33);

	//options
	//R = REPLYTO=@username email address if exists
	//W = Groups by Word eg user with dept MEDIA BUYING matches group MEDIA

	var mode = mode0;
	//evade c++ warning variable not used
	if (jobids0.unassigned()) {
		{}
	}

	if (SENTENCE.field(" ", 1) == "EMAILUSERS") {

		mode = SENTENCE.field(" ", 2);
		var version = SENTENCE.field(" ", 3, 9999);
		SENTENCE = "";

		if (mode ne "UPGRADE") {
			var msg = DQ ^ (mode ^ DQ) ^ " is invalid in EMAILUSERS";
			if (interactive) {
				call mssg(msg);
			}else{
				call sysmsg(msg);
			}
			var().stop();
		}

		subject = "NEOSYS Upgrade: " ^ SYSTEM.a(23);
		if (SYSTEM.a(17) ne SYSTEM.a(23)) {
			subject ^= " (" ^ SYSTEM.a(17) ^ ")";
		}

		body = "";
		body ^= "The NEOSYS system software has been upgraded.";
		//body:=vm
		//body:=vm:'In case of errors, please follow the instructions at'
		//body:=vm:'http://userwiki.neosys.com/index.php/cache.'
		body ^= VM;
		body ^= VM ^ "Please email SUPPORT@NEOSYS.COM for any assistance.";
		//body:=vm
		//body:=vm:'This is an automated email. You cannot reply to it.'
		body.converter(VM, var().chr(13));

		//call emailusers(mode,subject,body,'','','','R',emaillog)
		//no more replyto support@neosys.com because no longer needed
		//because installations sender emails like clientxyz@neosys.com
		//are configured as aliases of support@neosys.com
		//and replyto option suppresses the email header
		//urn:schemas:mailheader:X-Auto-Response-Suppress = RN, NRN, OOF
		//thereby allowing Out of Office replies which we do not want
		call emailusers(mode, subject, body, "", "", "", "", emaillog);

		if (not emaillog) {
			emaillog = "(nobody)";
		}
		emaillog = "Upgrade Notification emailed to:" ^ VM ^ emaillog;
		emaillog.swapper(VM, var().chr(13));
		call sysmsg(emaillog, "Upgrade to version " ^ version);

		var().stop();
	}

	//init:
	/////
	//if target and options='' or index(options,'U',1) then
	// end
	var groupword = options.index("W", 1);
	nsent = 0;

	subject = subject0;
	body = body0;

	//read fromuser from users,@username else fromuser=''
	replyto = "";
	if (options.index("R", 1)) {
		if (((USERNAME == "NEOSYS") or (USERNAME == "ADAGENCY")) or (USERNAME == "ACCOUNTS")) {
			replyto = "support@neosys.com";
		}else{
			var fromuser = USERNAME.xlate("USERS", "", "X");
			replyto = fromuser.a(7);
			var fromline = "From " ^ fromuser.a(1);
			if (USERNAME ne fromuser.a(1)) {
				fromline ^= " (" ^ USERNAME ^ ")";
			}
			subject.splicer(1, 0, fromline ^ " : ");
		}
	}

	var usercodes = SECURITY.a(1);
	var nusers = usercodes.count(VM) + 1;
	var usern = 0;
	emaillog = "";
	var alreadyemailed = "";
	body.converter(FM ^ VM, var().chr(13) ^ var().chr(13));

	var groupids = groupids0;
	groupids.converter(",", VM);
	var ngroups = groupids.count(VM) + 1;

	var userids = userids0;
	userids.converter(",", VM);

	toemails = "";
	ccemails = "";
	var currdept = "";

	var users;
	if (not(users.open("USERS", ""))) {
		call fsmsg();
		return 0;
	}

/////////
nextuser:
/////////
	if (esctoexit()) {
		goto exit;
	}

	usern += 1;
	if (usern > nusers) {
		goto exit;
	}

	//skip empty users
	usercode = usercodes.a(1, usern);
	if (usercode == "") {
		goto nextuser;
	}

	//only users on file
	if (not(userx.read(users, usercode))) {
		goto nextuser;
	}

	//not expired users
	expirydate = userx.a(35);
	if (expirydate and (expirydate <= var().date())) {
		goto nextuser;
	}

	//skip users with no email at all
	//users may have 0 or more email addresses eg xyz@abc.com;123@345.com etc
	emails = userx.a(7);
	if (emails == "") {
		goto nextuser;
	}

	//always email to self last
	if ((usercode == USERNAME) and replyto) {
		goto nextuser;
	}

	ok = 0;
	if (not ok and userids) {
		if (userids.locateusing(usercode, VM, xx)) {
			ok = 1;
		}
		if (not ok and not groupids) {
			goto nextuser;
		}
	}

	//skip users not of required type (eg FINANCE is ok in FINANCE CONTROLLER)
	//could determine user type from what menus they can access eg MEDIA
	if (not ok and groupids) {
		if (groupword) {
			//eq search for MEDIA in user department like MEDIA BUYER
			for (var groupn = 1; groupn <= ngroups; ++groupn) {
				ok = userx.a(5).index(groupids.a(1, groupn), 1);
			///BREAK;
			if (not(not ok)) break;;
			};//groupn;
		}else{
			//exact groups
			if (groupids.locateusing(userx.a(21), VM, xx)) {
				ok = 1;
			}
		}
		if (not ok) {
			goto nextuser;
		}
	}

	//must be last to avoid adding emails to sent list unless actually sent
	//remove any emails that have already been emailed before
	emails.converter(" ", "");
	emails.converter(";", VM);
	nn = emails.count(VM) + (emails ne "");
	for (var ii = nn; ii >= 1; --ii) {
		var email = emails.a(1, ii);
		if (alreadyemailed.locateusing(email, VM, xx)) {
			emails.eraser(1, ii);
		}else{
			alreadyemailed ^= VM ^ email;
		}
	};//ii;
	emails.converter(VM, ";");

	//skip users that have already been emailed before
	if (emails == "") {
		goto nextuser;
	}

	//userinit:
	/////////
	print(usercode, " ");

	if (currdept and userx.a(5) ne currdept) {
		gosub sendemails( emaillog);
	}
	currdept = userx.a(5);

	if (toemails == "") {
		toemails = emails;

	//sending to users (or groups and users)
	} else if (userids) {
		toemails ^= ";" ^ emails;

	//sending to groups then to the first and cc the rest
	} else {
		ccemails ^= ";" ^ emails;
	}
//L1129:
	emaillog ^= VM ^ usercode ^ " " ^ emails;

	//userexit:
	/////////
	goto nextuser;

exit:
/////
	gosub sendemails( emaillog);

	while (true) {
	///BREAK;
	if (not(var("." ^ VM ^ FM).index(emaillog[-1], 1))) break;;
		emaillog.splicer(-1, 1, "");
	}//loop;

	//always email to self
	if (nsent and replyto) {
		toemails = replyto;
		body ^= VM ^ VM ^ "-- Sent to --" ^ emaillog;
		body.swapper(VM, var().chr(13));
		gosub sendemails( emaillog);
	}

	//no longer suppress sendmail errors
	//since this hides problems with email server/configuration
	//if nsent else
	// emaillog=''
	// end

	while (true) {
	///BREAK;
	if (not(var("." ^ VM ^ FM).index(emaillog[-1], 1))) break;;
		emaillog.splicer(-1, 1, "");
	}//loop;

	return 0;

}

subroutine sendemails(io emaillog) {
	//sendemails(io emaillog)
	if (not toemails) {
		return;
	}

	if (ccemails[1] == ";") {
		ccemails.splicer(1, 1, "");
	}

	call sendmail(toemails, ccemails, subject, body, "", "", errormsg, replyto);

	if (errormsg and errormsg ne "OK") {
		emaillog ^= VM ^ errormsg;
	}else{
		nsent += 1;
	}

	emaillog ^= VM;

	toemails = "";
	ccemails = "";

	return;

}


libraryexit()
