#include <exodus/library.h>
libraryinit()

#include <addcent4.h>
#include <authorised.h>
#include <changelogsubs.h>
#include <emailusers.h>
#include <generalsubs.h>
#include <gethtml.h>
#include <getsubs.h>
#include <otherusers.h>
#include <proxysubs.h>
#include <safeselect.h>
#include <securitysubs.h>
#include <sendmail.h>
#include <singular.h>
#include <sysmsg.h>
#include <uploadsubs.h>
#include <usersubs.h>

#include <service_common.h>

#include <srv_common.h>
#include <req_common.h>

#include <request.hpp>

var stationery;
var mode;
var html;
var dbn;
var emailresult;
var usersordefinitions;
var userkey;
var userx;
var newpassword;
var xx;
var fn;	 // num
var task;
var taskprefix;
var reportid;
var logyear;
var logfromdate;
var loguptodate;
var logkey;
var logsearch;

function main() {
	// !subroutine general(request,data,response)

	// use app specific version of generalsubs
	if (APPLICATION ne "EXODUS") {
		generalsubs = "generalsubs_app";
	}

	req.valid  = 1;
	msg_	   = "";
	stationery = "";

	call cropper(request_);
	call cropper(data_);
	mode = request_.f(1).ucase();

	// request 2 - can be anything actually

	req.datafile = request_.f(2);
	var keyx	 = request_.f(3);

	response_ = "OK";

	if (mode eq "TEST") {

	} else if (mode eq "TRANTEST") {

		/* TRANTEST - check speed and accuracy of "concurrent" updates by mass updating a single record.

		Simply add 1 to a field 1 of record key "1", 100 times in file exodus_trantest

		The Total: *MUST* go up by the expected number even if multiple browsers are running concurrently

		e.g. TWO browsers (not two tabs of one browser) running the test CONCURRENTLY using 10 repeats
		*MUST* increase the Total: by 2 x 10 x 100 = 2000

		The speed seems to go down a lot after thousands of update, perhaps due to postgresql needing
		a VACUUM to remove the historical versions of the updates records since it never actually
		overwrites the original data blocks.
		*/

		var transactions = "exodus_trantest";
		if (not open(transactions, transactions))
			//createfile(transactions);
			if (not createfile(transactions))
				abort(lasterror());

		// ERROR:  VACUUM cannot run inside a transaction block sqlstate:25001
		// if (not transactions.sqlexec("vacuum " ^ transactions.f(1))) {
		// 	response_ = var().lasterror();
		// 	stop();
		// }

		ID		   = "1";
		let ntrans = 100;
		// printl();
		// printl("TRANTEST 1.", THREADNO);
		for (const var recn : range(1, ntrans)) {
			// if (TERMINAL and recn % 100)
			// 	printl(recn);
			if (not read(RECORD, transactions, ID))
				RECORD = "";
			write(RECORD + 1, transactions, ID);
		}
		// printl();
		// printl("TRANTEST 2.", THREADNO);
		response_ = "OK Thread No. " ^ THREADNO ^ ", Total: " ^ (RECORD + 1);

	} else if (mode.f(1) eq "PREVIEWLETTERHEAD") {

		// comma sep
		var compcodes = request_.f(2);
		var testing	  = request_.f(3);

		var allhtml = "";

		// if testing, style borders of td and divs for visual insight
		if (testing) {
			allhtml(-1) = "<style>";
			allhtml(-1) = "td {border:dotted 1px #EEEEEE;}";
			allhtml(-1) = "div {border:dashed 1px lightgrey;}";
			allhtml(-1) = "</style>";
		}

		let ncomps = compcodes.fcount(",");
		for (const var compn : range(1, ncomps)) {

			mode		  = "HEAD";
			var	 compcode = compcodes.field(",", compn);
			call gethtml(mode, html, compcode);

			allhtml(-1) = "<br />Company " ^ compcode ^ " from " ^ mode;
			allhtml(-1) = "<br />";

			// if not testing then wrap html in hr for clarity
			// if testing else allhtml<-1>='<hr/>'
			allhtml(-1) = html;
			// if testing else allhtml<-1>='<hr/>'

		}  // compn;
		allhtml(-1) = "Press F5 to refresh any images just uploaded.";
		allhtml.replacer(_FM, _EOL);
		//var(allhtml).oswrite(SYSTEM.f(2));
		if (not var(allhtml).oswrite(SYSTEM.f(2)))
			abort(lasterror());
		gosub postproc();

	} else if (mode eq "PERIODTABLE") {

		var year	= request_.f(2).field("-", 1).field("/", 2);
		var finyear = request_.f(3);

		perform("PERIODTABLE " ^ year ^ " " ^ finyear ^ " (H)");

		gosub postproc();

	} else if (mode eq "FILEMAN" and request_.f(2) eq "COPYDB") {

		var copydb = request_.f(3);
		if (not(SYSTEM.f(58).locate(copydb, dbn))) {
			// Not found. dbn points to last + 1
		}
		var todb = SYSTEM.f(63, dbn);
		if (not todb) {
			abort("\"Copy to\" database must be configured (and saved) for database " ^ copydb ^ " first");
		}

		// ensure authorised to login to one or the other database
		// by ensuring the user is currently logged in to one or other database
		if ((USERNAME ne "EXODUS" and copydb ne SYSTEM.f(17)) and todb ne SYSTEM.f(17)) {
			msg_	 = "In order to copy database " ^ (copydb.quote()) ^ " to " ^ (todb.quote()) ^ ",";
			msg_(-1) = "you must be logged in to database " ^ (copydb.quote()) ^ " or " ^ (todb.quote());
			msg_(-1) = "but you are currently logged in to database " ^ (SYSTEM.f(17).quote());
			abort(msg_);
		}

		// should really have an option to close the live dataset and then copy
		if (not(authorised("DATASET COPY", msg_, "LS"))) {
			abort(msg_);
		}

		var started		= time().oconv("MTS");
		var otherusersx = otherusers(copydb);
		var log			= started ^ " Started copy database " ^ copydb ^ " to " ^ todb;
		log ^= "|" ^ started ^ " Other processes online:" ^ otherusersx.f(1);

		perform("COPYDB " ^ copydb ^ " " ^ todb);

		response_ = msg_;
		if (not response_) {
			log ^= "|" ^ time().oconv("MTS") ^ " Finished";
			call sysmsg(log);
			response_ = "OK " ^ log;
		}

	} else if (mode eq "EMAILUSERS") {

		var groupids	   = data_.f(1);
		var jobfunctionids = "";
		var userids		   = data_.f(2);
		if (not((groupids or jobfunctionids) or userids)) {
			abort("You must specify some groups or users to email");
		}

		// ensure sender has an email address
		// not absolutely necessary but provides a return email address
		if (USERNAME ne "EXODUS" and not(USERNAME.xlate("USERS", 7, "X"))) {
			abort("You cannot send email because you do not have an email address for replies");
		}

		// T=
		// R=reply to is current user
		// W=group is word to be found in user department
		var options = "TR";

		var subject = data_.f(4);
		var message = data_.f(5);

		message.converter(_TM, _VM);
		call emailusers("", subject, message, groupids, jobfunctionids, userids, options, emailresult);

		if (emailresult) {
			emailresult.converter(_VM _FM, "\r\r");
			response_ =
				"OK Email Sent to"
				"\r" ^
				emailresult;
		} else {
			call mssg("No users can be found to email,|or some problem with email server");
		}
		data_ = "";

	} else if (mode eq "CREATEDATABASE") {
		// For patsalides

		if (not(authorised("DATASET CREATE", msg_))) {
			abort(msg_);
		}

		var targetdbname = data_.f(1);
		var targetdbcode = data_.f(2).lcase();
		var sourcedbcode = data_.f(3).lcase();

		var sourcedatadir = "../data/" ^ sourcedbcode;
		var targetdatadir = "../data/" ^ targetdbcode;

		var dbcodes = dblist();

		// source database services will be stopped for source to target database copy
		// reject source db if same as current database.
		var currentdbcode = SYSTEM(17);
		if (sourcedbcode eq currentdbcode) {
			return invalid("The old database and the database you are curently logged into cannot be the same.\nLogin to a different database and try again.");
		}

		// check source database exists
		if (not locateusing(_FM, sourcedbcode, dbcodes)) {
			return invalid("Source database doesn't exists.");
		}

		// check new database and data dir dont already exist
		if (locateusing(_FM, targetdbcode, dbcodes)) {
			return invalid("Sorry, " ^ targetdbcode.quote() ^ " already exists");
		}

		// stop source live service
		if (not osshell("systemctl stop agy_live@" ^ sourcedbcode)) {
			return invalid("Cannot stop " ^ sourcedbcode ^ "'s service");
		}

		// Copy source db to target db
		// ERROR:  CREATE DATABASE cannot run inside a transaction block
		// if (not dbcopy(sourcedbcode,targetdbcode)) {
		//osshell("dbcopy " ^ sourcedbcode ^ " " ^ targetdbcode);
		// check target db created
		//var newdbcodes = dblist();
		//if (not locateusing(_FM, targetdbcode, newdbcodes)) {
		//	return invalid(targetdbname ^ targetdbcode.quote() ^ " not created");
		//}

		if (not osshell("dbcopy " ^ sourcedbcode ^ " " ^ targetdbcode)) {
			return invalid("Failed to create new database: " ^ targetdbcode.quote() ^ " - " ^ targetdbname);
		}

		// start source live service
		//osshell("systemctl start agy_live@" ^ sourcedbcode);
		if (not osshell("systemctl start agy_live@" ^ sourcedbcode))
			return invalid("Failed to start source database " ^ sourcedbcode.quote() ^ " services");

		// create target data dir
		// oscopy(sourcedatadir, targetdatadir);
		// KEEP IN SYNC. SIMILAR code in create_site, create_service and generalproxy.cpp
		//osshell("mkdir -p " ^ targetdatadir);
		//osshell("chmod a+srw " ^ targetdatadir);
		//osshell("setfacl -d -m g::rw " ^ targetdatadir);
		if (not osshell("mkdir -p " ^ targetdatadir))
			return invalid(lasterror());

		if (not osshell("chmod a+srw " ^ targetdatadir))
			return invalid(lasterror());

		if (not osshell("setfacl -d -m g::rw " ^ targetdatadir))
			return invalid(lasterror());

		if (not osdir(targetdatadir)) {
			return invalid("Error in creating target data directory");
		}

		// skip - target image dir on first upload

		// update database name file
		if (not oswrite(targetdbname, "../data/" ^ targetdbcode ^ "/name")) {
			return invalid("Cannot update database file name");
		}

		// email confirmaion
		call sysmsg("", targetdbname ^ " - (" ^ targetdbcode ^ ") created");

		response_ = "OK Database " ^ targetdbname ^ " " ^ targetdbcode.quote() ^ " has been created.";

	} else if (mode eq "VAL.EMAIL") {
		req.is = request_;
		call usersubs(mode);

		// case mode[-3,3]='SSH'
		// call ssh(mode)

	} else if (mode eq "PASSWORDRESET") {

		if (not(authorised("PASSWORD RESET", msg_))) {
			abort(msg_);
		}

		var users;
		if (not(users.open("USERS", ""))) {
			abort("USERS file is missing");
		}

		ID				 = request_.f(2);
		var emailaddress = request_.f(3);

		var baduseroremail = "Either " ^ (ID.quote()) ^ " or " ^ (emailaddress.quote()) ^ " does not exist";

		var userrec;
		if (userrec.read(users, ID)) {
			usersordefinitions = users;
			userkey			   = ID;

			if (emailaddress.ucase() eq userx.f(7).ucase()) {

				// signify ok
				baduseroremail = "";

				call securitysubs("GENERATEPASSWORD");
				newpassword = ANS;
				userrec(4)	= newpassword;
			}

		} else {
			usersordefinitions = DEFINITIONS;
			userkey			   = "BADUSER*" ^ ID;
			if (not(userrec.read(usersordefinitions, userkey))) {
				userrec = "";
			}
		}

		// record historical resets/attempts
		// datetime=(date():'.':time() 'R(0)#5')+0
		var datetime = date() ^ "." ^ time().oconv("R(0)#5");
		userrec.inserter(15, 1, datetime);
		userrec.inserter(16, 1, SYSTEM.f(40, 2));
		userrec.inserter(18, 1, ("Password Reset " ^ baduseroremail).trim());

		if (baduseroremail) {
			userrec.write(usersordefinitions, userkey);
			abort(baduseroremail);
		}

		// prewrite (locks authorisation file or fails)
		req.valid = 1;
		call usersubs("PREWRITE");
		if (not(req.valid)) {
			abort();
		}

		userrec.write(usersordefinitions, userkey);

		// postwrite
		call usersubs("POSTWRITE");

		// cc the user too if the username is valid
		// call sysmsg('User: ':username:fm:'From IP: ':system<40,2>:fm:text,'Password Reset',userkey)

		// send the new password to the user
		var emailaddrs = userrec.f(7);
		var ccaddrs	   = "";
		var subject	   = "EXODUS Password Reset";
		var body	   = "User: " ^ ID;
		body(-1)	   = "Your new password is " ^ newpassword;
		call sendmail(emailaddrs, ccaddrs, subject, body, "", "", xx);

	} else if (mode eq "MAKEUPLOADPATH") {
		call uploadsubs("MAKEUPLOADPATH." ^ request_.f(2));

	} else if (mode eq "POSTUPLOAD") {
		call uploadsubs("POSTUPLOAD");

	} else if (mode eq "VERIFYUPLOAD") {
		call uploadsubs("VERIFYUPLOAD." ^ request_.f(2));

	} else if (mode eq "OPENUPLOAD") {
		call uploadsubs("OPENUPLOAD." ^ request_.f(2));

	} else if (mode eq "DELETEUPLOAD") {
		call uploadsubs("DELETEUPLOAD." ^ request_.f(2));

	} else if (mode eq "GETCODEPAGE") {
		data_ = "";
		// have to skip char zero it seems to be treated as string terminator
		// somewhere on the way to the browser (not in revelation)
		for (const var ii : range(1, 255)) {
			data_ ^= chr(ii);
		}  // ii;
		// data='xxx'
		response_ = "OK";

	} else if (mode eq "SETCODEPAGE") {
		if (not(srv.alanguage.open("ALANGUAGE", ""))) {
			abort(lasterror());
		}

		var codepage = request_.f(3);
		if (not codepage) {
			goto badsetcodepage;
		}

		if (request_.f(2) eq "SORTORDER") {

			/* should be inverted but bother since cant get DOS only collated ascii;
				if len(data) ne 254 then;
					call msg('SORTORDER data is ':len(data):' but must be 254 characters long|(initial char 0 excluded)');
					stop;
					end;
				write char(0):data on alanguage,'SORTORDER*':codepage;
			*/

		} else if (request_.f(2) eq "UPPERCASE") {
			fn = 9;
setcodepagecase:
			var recordx;
			if (not(recordx.read(srv.alanguage, "GENERAL*" ^ codepage))) {
				if (not(recordx.read(srv.alanguage, "GENERAL"))) {
					recordx = "";
				}
			}
			var temp = data_;
			temp.replacer(_RM, "%FF");
			temp.replacer(_FM, "%FE");
			temp.replacer(_VM, "%FD");
			recordx(1, fn) = temp;

			recordx.write(srv.alanguage, "GENERAL*" ^ codepage);

			/*;
				// check lower=lcase(upper) and vice versa
				// but ucase() may strip accents whereas lcase() may leave them
				x=@lower.case;
				y=@upper.case;
				convert @lower.case to @upper.case in x;
				convert @upper.case to @lower.case in y;
				convert @lower.case to @upper.case in y;
				convert @upper.case to @lower.case in x;
				for ii=1 to len(x);
					// if x[ii,1] ne @lower.case[ii,1] then print 'x: ':ii:' ':seq(x[ii,1]) 'MX':' ':seq(@lower.case[ii,1]) 'MX'
					if y[ii,1] ne @upper.case[ii,1] then print 'y: ':ii:' ':seq(y[ii,1]) 'MX':' ':seq(@upper.case[ii,1]) 'MX';
					next ii;
				*/

		} else if (request_.f(2) eq "LOWERCASE") {
			fn = 10;
			goto setcodepagecase;

		} else {
badsetcodepage:
			call mssg("Request must be SETCODEPAGE SORTORDER|UPPERCASE|LOWERCASE codepage");
			return 0;
		}

		response_ = "OK";

	} else if (mode eq "GETDATASETS") {
		ANS = "";
		call generalsubs("GETDATASETS");
		data_ = ANS;
		if (data_) {
			response_ = "OK";
		} else {
			response_ = "Error: No datasets found";
		}

	} else if (mode eq "LISTPROCESSES") {

		perform("SORT PROCESSES");

		gosub postproc();

	} else if (mode eq "LISTREQUESTLOG") {

		PSEUDO = data_;
		perform("LISTREQUESTLOG");

		// printopts='L'
		gosub postproc();

	} else if (mode eq "LISTLOCKS") {

		perform("SORT LOCKS WITH NO LOCK_EXPIRED");

		gosub postproc();

	} else if (mode eq "GETVERSIONDATES") {

		call changelogsubs("GETVERSIONDATES");

	} else if (mode.ucase() eq "WHATSNEW") {

		call changelogsubs("SELECTANDLIST" ^ FM ^ data_);
		if (msg_) {
			msg_.move(response_);
			stop();
		}

		// printopts='L'
		gosub postproc();

	} else if (mode eq "LISTADDRESSES") {

		perform("LISTADDRESSES");

		// printopts='L'
		gosub postproc();

	} else if (mode eq "GETDEPTS") {
		call usersubs("GETDEPTS");
		data_	  = ANS;
		response_ = "OK";

	} else if (mode eq "LISTACTIVITIES") {

		perform("LISTACTIVITIES " ^ request_.f(2));

		gosub postproc();

	} else if (mode eq "ABOUT") {
		perform("ABOUT");
		// transfer @user4 to data
		// response='OK'
		msg_.move(response_);
		response_.prefixer("OK ");

	} else if (mode eq "UTIL") {
		perform("UTIL");
		response_ = "OK";

	} else if (mode eq "PROG") {
		perform("PROG");
		response_ = "OK";

		// LISTAUTH.TASKS = list of tasks LISTTASKS
		// LISTAUTH.USERS = list of users LISTUSERS
	} else if (mode.field(".", 1) eq "LISTAUTH") {

		req.wlocked	  = 0;
		req.templatex = "SECURITY";
		call securitysubs("SETUP");
		if (not(req.valid)) {
			abort();
		}
		// call security.subs('LISTAUTH')
		call securitysubs(mode);
		if (not(req.valid)) {
			abort();
		}
		call securitysubs("POSTAPP");

		gosub postproc();

	} else if (mode eq "READUSERS") {

		req.templatex = "SECURITY";
		call securitysubs("SETUP");
		if (not(req.valid)) {
			abort();
		}

		data_	  = RECORD;
		response_ = "OK";

	} else if (mode eq "GETREPORTS") {

		task = request_.f(2);
		gosub gettaskprefix();
		if (taskprefix) {
			if (not(authorised(taskprefix ^ " ACCESS", msg_, ""))) {
				abort(msg_);
			}
		}

		var select = "SELECT DOCUMENTS BY-DSND EXODUS_STANDARD BY DESCRIPTION";

		var instructions = request_.f(2);
		instructions.replacer(_VM, "%FD");
		select ^= " WITH INSTRUCTIONS2 " ^ (instructions.quote());

		if (not(authorised("DOCUMENTS: ACCESS OTHER PEOPLES DOCUMENTS", xx, ""))) {
			select ^= " AND WITH CREATEDBY " ^ (USERNAME.quote());
		}

		call safeselect(select ^ " (S)");

		gosub opendocuments();

		data_	 = "";
		var repn = 0;
nextrep:
		if (readnext(reportid)) {
			var report;
			if (report.read(srv.documents, reportid)) {
				repn += 1;

				var temp = report.f(2);
				temp.replacer("&", "&amp;");
				temp.replacer("<", "&lt;");
				temp.replacer(">", "&gt;");
				report(2) = temp;

				// !dont send instructions since takes up space and not needed
				// DO send now to have info in requestlog
				// report<5>=''

				report.converter(_VM, _RM);
				let nn = report.fcount(_FM);
				for (const var ii : range(1, nn)) {
					data_(ii, repn) = report.f(ii);
				}  // ii;

				data_(9, repn) = reportid;

				// print repn,data<1>
			}
			// if len(data)<65000 then goto nextrep
			if (data_.len() lt maxstrsize_ - 530) {
				goto nextrep;
			}
		}
		response_ = "OK";

	} else if (mode eq "DELETEREPORT") {

		gosub opendocuments();

		var docnos = request_.f(2);

		let ndocs = docnos.fcount(_VM);
		for (const var docn : range(1, ndocs)) {
			ID = docnos.f(1, docn);
			if (ID) {
				if (RECORD.read(srv.documents, ID)) {
					req.orec = RECORD;
					call getsubs("PREDELETE");
					if (not(req.valid)) {
						goto exit;
					}

					if (req.valid) {
						srv.documents.deleterecord(ID);

						call getsubs("POSTDELETE");
					}
				}
			}
		}  // docn;

	} else if (mode eq "UPDATEREPORT") {

		gosub opendocuments();

		var doc;
		if (not(doc.read(srv.documents, request_.f(2)))) {
			abort("Document " ^ (request_.f(2).quote()) ^ " is missing");
		}

		// TODO security

		doc(6) = lower(data_);

		doc.write(srv.documents, request_.f(2));

	} else if (mode eq "COPYREPORT") {

		gosub opendocuments();

		var doc;
		if (not(doc.read(srv.documents, request_.f(2)))) {
			abort("Document " ^ (request_.f(2).quote()) ^ " is missing");
		}

		task = doc.f(5);
		gosub gettaskprefix();
		if (taskprefix) {
			if (not(authorised(taskprefix ^ " CREATE", msg_, ""))) {
				abort(msg_);
			}
		}

		call getsubs("DEF.DOCUMENT.NO");
		if (not(req.valid)) {
			abort();
		}

		var description = doc.f(2);
		doc(2)			= description ^ " (Copy)";

		// prevent copy from appearing like a exodus standard
		doc(10) = "";

		doc(1) = USERNAME;
		doc.write(srv.documents, ID);

		data_ = ID ^ FM ^ doc;

		response_ = "OK";

	} else if (mode eq "GETREPORT") {

		// printopts='L'
		gosub opendocuments();

		// get parameters from documents into @pseudo

		if (not(srv.document.read(srv.documents, request_.f(2)))) {
			msg_ = "Document " ^ (request_.f(2).quote()) ^ " does not exist";
			abort(msg_);
		}

		task = srv.document.f(5);
		gosub gettaskprefix();
		if (taskprefix) {
			if (not(authorised(taskprefix ^ " ACCESS", msg_, ""))) {
				abort(msg_);
			}
		}

		// if task='BALANCES' then printopts='P'
		// if index(task,'MEDIADIARY',1) then printopts='X'

		PSEUDO = srv.document.f(6);
		PSEUDO.converter(_RM, _VM);
		PSEUDO = raise(PSEUDO);

		// merge any runtime parameters into the real parameters
		for (fn = 1; fn <= 999; ++fn) {
			var tt = data_.f(fn);
			if (tt) {
				PSEUDO(fn) = tt;
			}
		}  // fn;
		data_ = "";

		// save runtime params in case saving below for scheduled reruns
		// document<11>=lower(data)

		var sentencex = srv.document.f(5);
		sentencex.converter(_VM, " ");
		data_ = PSEUDO;

		// in case we are calling another proxy
		if (srv.document.f(5, 1).ends("PROXY")) {

			// run but suppress email
			// perform 'TEST ':request<2>:' (S)'

			request_ = raise(srv.document.f(5)).field(_FM, 2, 999999) ^ FM ^ request_.f(2);
			// moved up so parameters show in any emailed error messages
			// data=@pseudo
			// override the saved period with a current period
			var runtimeperiod = date().oconv("D2/E").b(4, 5);
			if (runtimeperiod.starts("0")) {
				runtimeperiod.cutter(1);
			}
			data_.replacer("{RUNTIME_PERIOD}", runtimeperiod);
			goto performreport;

		} else {

performreport:
			response_ = "";

			perform(sentencex);
			if (not response_) {

				gosub postproc();
			}
		}

	} else if (mode eq "USAGESTATISTICS") {

		PSEUDO = data_;
		perform("LISTSTATS");

		// printopts='L'
		gosub postproc();

	} else if (mode eq "VIEWLOG") {

		gosub initlog();
		var	  datedict = "LOG_DATE";

		var cmd = "LIST LOG" ^ logyear;
		cmd ^= " BY LOG_DATE BY LOG_TIME";
		cmd ^= " LOG_DATE LOG_TIME LOG_USERNAME LOG_WORKSTATION LOG_SOURCE LOG_MESSAGE2";
		if (logfromdate) {
			if (loguptodate) {
				cmd ^= "  xAND WITH LOG_DATE BETWEEN " ^ (logfromdate.oconv("D4").quote()) ^ " AND " ^ (loguptodate.oconv("D4").quote());
			} else {
				cmd ^= "  xAND WITH " ^ datedict ^ " >= " ^ (logfromdate.oconv("D4").quote());
			}
		}
		if (logkey) {
			cmd ^= " xAND WITH LOG_SOURCE1 STARTING " ^ (logkey.quote());
		}
		if (logsearch) {
			cmd ^= " xAND WITH ALLUPPERCASE CONTAINING " ^ (logsearch.quote());
		}
		cmd ^= " ID-SUPP";
		var temp = cmd.index("xAND");
		if (temp) {
			cmd.paster(temp, 5, "");
		}
		cmd.replacer("xAND", "AND");
		perform(cmd);

		// printopts='L'
		gosub postproc();

	} else if (mode eq "LISTMARKETS") {

		var cmd = "SORT MARKETS WITH AUTHORISED BY SEQ";
		cmd ^= " HEADING " ^ (var("List of Markets     'T'     Page 'PL'").quote());
		perform(cmd);

		gosub postproc();

	} else if (mode eq "LISTCURRENCIES") {

		perform("LISTCURRENCIES " ^ request_.f(2));

		gosub postproc();

	} else if (mode eq "LISTCOMPANIES") {

		perform("LISTCOMPANIES");

		// printopts='L'
		gosub postproc();

	} else if (mode.field(".", 1) eq "GETTASKS") {

		call securitysubs("GETTASKS." ^ request_.f(2) ^ "." ^ request_.f(3));
		data_ = ANS;

	} else {
		call mssg("System Error: " ^ (mode.quote()) ^ " invalid request in GENERALPROXY");
	}

// ///
exit:
	// ///
	return "";
}

