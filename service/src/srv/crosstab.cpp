#include <exodus/library.h>
libraryinit()

#include <exodus/htmllib2.h>
#include <gethtml.h>

#include <service_common.h>

#include <srv_common.h>

var cmdline;  // num
int nrows;		  // num
int ncols;		  // num
var file;
var dictvoc;
var rowfields;
var nrowfields;
dim rowdict;
dim rowfieldismv;
//int rowfn;	// num
var rowfield;
var totcol;
//var colnx;  // num
var colorder;
var coldict;
var datadict;
var prefixn;  // num
var prefix;
int recn;  // num
var tt;
var nmvs;  // num
//var xx;
var datavals;  // num
var nrowvals;  // num
var rowvals;
var fieldname;
var nn;
var colvals;
var ncolvals;
//var rowvaln;  // num
var rowval;
//var rown;  // num
var msg;
//var colvaln;  // num
var colval;
//var rownx;	 // num
var oldval;	 // num
//var colconv;
//var rowconv;

var printptr;	// num
var topmargin;	// num
var tx;
var system;
var printfilename;
var html;
var ownprintfile;  // num
var ptx_filenamelen;
var ptx_random;
var printfile;
var letterhead;
var pagelns;
var bodyln;		// num
var realpagen;	// num
var pagen;		// num
var newpage;	// num
var bottomline;
var printtxmark;
var rfmt;
var head;
var foot;
var ntxlns;	   // num
var nbodylns;  // num
var ptx_temp;
var headx;
var newpagetag;
var ptx_css;
var cssver;
var style;
var stylennx;  // num
var htmltitle;
var head_or_foot;
var footx;
var head1;
var optioncharn;  // num
var optionchars;
var optionchar;
var newoptions;
var printtx_ii;	 // num
var spaceoptionsize;

