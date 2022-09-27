#include <exodus/library.h>
libraryinit()

#include <btreeextract.h>

#include <system_common.h>

#include <sys_common.h>

var foundkeys;

function main(in filename, in indexid, in texts0, io msg) {
	//c sys in,in,in,io

	var texts = texts0;
	msg = "";
	var ntexts = texts.count(VM) + 1;

	//analogous code whereever allpunctuation is used
	//ED BP LISTSCHED AGENCY.SUBS SCHEDULES.DICT
	var allpunctuation = SYSTEM.f(130);
	texts.converter(allpunctuation, var(50).space());
	var temp = texts.trim();

	temp.replacer(" ", "&");
	//if 1 then
	temp.replacer("&", "]&");
	temp.replacer(VM, "]" ^ VM);
	temp ^= "]";
	// end

	var srcdict;
	if (not(srcdict.open("DICT." ^ filename, ""))) {
		call fsmsg();
		stop();
	}

	temp = indexid ^ VM ^ temp ^ FM;
	call btreeextract(temp, filename, srcdict, foundkeys);

	//suppress maximum message
	//similar code in LISTSCHED and AGENCY.SUBS
	if (msg_.contains("maximum")) {
		msg_ = "Too many records found. Some may have been excluded.";
	}

	if (not foundkeys) {
		msg = "No " ^ filename.lcase() ^ " found for " ^ texts;
		clearselect();
		return 0;
	}

	var lists;
	if (not(lists.open("LISTS", ""))) {
		call fsmsg();
		stop();
	}
	foundkeys.converter(VM, FM);
	var listkey = "SELECTBTREE:" ^ SYSTEM.f(24);
	foundkeys.write(lists, listkey);
	getlist("" ^ listkey ^ " (S)");
	//delete lists,listkey

	return 0;
}

libraryexit()
