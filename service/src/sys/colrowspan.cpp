#include <exodus/library.h>
libraryinit()

#include <system_common.h>

#include <sys_common.h>

var nobase;//num
var t2;
var colspan;//num
var rowspan;//num

function main(io colhdg, in thproperties, in nobase0, in basecurrcode) {

	//called from nlist, adxtab


	//given fm list of column heading and vm per row of column heading
	//return the same but converted to tagged something like <th>xx</th>
	//but with rowspan= and colspan= inserted to span any duplicates

	//thproperties can be fm list otherwise one is used for all

	if (nobase0.unassigned()) {
		nobase = 0;
	} else {
		nobase = nobase0;
	}

	//determine ncols
	var ncols2 = colhdg.count(FM) + 1;

	//determine nrows
	var nrows = 1;
	for (const var coln : range(1, ncols2)) {
		var tt = colhdg.f(coln).count(VM) + 1;
		if (tt gt nrows) {
			nrows = tt;
		}
	} //coln;

	var thprop = thproperties;
	var nocell = "%~%";
	// Additional space to defeat convsyntax until clanf-format
	for ( var rown = 1; rown <= nrows; rown++) {

		// Additional space to defeat convsyntax until clang-format
		for ( var coln = 1; coln <= ncols2; coln++) {
		// Reverted because we do need to skip coln's
		//for (const var coln : range(1, ncols2)) {

			var tt = colhdg.f(coln, rown);
			if (tt eq nocell) {
				goto nextcoln;
			}

			//t2='<th style="background-color:':thcolor:'"'
			if (thproperties.contains(FM)) {
				thprop = thproperties.f(coln);
			}
			t2 = "\r\n" " <th " ^ thprop;

			colspan = 1;
			rowspan = 1;

			//determine any colspan (duplicate cells to the right)
			while (true) {
				var coln2 = coln + colspan;
				var t3 = colhdg.f(coln2, rown);
				///BREAK;
				if (not((coln2 le ncols2 and t3 eq tt) and t3 ne nocell)) break;
				colspan += 1;
				//colhdg(coln2, rown) = nocell;
				pickreplacer(colhdg, coln2, rown, nocell);
			}//loop;

			//if usecols else t:=coldict(coln)<14>
			if (colspan gt 1) {
				t2 ^= " colspan=" ^ colspan ^ " align=center";

			} else {

				//determine any rowspan (duplicate cells below)
				while (true) {
					var rown2 = rown + rowspan;
					var t3 = colhdg.f(coln, rown2);
					///BREAK;
					if (not((rown2 le nrows and ((t3 eq tt or t3 eq ""))) and t3 ne nocell)) break;
					rowspan += 1;
					//colhdg(coln, rown2) = nocell;
					pickreplacer(colhdg, coln, rown2, nocell);
				}//loop;

				if (rowspan gt 1) {
					t2 ^= " rowspan=" ^ rowspan;
				}
			}

			t2 ^= ">" ^ tt ^ "</th>";
			//colhdg(coln, rown) = t2;
			pickreplacer(colhdg, coln, rown, t2);

			coln += colspan - 1;

nextcoln:;
		} //coln;

	} //rown;
	colhdg.replacer(nocell, "");

	colhdg = invertarray(colhdg);
	colhdg.replacer(FM, "</tr><tr>");
	colhdg.replacer(VM, "");
	colhdg = "<tr>" ^ colhdg ^ "</tr>";

	//change all "(Base)" in dictionary column headings to the base currency
	//unless the keyword NO-BASE is present in which case replace with blank
	//this is useful if all the columns are base and no need to see the currency
	//var basecurrcode = sys.company.f(3);
	//t2 = sys.company.f(3);
	t2 = basecurrcode;
	if (t2) {
		if (nobase) {
			t2 = "";
		} else {
			t2 = "(" ^ t2 ^ ")";
		}
		colhdg.replacer("(Base)", t2);
		colhdg.replacer("%BASE%", basecurrcode);
	}

	return 0;
}

libraryexit()
