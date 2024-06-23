#include <exodus/library.h>
libraryinit()

#include <service_common.h>

#include <srv_common.h>

var basekey;
var listid;
var lists;
var listn;	// num
var keys;
var key;

function main(io ranges0, in basekey0 = "", in listid0 = "") {

	// generate individual numbers given a list of ranges
	// eg 1000,1100-1200,2000-2100,3000
	// in a comma, space or fm separated list
	//
	// basekey if given will allow creation of formatted numbers
	// 18/M/1000
	//
	// listid if given will store the generated numbers/codes as a list in LISTS
	//
	// Returns: result in ranges0 unless listid given

	// equ maxlistpartsize to 32000
	let maxlistpartsize = maxstrsize_ / 2;

	if (basekey0.unassigned()) {
		basekey = "";
	} else {
		basekey = basekey0;
	}

	if (listid0.unassigned()) {
		listid = "";
	} else {
		listid = listid0;
	}

	if (listid) {
		if (not lists.open("LISTS", "")) {
			abort(lasterror());
		}
		listn = "";
		keys  = "";
	}

	// comma/space/fm separated ranges

	var ranges = ranges0.trim();
	// convert ',/' to '  ' in ranges
	// dont convert / since an invoice number may contain a slash
	ranges.converter(",", " ");
	ranges.trimmer();
	ranges.converter(" ", FM);
	ranges.replacer(" -", "-");
	ranges.replacer("- ", "-");

	let nn = ranges.fcount(FM);

	var allchars = "";
	for (const var ii : range(32, 255)) {
		allchars ^= chr(ii);
	}  // ii;
	allchars.converter("0123456789-", "");

	//for (var ii = nn; ii >= 1; --ii) {
	for (let ii : reverse_range(1, nn)) {

		let rangex = ranges.f(ii);

		// if enter something like P13/1000-2000 then
		// prefix=P13/ start=1000 stop=2000
		// ie TRAILING number range but wont work if trailing year like 1000/13
		// startx=field(temp,'-',1)
		var startx = "";
		var prefix = rangex.field("-", 1);
		while (true) {
			let lastchar = prefix.last();
			// /BREAK;
			if (not var("0123456789").contains(lastchar))
				break;
			startx.prefixer(lastchar);
			prefix.popper();
		}  // loop;

		// range of numbers
		let temp0 = rangex;
		var temp  = temp0;
		temp.converter(allchars, "");

		if (temp.match("^\\d+-\\d+$")) {

			// finish=field(temp,'-',2)
			var finish = "";
			var tt	   = ranges.f(ii).field("-", 2);
			while (true) {
				let lastchar = tt.last();
				// /BREAK;
				if (not var("0123456789").contains(lastchar))
					break;
				finish.prefixer(lastchar);
				tt.popper();
			}  // loop;

			if (finish < startx) {
				ranges0 = "";
				call mssg(startx ^ "-" ^ finish ^ " range is not consecutive");
				return 0;
			}
			var range2 = "";
			for (const var i2 : range(startx, finish)) {
				if (esctoexit()) {
					goto exit;
				}

				if (basekey and not(prefix)) {
					key = basekey;
					key.replacer("%", i2);
				} else {
					key = prefix ^ i2;
				}

				if (listid) {
					keys ^= FM ^ key;
					// assumes key is not too long
					if (keys.len() > maxlistpartsize) {
						gosub writelist();
					}

				} else {
					// if (len(range)+len(ranges))>65000 then
					if (range2.len() + ranges.len() > maxstrsize_ - 530) {
						// call msg('Range exceeds maximum of 65,000 characters|Please enter a smaller range')
						call mssg("Range exceeds maximum|Please enter a smaller range");
						ranges = "";
						goto exit;
					}
					range2 ^= " " ^ key;
				}

			}  // i2;

			if (not listid) {
				range2.cutter(1);
				ranges(ii) = range2;
			}

			// not range
		} else {
			if (listid) {

				if (basekey and not(prefix)) {
makekey:
					key = basekey;
					key.replacer("%", temp);

				} else if (basekey) {
					temp = rangex;
					goto makekey;

				} else {
					// key=prefix:i2
					key = rangex;
				}

				keys ^= FM ^ key;
			}
		}

		// nextrange:;
	}  // ii;

exit:
// ///
	if (listid) {
		if (keys) {
			// return nlistparts in vnos to indicate success
			var vnos = listn;
			if (not vnos) {
				vnos = 1;
			}
			gosub writelist();
		} else {
			// return '' in vnos if none found (unlikely)
			let vnos = "";
		}
	} else {
		ranges0 = ranges;
		ranges0.converter(FM, " ");
	}

	return 0;
}

subroutine writelist() {
	if (not keys.len()) {
		return;
	}
	keys.cutter(1);
	keys.write(lists, listid ^ listn);
	keys = "";
	listn += 1;
	return;
}

libraryexit()
