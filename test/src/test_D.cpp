#undef NDEBUG //because we are using assert to check actual operations that cannot be skipped in release mode testing
#include <cassert>

#include <exodus/program.h>

programinit()

function main()
{

		// C++ double to int is symmetrical about 0
		assert(int(2.9) == 2);
		assert(int(-2.9) == -2);

		// BUT exodus and pickos int()
		// is the floor function which is *not* symmetrical
		// float -> nearest integer towards negative infinity
		// 1.999 -> 1 and -1.001 -> -2
		assert(var(1.9999).toInt() == 1);
		assert(var(-1.9999).toInt() == -2);

		// exodus round function is symmetrical
		// and rounds 0.5 up and -0.5 down
		// which is 
		assert(var(1.5).toInt() == 1);
		assert(var(-1.5).toInt() == -2);

		// pickos day zero is "1968/12/31"
		// "1968/12/31" <-> 0
		// Fractional days eg 1.5 are handled by floor() function

		// [-1 to 0) is "30 DEC 1967" (pickos day -1)
		assert(oconv(-1,"D") eq "30 DEC 1967");
		assert(oconv(-0.9999999,"D") eq "30 DEC 1967");
		assert(oconv(-0.5,"D") eq "30 DEC 1967");
		assert(oconv(-0.0000001,"D") eq "30 DEC 1967");

		// [0 to 1) is "31 DEC 1967" (pickos day 0)
		assert(oconv(0,"D") eq "31 DEC 1967");
		assert(oconv(0.5,"D") eq "31 DEC 1967");
		assert(oconv(0.9999999,"D") eq "31 DEC 1967");

		// [1 to 2) is "01 JAN 1968" (pickos day 1)
		assert(oconv(1.0,"D") eq "01 JAN 1968");
		assert(oconv(1.5,"D") eq "01 JAN 1968");
		assert(oconv(1.9999999,"D") eq "01 JAN 1968");

	{
		// bad date testing

		// General pattern is:
		// JUNK1
		// (digitgroup | alphagroup)
		// JUNK2
		// (digitgroup | alphagroup)
		// JUNK2
		// (digitgroup | alphagroup)
		//
		// Where JUNK1 is [^A-Za-z0-9]*
		// Where JUNK2 is [^A-Za-z0-9]+
		// alphagroup is three letter month JAN-DEC case insensitive and can only appear once
		// digitgroup is \d{1,2} and can only appear twice if alphagroup exists

		// iconv of two digit years
		// years 0-49 are assumed to be 2000-2049
		// years 50-99 are assumed to be 1950-1999

		// any junk is allowed on the left and between digit groups and alpha groups but not at the end
		assert(iconv("(*& 1 *^^&* 2 &^%&^% 2000","D"));

		// lower case latin month is ok (unlike pickos?)
		assert(iconv("1 jan 2000","D").outputl());
		assert(iconv("1 JAN 2020","D").outputl());

		assert(not iconv("1 JAN JAN 2020","D").outputl());

		assert(not iconv("","D").outputl());
		assert(not iconv("xxx","D").outputl());
		assert(not iconv("jan","D").outputl());
		assert(not iconv("jan 2000","D").outputl());
		assert(not iconv("jan 2000","D").outputl());
		assert(not iconv("1 jan","D").outputl());

		assert(iconv("1 2 2000","D").outputl());

		assert(not iconv("1 2 2000 ","D").outputl());
		assert(not iconv("1 2 2000 333","D").outputl());
		assert(not iconv("1/2/2000/","D").outputl());
		assert(not iconv("1 XYZ 2000","D").outputl());
		assert(not iconv("1 2000","D").outputl());
		assert(not iconv("1","D").outputl());

		assert(iconv("1 jan 2","D").outputl());
		assert(oconv(iconv("1 jan 2","D"),"D").outputl());

		// long month is not ok
		assert(not iconv("1 january 2000","D").outputl());

		assert(var(10000).oconv("DY0") eq "");

		assert(iconv("1 2 2000","D"));
		assert(not iconv("1 2 2000 333","D"));
		assert(not iconv("1/2/2000/","D"));
		assert(not iconv("1 XYZ 2000","D"));
		assert(not iconv("1 2000","D"));
		assert(not iconv("1","D"));
	}

	assert(oconv(6666,"D2-") eq "04-01-86");

	assert(var(1000).oconv("MD80") eq "1000.00000000");
	assert(var("31 JAN 2008").iconv("D") eq "14641");

	assert(var("1/31/2008").iconv("D") eq 14641);
	assert(var("2008/1/31").iconv("DS") eq "14641");

	assert(var("JAN/31/2008").iconv("D") eq "14641");
	assert(var("2008/JAN/31").iconv("DS") eq "14641");

	assert(var("1/1/92").iconv("D/E") eq "8767");

	assert(var("1/1/68").iconv("D/E") eq "1");
	assert(var("31/12/67").iconv("D/E") eq "0");
	assert(var("30/12/67").iconv("D/E") eq "-1");
	assert(var("1/1/1900").iconv("D/E") eq "-24835");

	assert(var("31/1/2008").iconv("DE") eq "14641");
	assert(var("31/1/2008").iconv("D/E") eq "14641");
	assert(var("31 1 2008").iconv("DE") eq "14641");
	assert(var("31-1-2008").iconv("DE") eq "14641");
	assert(var("31/JAN/2008").iconv("DE") eq "14641");
	assert(var("JAN/31/2008").iconv("DE") eq "14641");

	assert(var("29 FEB 1900").iconv("D") eq "");               //centuries not divisible by 400 are not leap years
	assert(var("29 FEB 2000").iconv("D").outputl() eq "11748");//centuries divisible by 400 are leap years
	assert(var("29 FEB 2008").iconv("D") eq "14670");          //years divisible by 4 are leap years
	assert(var("29 FEB 2009").iconv("D") eq "");               // years not divisible 4 are leap years

	assert(var("30 FEB 1900").iconv("D") eq "");
	assert(var("30 FEB 2000").iconv("D") eq "");
	assert(var("30 FEB 2008").iconv("D") eq "");
	assert(var("30 FEB 2009").iconv("D") eq "");

	assert(var("31 FEB 1900").iconv("D") eq "");
	assert(var("31 FEB 2000").iconv("D") eq "");
	assert(var("31 FEB 2008").iconv("D") eq "");
	assert(var("31 FEB 2009").iconv("D") eq "");

	assert(var("32 JAN 1900").iconv("D") eq "");
	assert(var("32 JAN 2000").iconv("D") eq "");
	assert(var("32 JAN 2008").iconv("D") eq "");
	assert(var("32 JAN 2009").iconv("D") eq "");

	// Check which months have 31 and which do not
	assert(var("31-01-2000").iconv("DE").outputl() eq "11719");
	assert(var("31-02-2000").iconv("DE").outputl() eq "");
	assert(var("31-03-2000").iconv("DE").outputl() eq "11779");
	assert(var("31-04-2000").iconv("DE").outputl() eq "");
	assert(var("31-05-2000").iconv("DE").outputl() eq "11840");
	assert(var("31-06-2000").iconv("DE").outputl() eq "");
	assert(var("31-07-2000").iconv("DE").outputl() eq "11901");
	assert(var("31-08-2000").iconv("DE").outputl() eq "11932");
	assert(var("31-09-2000").iconv("DE").outputl() eq "");
	assert(var("31-10-2000").iconv("DE").outputl() eq "11993");
	assert(var("31-11-2000").iconv("DE").outputl() eq "");
	assert(var("31-12-2000").iconv("DE").outputl() eq "12054");

	assert(var("32/1/2008").iconv("DE") eq "");
	assert(var("30/2/2008").iconv("DE") eq "");
	assert(var("1/31/2008").iconv("DE") eq "");

	//check uses yyyy-mm-dd format if data starts with four digit year
	assert(iconv("2000-12-31","DE") == 12054);

	assert(oconv(15036, "D")     == "01 MAR 2009");
	assert(oconv(15036, "D2")    == "01 MAR 09");
	assert(oconv(15036, "D4")    == "01 MAR 2009");

	assert(oconv(15036, "D/")    == "03/01/2009");
	assert(oconv(15036, "D2/")   == "03/01/09");
	assert(oconv(15036, "D*")    == "03*01*2009");

	assert(oconv(15036, "D/E")   == "01/03/2009");
	assert(oconv(15036, "D2E")   == "01 MAR 09");
	assert(oconv(15036, "D2/E")  == "01/03/09");

	assert(oconv(15036, "DQ")   == "1");
	assert(oconv(15036, "DW")   == "7");
	assert(oconv(15036, "DWA")  == "SUNDAY");



	assert(oconv(6666,"D2-")                    eq "04-01-86");
	assert(oconv(0,"D4") eq "31 DEC 1967");
	assert(oconv("","D") eq "");
	assert(oconv("X","D") eq "X");
	assert(oconv("-1.5","D") eq "29 DEC 1967");
	assert(oconv("1.5","D") eq "01 JAN 1968");
	assert(oconv("1.5" _FM_ "-1.5","D") eq ("01 JAN 1968" _FM_ "29 DEC 1967"));

	assert(oconv(14276,"D").outputl() eq "31 JAN 2007");
	assert(oconv(14276,"D2").outputl() eq "31 JAN 07");
	assert(oconv(14276,"D4").outputl() eq "31 JAN 2007");
	assert(oconv(14276,"D/").outputl() eq "01/31/2007");
	assert(oconv(14276,"D ").outputl() eq "01 31 2007");
	assert(oconv(14276,"D2/").outputl() eq "01/31/07");
	assert(oconv(14276,"D2-").outputl() eq "01-31-07");
	assert(oconv(14276,"D/").outputl() eq "01/31/2007");
	assert(oconv(14276,"D/E").outputl() eq "31/01/2007");
	assert(oconv(14276,"D2 E").outputl() eq "31 01 07");
	assert(oconv(14276,"D S").outputl() eq "2007 01 31");
	assert(oconv(14276,"DM").outputl() eq "1");
	assert(oconv(14276,"DMA").outputl() eq "JANUARY");
	assert(oconv(14276,"DW").outputl() eq "3");
	assert(oconv(14276,"DWA").outputl() eq "WEDNESDAY");
	assert(oconv(14276,"DY").outputl() eq "2007");
	assert(oconv(14276,"DY2").outputl() eq "07");
	assert(oconv(14276,"D2Y").outputl() eq "07 JAN 31");
	assert(oconv(14276,"D5Y").outputl() eq "02007 JAN 31");
	assert(oconv(14276,"DD").outputl() eq "31");
	assert(oconv(14276,"DL").outputl() eq "31");
	assert(oconv(14276,"DQ").outputl() eq "1");
	assert(oconv(14276,"DJ").outputl() eq "31");

	var feb29_2004=13209;//iconv("29 FEB 2004","D");
	assert(oconv(feb29_2004,"DL") eq "29");

	//check does multivalues
	assert(oconv("14591" _VM_ _VM_ "14592", "D") eq "12 DEC 2007" _VM_ _VM_ "13 DEC 2007");
	assert(oconv("14591" _FM_ _VM_ "14592", "D") eq "12 DEC 2007" _FM_ _VM_ "13 DEC 2007");

	assert(oconv(14591,"D") eq "12 DEC 2007");
	assert(oconv(14591,"D2/") eq "12/12/07");
	assert(oconv(14591,"D2-") eq "12-12-07");
	assert(oconv(14591,"D-") eq "12-12-2007");
	assert(oconv(14591,"D2-") eq "12-12-07");
	assert(oconv(14591,"DJ") eq "346");
	assert(oconv(14591,"DM") eq "12");
	assert(oconv(14591,"DMA") eq "DECEMBER");
	assert(oconv(14591,"DW") eq "3");
	assert(oconv(14591,"DWA") eq "WEDNESDAY");
	assert(oconv(14591,"DY") eq "2007");
	assert(oconv(14591,"DQ") eq "4");
	assert(oconv(14591,"DD") eq "12");
	assert(oconv(14591,"DL") eq "31");



	printl("Test passed");

	return 0;

}
programexit()