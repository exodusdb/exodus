#include <exodus/library.h>
libraryinit()

#include <authorised.h>
#include <locking.h>
#include <singular.h>

#include <service_common.h>

#include <srv_common.h>
#include <req_common.h>

#include <request.hpp>

	var msg;
var deleting;	 // num
var validating;	 // num
var mastercode;
var vn;
var op;
var op2;
var wspos;
var wsmsg;
var xx;

function main(in mode, in subfn, in masterfilename, io masterfile, in masterfn, io locklist) {

	// also needs
	// datafile,src.file

	// and (except build)
	// @id,@record,orec

	if (masterfile.unassigned()) {
		if (not(masterfile.open(masterfilename, ""))) {
			msg = masterfilename.quote() ^ " file does not exist";
			return invalid(msg);
		}
	}

	if (mode eq "BUILD") {

		// zzz build updates masterfile without locking
		// so should be run exclusively

		clearselect();
		perform("SELECT " ^ req.datafile ^ " (S)");
		if (not LISTACTIVE) {
			return 0;
		}

nextbuild:
		if (readnext(ID)) {
			if (not(RECORD.read(req.srcfile, ID))) {
				goto nextbuild;
			}
			if (RECORD.f(subfn) eq "") {
				goto nextbuild;
			}
			deleting   = 0;
			validating = 0;
			gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);
			goto nextbuild;
		}

	} else if (mode eq "PREWRITE") {
		if (req.orec.f(subfn) eq RECORD.f(subfn)) {
			return 0;
		}
		validating = 1;

		deleting = 1;
		gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);
		if (not(req.valid)) {
			return 0;
		}

		deleting = 0;
		gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);

	} else if (mode eq "PREDELETE") {
		validating = 1;
		deleting   = 1;
		gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);

	} else if (mode eq "WRITE") {

		validating = 1;

		deleting = 1;
		gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);
		if (not(req.valid)) {
			return 0;
		}

		deleting = 0;
		gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);
		if (not(req.valid)) {
			return 0;
		}

		validating = 0;

		deleting = 1;
		gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);
		if (not(req.valid)) {
			return 0;
		}

		deleting = 0;
		gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);
		if (not(req.valid)) {
			return 0;
		}

		gosub unlockall(locklist);

	} else if (mode eq "DELETE") {
		deleting = 1;

		validating = 1;
		gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);

		validating = 0;
		gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);

		gosub unlockall(locklist);

	} else if (mode eq "POSTWRITE") {
		if (req.orec.f(subfn) eq RECORD.f(subfn)) {
			return 0;
		}
		validating = 0;

		deleting = 1;
		gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);

		deleting = 0;
		gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);

	} else if (mode eq "POSTDELETE") {
		deleting   = 1;
		validating = 0;
		gosub validateupdate(subfn, masterfilename, masterfile, masterfn, locklist);

	} else {
		msg = mode.quote() ^ " invalid mode in xref.subs";
		return invalid(msg);
	}

	return 0;
}

subroutine validateupdate(in subfn, in masterfilename, in masterfile, in masterfn, io locklist) {

	if (deleting) {
		mastercode = req.orec.f(subfn);
	} else {
		mastercode = RECORD.f(subfn);
	}
	if (mastercode eq "") {
		return;
	}

	if (validating) {

		if (not(locking("LOCK", masterfilename, mastercode, "", locklist, 1, msg))) {
			gosub unlockall(locklist);
			msg = mastercode.quote() ^ " master record is in use elsewhere|Try again later";
			gosub invalid(msg);
			return;
		}

	} else {

		var masterrecord;
		if (masterrecord.read(masterfile, mastercode)) {
			var origmasterrecord = masterrecord;
			if (masterrecord.f(masterfn).locateby("AL", ID, vn)) {
				if (deleting) {
					masterrecord.remover(masterfn, vn);
				}
			} else {
				if (not deleting) {
					masterrecord.inserter(masterfn, vn, ID);
				}
			}
			if (masterrecord ne origmasterrecord) {
				masterrecord.write(masterfile, mastercode);
			}
		} else {
		}
	}

	return;
}

subroutine unlockall(io locklist) {

	call locking("UNLOCKALL", "", "", "", locklist, 0, msg);
	return;
}

libraryexit()