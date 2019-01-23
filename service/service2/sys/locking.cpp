#include <exodus/library.h>
libraryinit()


#include <gen.h>

var ntries;//num
var lockdesc;
var xx;
var allowduplicate;//num
var lockitem;
var lockn;//num
var lockfile;

function main(in mode, in lockfilename, in lockkey, in lockdesc0, io locklist, in ntries0, out msg) {
	//c sys ,,,,io,in,out

	//global lockfile,lockitem

	//declare function delay
	if (locklist.unassigned()) {
		locklist = "";
	}
	if (ntries0.unassigned()) {
		ntries = 100;
	}else{
		ntries = ntries0;
	}
	if (lockdesc0.unassigned()) {
		lockdesc = "";
	}else{
		lockdesc = lockdesc0;
		}
	if (not ntries) {
		ntries = 100;
	}
	msg = "";

	var interactive = not SYSTEM.a(33);

	if (var(0) and (USERNAME == "NEOSYS")) {
		printl(mode, " ", lockfilename, " ", lockkey, " ", locklist);
		xx.input();
	}

	if (mode == "LOCK") {
		allowduplicate = 0;
		goto lockit;
		{}
	} else if (mode == "RELOCK") {
		allowduplicate = 1;
		goto lockit;
		{}
	} else if (mode == "UNLOCK") {
		gosub unlockit( lockfilename,  lockkey,  locklist);
		return 1;
	} else if (mode == "UNLOCKALL") {
		gosub unlockall( locklist,  msg);
		return 1;
	} else {
		call note(DQ ^ (mode ^ DQ) ^ " is invalid in LOCKING");
	}
//L277:
	return 0;

///////
lockit:
///////

	lockitem = lockfilename ^ "*" ^ lockkey;
	if (locklist.locateusing(lockitem, FM, lockn)) {
		//msg=quote(lockitem):' already locked'
		return 1;
	}

	if (not(lockfile.open(lockfilename, ""))) {
		//call fsmsg()
		msg = DQ ^ (lockfilename ^ DQ) ^ " file cannot be opened in LOCKING";
		return 0;
	}

	var tryn = 1;
tryagain:
	if (not(lockrecord("", lockfile, lockkey, "", 0, allowduplicate))) {
		var lockholder = (lockfilename ^ "*" ^ lockkey).xlate("LOCKS", 4, "X");
		if (lockholder == "") {
			lockholder = "Someone, maybe you,";
		}
		//if lockdesc then
		gosub getlockdesc( lockdesc,  lockfilename,  lockkey);
		if (interactive) {
			call note(lockholder ^ " is using the " ^ lockdesc, "T1", xx, "");
			if (esctoexit()) {
				return 0;
			}else{
				goto tryagain;
			}
		}else{
			msg = lockholder ^ " is updating|" ^ lockdesc ^ "||PLEASE TRY LATER";
			if (tryn >= ntries) {
				return 0;
			}else{
				if (tryn == 1) {
					printl(msg, " trying for ", ntries, " secs");
				} else if (ntries < 1000) {
					print("*");
				}
				msg = "";
				call ossleep(1000*1);
				tryn += 1;
				goto tryagain;
			}
		}
		// end

		return 0;

	}

	locklist.r(-1, lockitem);

	return 1;

}

subroutine getlockdesc(io lockdesc, in lockfilename, in lockkey) {
	//getlockdesc(io lockdesc, in lockfilename, in lockkey)
	if (not lockdesc) {
		lockdesc = lockfilename ^ " " ^ lockkey;
		lockdesc.converter(".", " ");
	}

	return;

}

subroutine unlockit(in lockfilename, in lockkey, io locklist) {
	//unlockit(in lockfilename, in lockkey, io locklist)
	if (lockfile.open(lockfilename, "")) {
		xx = unlockrecord("", lockfile, lockkey);
	}

	if (locklist.locateusing(lockfilename ^ "*" ^ lockkey, FM, lockn)) {
		locklist.eraser(lockn);
	}

	return;

}

subroutine unlockall(io locklist, io msg) {
	//unlockall(io locklist, io msg)
	var filename2 = "";
	var file = "";
	for (lockn = 1; lockn <= 9999; ++lockn) {
		lockitem = locklist.a(lockn);
	///BREAK;
	if (not lockitem) break;;
		var filename = lockitem.field("*", 1);
		var lockkeyx = lockitem.field("*", 2, 9999);
		if (filename ne filename2) {
			if (not(lockfile.open(filename, ""))) {
				msg.r(-1, DQ ^ (filename ^ DQ) ^ " file cannot be opened in LOCKING,UNLOCKALL");
				lockfile = "";
			}
			filename2 = filename;
		}
		if (lockfile ne "") {
			xx = unlockrecord("", lockfile, lockkeyx);
		}
	};//lockn;

	locklist = "";

	//if msg then return 0

	return;

}


libraryexit()