function main(in filename, in rowfields0, in colfield, in datafield, io output, io filterdictid, io filterin, io filterout, io allrowvals, io allcolvals, io prefixes, io prefixmvfn) {

	// for printtx

	// if prefixes then it will not select records but read sequentially
	// prefix*1 prefix*2 etc

	cmdline = 0;

	// /////
	// init:
	// /////
	if ((output.unassigned() or allrowvals.unassigned()) or allcolvals.unassigned()) {
		allcolvals = "";
		allrowvals = "";
		output	   = "";
	}
	if (prefixes.unassigned()) {
		prefixes   = "";
		prefixmvfn = "";
	}

	nrows = allrowvals.fcount(VM);
	ncols = allcolvals.fcount(VM);

	if (filterdictid.unassigned()) {
		filterdictid = "";
	}
	if (filterin.unassigned()) {
		filterin = "";
	}
	if (filterout.unassigned()) {
		filterout = "";
	}

	if (not file.open(filename, "")) {
		abort(lasterror());
	}
	if (not DICT.open("DICT." ^ filename, "")) {
		abort(lasterror());
	}
	if (not dictvoc.open("DICT.voc", "")) {
		abort(lasterror());
	}

	// selectcmd='SELECT ':filename
	// selectcmd:=' BY ':colfield

	rowfields = rowfields0;
	rowfields.converter(",", VM);
	nrowfields = rowfields.fcount(VM);
	rowdict.redim(20);
	rowfieldismv.redim(20);
	for (const int rowfn : range(1, nrowfields)) {
		rowfield = rowfields.f(1, rowfn);
		if (not rowdict[rowfn].read(DICT, rowfield)) {
			if (not rowdict[rowfn].read(dictvoc, rowfield)) {
				call mssg(rowfield.quote() ^ " row field doesnt exist in " ^ filename);
				stop();
			}
		}
		rowfieldismv[rowfn] = rowdict[rowfn].f(4) != "S";
		if ((rowfieldismv[rowfn] and not(prefixmvfn)) and rowdict[rowfn].f(1) == "F") {
			prefixmvfn = rowdict[rowfn].f(2);
		}
	}  // rowfn;

	totcol = colfield == "TOTAL";
	var colnx   = 1;
	if (totcol) {
		colorder = "AR";
	} else {
		if (not coldict.read(DICT, colfield)) {
			if (not coldict.read(dictvoc, colfield)) {
				call mssg(colfield.quote() ^ " column field doesnt exist in " ^ filename);
				stop();
			}
		}

		if (coldict.f(9) == "R") {
			colorder = "AR";
		} else {
			colorder = "AL";
		}
	}

	if (not datadict.read(DICT, datafield)) {
		if (not datadict.read(dictvoc, datafield)) {
			call mssg(datafield.quote() ^ " data field doesnt exist in " ^ filename);
			stop();
		}
	}

	if (prefixes) {
		prefixn = 0;
nextprefix:
		// /////////
		prefixn += 1;
		prefix = prefixes.f(1, prefixn);
		if (not prefix) {
			goto exit;
		}
		recn = 0;
		MV	 = 0;

	} else {
		if (not LISTACTIVE) {
			select(file);
		}
		// perform selectcmd
	}

nextrecord:
	// /////////
	if (prefixes) {
		recn += 1;
		MV = 0;
		ID = prefix ^ recn;
	} else {
		if (not readnext(ID, MV)) {
			goto exit;
		}
	}
	if (not RECORD.read(file, ID)) {
		if (prefixes) {
			goto nextprefix;
		}
		goto exit;
	}

	if (prefixmvfn) {
		tt	 = RECORD.f(prefixmvfn);
		nmvs = tt.fcount(VM);
nextmv:
		// /////
		// if prefixes else goto nextrecord
		MV += 1;
		if (MV > nmvs) {
			goto nextrecord;
		}
	} else {
		MV	 = 1;
		nmvs = 1;
	}

	if (filterdictid) {
		if (filterdictid.isnum()) {
			tt = RECORD.f(filterdictid, MV);
		} else {
			tt = calculate(filterdictid);
		}
		if (filterin) {
			if (not filterin.locate(tt)) {
				goto nextmv;
			}
		}
		if (filterout) {
			if (filterout.locate(tt)) {
				goto nextmv;
			}
		}
	}

	if (datafield.len() == 0) {
		datavals = 1;
	} else if (datafield.match("^\\d*$")) {
		datavals = RECORD.f(datafield);
	} else {
		datavals = calculate(datafield);
	}

	nrowvals = 0;
	rowvals	 = "";
	if (nrowfields) {
		for (const int rowfn : range(1, nrowfields)) {
			fieldname = rowfields.f(1, rowfn);
			tt		  = calculate(fieldname);
			if (fieldname == "HOUR") {
				tt = ("00" ^ tt).last(2);
			}
			nn = tt.fcount(VM);
			if (nn > nrowvals) {
				nrowvals = nn;
			}
			pickreplacer(rowvals, rowfn, tt);
		}  // rowfn;
	} else {
		rowvals	 = "Total";
		nrowvals = 1;
	}

	if (totcol) {
		colvals = "Total";
	} else {
		colvals = calculate(colfield);
		if (colfield == "HOUR") {
			colvals = ("00" ^ colvals).last(2);
		}
	}
	ncolvals = colvals.fcount(VM);
	for (const int rowvaln : range(1, nrowvals)) {

		// build row value (multiple fields sm separated)
		if (nrowfields) {
			rowval = "";
			for (const int rowfn : range(1, nrowfields)) {
				if (rowfieldismv[rowfn]) {
					tt = rowvals.f(rowfn, rowvaln);
				} else {
					tt = rowvals.f(rowfn, 1);
				}
				pickreplacer(rowval, 1, 1, rowfn, tt);
			}  // rowfn;
		} else {
			rowval = rowvals;
		}

		// determine which row to add into
		var rown;
		if (not allrowvals.f(1).locateby("AL", rowval, rown)) {
			if (allrowvals.len() + rowval.len() > 65000) {
				gosub abort_toobig(filename);
				//std::unreachable()
			}
			nrows += 1;
			allrowvals.inserter(1, rown, rowval);
			output.inserter(rown + 1, "");
		}

		for (const int colvaln : range(1, ncolvals)) {

			// determine which column to add into
			colval = colvals.f(1, colvaln);
			if (not allcolvals.f(1).locateby(colorder, colval, colnx)) {
				ncols += 1;
				if (allcolvals.len() + colval.len() > 65000) {
					gosub abort_toobig(filename);
					//std::unreachable()
				}
				allcolvals.inserter(1, colnx, colval);

				if (output.len() + nrows * 2 > 65000) {
					gosub abort_toobig(filename);
					//std::unreachable()
				}

				output.inserter(1, 1 + colnx, colval);
				for (const int rownx : range(2, nrows + 1)) {
					output.inserter(rownx, 1 + colnx, "");
				}  // rownx;
			}

			if (output.len() + datavals.len() > 6500) {
				gosub abort_toobig(filename);
				//std::unreachable()
			}

			oldval = output.f(rown + 1, colnx + 1);
			// output(rown + 1, colnx + 1) = oldval + datavals;
			// output(1, 1) = output.f(1, 1) + 1;
			pickreplacer(output, rown + 1, colnx + 1, oldval + datavals);
			pickreplacer(output, 1, 1, output.f(1, 1) + 1);

			// total column at the end
			if (not totcol) {
				oldval = output.f(rown + 1, ncols + 2);
				// output(rown + 1, ncols + 2) = oldval + datavals;
				pickreplacer(output, rown + 1, ncols + 2, oldval + datavals);
			}

		}  // colvaln;

	}  // rowvaln;

	// ///////
	// recexit:
	// ///////
	// if prefixes then goto nextmv
	goto nextmv;
	// goto nextrecord

/////
exit:
	// ///
	// format the column title values
	if (not totcol) {
		let colconv = coldict.f(7);
		if (colconv) {
			for (const int coln : range(1, ncols)) {
				output(1, 1 + coln) = oconv(output.f(1, 1 + coln), colconv);
			}
		}
	}

	if (totcol) {
		output(1, 1 + colnx) = datadict.f(3);
	} else {
		for (const int coln : range(1, ncols)) {
			output(1, 1 + coln) = coldict.f(3) ^ " " ^ output.f(1, 1 + coln);
		}
		output(1, ncols + 2) = "Total " ^ datadict.f(3);
	}

	for (const int rown : range(1, nrows)) {
		if (output.len() + allrowvals.len() > 65000) {
			gosub abort_toobig(filename);
			//std::unreachable()
		}
		output(rown + 1, 1) = allrowvals.f(1, rown);
	}  // rown;

	// format the row title values
	output(1, 1, 1) = "";
	for (const int rowfn : range(1, nrowfields)) {
		// output<1,1,rowfn>=rowfields<1,rowfn>
		output(1, 1, rowfn) = rowdict[rowfn].f(3);

		let rowconv = rowdict[rowfn].f(7);
		if (rowconv) {
			for (const int rown : range(1, nrows)) {
				output(rown + 1, 1, rowfn) = oconv(output.f(rown + 1, 1, rowfn), rowconv);
			}
		}

	}  // rowfn;

	// convert sm row keys into columns
	output.converter(SM, VM);

	return 0;
}

subroutine_noreturn abort_toobig(in filename) {

	clearselect();
	msg = "Crosstab too complex. Please simplify your request.";
	if (filename == "STATISTICS") {
		msg(-1) = "Perhaps choose Columns=Date and dont select Session or IP No.";
	}
	abort(msg);
}
libraryexit()
