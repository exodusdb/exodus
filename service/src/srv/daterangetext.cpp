#include <exodus/library.h>
libraryinit()

#include <service_common.h>

#include <srv_common.h>

function main(in d1, in d2, out result, io lang) {

	// returns minimal text representation of date range
	// if @sentence='DATERANGETEXT' then goto testit
	// 1 Jan 2010
	// Jan 2010
	// 2 - 20 Jan 2010
	// Jan - Feb 2010
	// 15 Feb - 20 Mar 2010
	// Jan - Dec 2010
	// Feb 2010 - Jan 2011
	// 10 Mar 2010 - 5 Feb 2010

	if (lang.unassigned()) {
		lang = srv.glang;
	}

	var t2 = d2.oconv("D/E");
	var tt = d1.oconv("D/E") ^ "/" ^ t2;
	tt.converter("/", FM);

	tt(1) = tt.f(1) + 0;
	tt(4) = tt.f(4) + 0;

	if (tt.f(3) == tt.f(6)) {
		// dont show both years if same
		tt(3) = "";
		if (tt.f(2) == tt.f(5)) {
			// dont show both months if same year and month
			tt(2) = "";
		}
	}

	// dont show start and end day of month if complete calendar months
	if (tt.f(1) == 1) {
		// calculate end of month of stop date
		t2 = iconv(t2.oconv("D2/E").field("/", 2, 2), "[DATEPERIOD]");
		// if (tt.f(4) == t2.oconv("D2/E").field("/", 1)) {
		if (tt.f(4) == t2.oconv("DD")) {
			tt(1) = "";
			tt(4) = "";
		}
	}

	// monthnames
	if (tt.f(2)) {
		tt(2) = lang.f(2).field("|", tt.f(2));
	}
	if (tt.f(5)) {
		tt(5) = lang.f(2).field("|", tt.f(5));
	}

	// eliminate first dom if same dom
	if ((tt.f(1) == tt.f(4) and not(tt.f(2))) and not(tt.f(3))) {
		tt(1) = "";
	}

	// add hyphen if still a range
//	if (not(tt.first(3) eq(FM ^ FM ^ FM))) {
	if (not tt.starts(_FM _FM _FM)) {
		if (tt.f(2) == "" and tt.f(3) == "") {
			tt(3) = tt.f(3) ^ " ^ ";
		} else {
			tt(3) = tt.f(3) ^ " - ";
		}
	}

	tt.converter(FM, " ");
	result = tt.trim();
	result.replacer(" ^ ", "-");

	return 0;

	/*;
	testit:
			@sentence='';
			dd='1/1/2010 1/1/2010';gosub test;
			dd='2/1/2010 2/1/2010';gosub test;
			dd='1/1/2010 31/1/2010';gosub test;
			dd='2/1/2010 31/1/2010';gosub test;

			dd='1/1/2010 28/2/2010';gosub test;
			dd='15/1/2010 21/1/2010';gosub test;
			dd='15/2/2010 21/3/2010';gosub test;

			dd='1/1/2010 31/12/2010';gosub test;

			// multi-year
			dd='1/2/2010 31/1/2011';gosub test;
			dd='1/1/2010 31/12/2011';gosub test;
			dd='10/3/2010 5/2/2011';gosub test;

			return 0;

	test:
			d1=iconv(field(dd,' ',1),'D2/E');
			d2=iconv(field(dd,' ',2),'D2/E');

			call daterangetext(d1,d2,result,lang);
			call msg(dd:fm:result);
			return 0;
	*/
}

libraryexit()