subroutine postproc() {
	call proxysubs("GENERAL", mode, stationery);
	return;
}

subroutine initlog() {

	logkey		   = request_.f(2);
	logyear		   = request_.f(3);
	var logformat  = request_.f(4);
	var logoptions = request_.f(5);

	if (logoptions.match("^\\d*/\\d{2}$")) {
		logyear	   = addcent4(logoptions.field("/", 2));
		logoptions = "";
	}

	logfromdate = data_.f(5);
	loguptodate = data_.f(6);
	logsearch	= data_.f(7);

	if (logoptions eq "TODAY") {
		logfromdate = date();
		loguptodate = logfromdate;
	}

	if (not logyear) {
		var tt	= logfromdate;
		var tt2 = loguptodate;
		if (not tt) {
			tt = date();
		}
		if (not tt2) {
			tt2 = tt;
		}
		logyear		  = tt.oconv("DY");
		var logtoyear = tt2.oconv("DY");
		if (logyear ne logtoyear) {
			response_ = "Dates must be within one calendar year";
			abort();
		}
	}

	if (not(authorised("LOG ACCESS", msg_, ""))) {
		response_ = msg_;
		abort();
	}

	if (not(authorised("LOG ACCESS " ^ (logkey.quote()), msg_, ""))) {
		response_ = msg_;
		abort();
	}

	return;

	// in get.subs and generalproxy
}

subroutine gettaskprefix() {
	taskprefix = "";
	task	   = task.field(" ", 1);
	if (task eq "ANAL") {
		taskprefix = "BILLING REPORT";
	} else if (task eq "BALANCES") {
		taskprefix = "FINANCIAL REPORT";
	} else if (task eq "ANALSCH") {
		taskprefix = "BILLING REPORT";
	} else {
		taskprefix = "";
	}
	return;
}

subroutine opendocuments() {
	if (not(srv.documents.open("DOCUMENTS", ""))) {
		abort(lasterror());
	}
	return;
}

libraryexit()