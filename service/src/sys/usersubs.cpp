#include <exodus/library.h>
libraryinit()

#include <authorised.h>
#include <sysmsg.h>
#include <usersubs.h>
#include <securitysubs.h>
#include <sortarray.h>
#include <singular.h>

#include <gen_common.h>
#include <win_common.h>

#include <window.hpp>

var msg;
var xx;
var usern;
var newuser;//num
var text;
var op;
var op2;
var wspos;
var wsmsg;

function main(in mode) {
	//c sys

	#include <general_common.h>
	//$insert abp,common

	var users;
	if (not(users.open("USERS", ""))) {
		msg = "USERS file is missing";
		return invalid(msg);
		}

	if (mode == "POSTREAD") {

		//no longer needed and prevents access to obsolete users
		//ie ones left in the users file after being deleted from auth table
		//gosub getusern
		//if valid else return

		//get email from userlist
		//it is in the user file already
		//if @id ne 'NEOSYS' then @record<7>=userprivs<7,usern>

		if (win.wlocked and ID ne USERNAME) {

			//only neosys to access neosys
			if (ID == "NEOSYS") {
				msg = "Not authorised";
				return invalid(msg);
			}

			//maybe can only access self - now commented out in order to:
			//allow user update to people like timesheet administrator
			//without access granting authorisation access
			//and rely on USER UPDATE task
			//if security('AUTHORISATION ACCESS',msg) else goto invalid

			//maybe can only update self
			//if security('AUTHORISATION UPDATE',xx) else
			if (not(authorised("USER UPDATE", xx))) {
				win.srcfile.unlock( ID);
				win.wlocked = 0;
			}

		}

		//prevent new users with punctuation characters etc
		if ((win.orec == "") and win.wlocked) {
			var temp = ID;
			temp.converter(UPPERCASE ^ "0123456789", "");
			if (temp) {
				msg = ID.quote() ^ " user doesnt exist and new usercodes";
				msg.r(-1, "must be alphanumeric characters only");
				return invalid(msg);
			}
		}

		//password autoexpiry inform UI directly to allow warning
		//without an additional request to the server to get the expiry days
		var expirydays = SECURITY.a(25);
		if (expirydays) {
			//password date default to lastlogin date
			//if no login date then consider the account to have expired
			//and they would not have been able to login with it.
			if (not(RECORD.a(36))) {
				var lastlogindate = RECORD.a(13).field(".", 1);
				RECORD.r(36, lastlogindate);
			}
			var expirydate = RECORD.a(36) + expirydays;
			RECORD.r(37, expirydate);
		}else{
			//indicate no expiry
			RECORD.r(37, "");
		}

	} else if (mode == "VAL.EMAIL") {

		//assume email addresses in IS

		//adminstrator email addresses
		//tempting to also allow any domains held by admin emails but
		//those could have special priviliges to blackberry etc
		var sysemails = " " ^ SYSTEM.a(76);
		sysemails ^= " " ^ SYSTEM.a(84);
		sysemails ^= " " ^ SYSTEM.a(101);
		sysemails ^= " " ^ SYSTEM.a(102);
		sysemails.converter(";,", "  ");
		sysemails.trimmer();

		//check all email addresses versus domain and email restrictions
		//email domains is a space separated list of domains and/or email addresses
		var emaildomains = SYSTEM.a(116).lcase();
		if (emaildomains) {
			//admin *emails* (NOT DOMAINS) are specifically allowed
			emaildomains ^= " " ^ sysemails;
		}else{
			//if no specific domain restrictions then only admin DOMAINS are allowed
			if (sysemails) {

				//strip out the domain names
				var nn = sysemails.count(" ") + 1;
				for (var ii = 1; ii <= nn; ++ii) {
					var word = sysemails.field(" ", ii);
					word = field2(word, "@", -1);
					//remove smtp. mailout. etc from smtp host domain
					if (var("smtp,mail,mailout").locateusing(",",word.field(".", 1),xx)) {
						word = word.field(".", 2, 999);
					}
					sysemails = sysemails.fieldstore(" ", ii, 1, word);
				};//ii;
				emaildomains ^= " " ^ sysemails;
			}
		}

		//always allow anything @neosys.com
		emaildomains ^= " neosys.com";

		if (emaildomains) {
			//check emails
			//emails=lcase(@record<7>)
			var emails = win.is.lcase();
			emails.converter(FM ^ VM ^ SVM ^ ", ", ";;;;;");
			var nn = emails.count(";") + 1;
			for (var ii = 1; ii <= nn; ++ii) {
				var email = emails.field(";", ii);
				if (email) {
					var emaildomain = email.field("@", 2);
					if (not(emaildomains.locateusing(" ",emaildomain,xx))) {
						if (not(emaildomains.locateusing(" ",email,xx))) {
							msg = "Neither " ^ (emaildomain.quote()) ^ " nor " ^ (email.quote()) ^ "|is in the list of allowed email domains/addresses";
							return invalid(msg);
						}
					}
				}
			};//ii;
		}

	//can be PREWRITE.RESETPASSWORD
	} else if (mode.field(".", 1) == "PREWRITE") {

		var resetpassword = mode.field(".", 2) == "RESETPASSWORD";

		if (SECURITY.read(DEFINITIONS, "SECURITY")) {
			SECURITY = SECURITY.invert();
		}

		if (SECURITY.a(1).locate(ID,usern)) {
			newuser = 0;
			if (RECORD.a(4) and RECORD.a(4) ne win.orec.a(4)) {
				resetpassword = 1;
			}
		}else{
			newuser = 1;
			resetpassword = 0;
		}

		if (not(lockrecord("DEFINITIONS", DEFINITIONS, "SECURITY", xx, 0))) {
			if (resetpassword) {
				resetpassword = 2;
			}else{
				msg = "Somebody is updating the Authorisation File now. Please retry later";
				return invalid(msg);
			}
		}

		//get the latest userprivs
		if (not(SECURITY.read(DEFINITIONS, "SECURITY"))) {
			msg = "SECURITY is missing from DEFINITIONS";
			gosub unlocksec();
			call sysmsg(msg);
			return invalid(msg);
		}
		SECURITY = SECURITY.invert();

		var olduserprivs = SECURITY;

		//neosys usually not in the users list or shouldnt update security record
		if (ID == "NEOSYS") {
			return 0;
		}

		//prevent amendment of expiry date and password date in UI if not authorised
		//also name, email and department
		if (mode == "PREWRITE") {
			if (win.orec) {
				if (not(authorised("AUTHORISATION UPDATE", xx))) {
					//expiry date
					if (win.orec.a(35)) {
						RECORD.r(35, win.orec.a(35));
						}
					//password date
					if (win.orec.a(36)) {
						RECORD.r(36, win.orec.a(36));
					}
					//tt=@record<19>
					//swap 'Default' with '' in tt
					//username
					RECORD.r(1, win.orec.a(1));
					//department
					RECORD.r(5, win.orec.a(5));
					//email
					RECORD.r(7, win.orec.a(7));
					//department2
					RECORD.r(21, win.orec.a(21));
				}

			}
		}

		//verify email domains
		win.is = RECORD.a(7);
		call usersubs("VAL.EMAIL");
		if (not(win.valid)) {
			gosub unlocksec();
			return 0;
		}

		//create new user in userprivs
		if (newuser) {

			var userdept = RECORD.a(21);
			if (not userdept) {
				msg = "USER GROUP CODE is required to create new users";
				gosub invalid(msg);
				gosub unlocksec();
				return 0;
			}

			if (not(SECURITY.a(1).locate(userdept,usern))) {
				msg = (userdept ^ " USER GROUP does not exist").quote();
				gosub invalid(msg);
				gosub unlocksec();
				return 0;
			}

			//insert an empty user
			for (var fn = 1; fn <= 9; ++fn) {
				SECURITY.inserter(fn, usern, "");
			};//fn;

			var newusername = RECORD.a(1);
			var newipnos = RECORD.a(40);
			var newemailaddress = RECORD.a(7);

			SECURITY.r(1, usern, ID);
			//userprivs<2,usern>=newkeys
			//userprivs<3,usern>=newexpirydate
			//userprivs<4,usern>=newpass
			//userprivs<5,usern>=newhourlyrate
			SECURITY.r(6, usern, newipnos);
			SECURITY.r(7, usern, newemailaddress);
			SECURITY.r(8, usern, newusername);

		}

		gosub getusern();
		if (not(win.valid)) {
			gosub unlocksec();
			return 0;
		}

		if (resetpassword < 2) {

			//email address
			SECURITY.r(7, usern, RECORD.a(7));

			//user name
			SECURITY.r(8, usern, RECORD.a(1));

		}

		//reencrypt new password
		var ans = RECORD.a(4);
		if ((newuser or resetpassword) and ans) {

			win.is = ID ^ FM ^ ans.a(1);
			call securitysubs("MAKESYSREC");
			ans = win.is;

			//save the encrypted bit in case we cannot update userprivs
			RECORD.r(4, ans.a(7));

			//update security if managed to lock it
			if (resetpassword < 2) {
				ans.converter(FM, TM);
				var tt = "<hidden>" ^ SVM ^ ans;
				SECURITY.r(4, usern, tt);
			}

		}

		if ((resetpassword < 2) and SECURITY ne olduserprivs) {
			SECURITY.invert().write(DEFINITIONS, "SECURITY");
			//no need on user if on userprivs
			RECORD.r(4, "");
		}

		//new password cause entry in users log to renable login if blocked
		//similar in security.subs and user.subs
		if (resetpassword or newuser) {
			//datetime=(date():'.':time() 'R(0)#5')+0
			var datetime = var().date() ^ "." ^ var().time().oconv("R(0)#5");
			RECORD.inserter(15, 1, datetime);
			RECORD.inserter(16, 1, SYSTEM.a(40, 2));

			if (USERNAME == APPLICATION) {
				text = "OK New password sent to " ^ RECORD.a(7);
			}else{
				text = "OK New password set by " ^ USERNAME;
			}
			RECORD.inserter(18, 1, text);

			RECORD.r(36, datetime);
		}

		//sort holidays in reverse order
		call sortarray(RECORD, 22 ^ VM ^ 23, "DR");

	} else if (mode == "POSTWRITE") {

		gosub updatemirror();

		call unlockrecord("DEFINITIONS", DEFINITIONS, "SECURITY");

		return 0;

	} else if (mode == "CREATEUSERNAMEINDEX") {
		win.srcfile = users;
		select(win.srcfile);

		while (true) {
			if (not(readnext(ID))) {
				ID = "*!%";
			}
			///BREAK;
			if (not(ID ne "*!%")) break;
			if (not(ID[1] == "%")) {
				if (RECORD.read(users, ID)) {
					gosub updatemirror();
				}
			}
		}//loop;

	} else {
		msg = mode.quote() ^ " is invalid in USER.SUBS";
		return invalid(msg);

	}

	return 0;
}

subroutine unlocksec() {
	call unlockrecord("DEFINITIONS", DEFINITIONS, "SECURITY");
	return;
}

subroutine getusern() {
	//fail safe only allowed to update existing users
	if (ID == "NEOSYS") {
		//usern remains unassigned to force an error if used later on
	}else{
		if (not(SECURITY.a(1).locate(ID,usern))) {
			msg = ID.quote() ^ " User does not exist";
			gosub invalid(msg);
			return;
		}
	}
	return;

	//similar code in user.subs and security.subs
}

subroutine updatemirror() {
	//save the user keyed as username too
	//because we save the user name and executive code in many places
	//and we still need to get the executive email if they are a user
	var mirror = RECORD.fieldstore(FM, 13, 5, "");
	mirror = RECORD.fieldstore(FM, 31, 3, "");
	var username = RECORD.a(1).ucase();
	var mirrorkey = "%" ^ username ^ "%";
	mirror.r(1, ID);
	mirror.write(win.srcfile, mirrorkey);
	return;
}

libraryexit()
