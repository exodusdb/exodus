#undef NDEBUG  //because we are using assert to check actual operations that cannot be skipped in release mode testing
#include <cassert>
//#include <functional>
//#include <limits>

#include <exodus/program.h>

programinit()

 function main() {

    printl("test_more2c says 'Hello World!'");

	var i  = 3;
	var d  = 7.5;
	var z  = "";
	var si = "8";
	var sd = "7.7";
	var sn = "xyz";

	//compare all different kinds of lhs with rhs double

	{
		//empty string is never equal to any double
		assert(var("") != 3.3);
		assert(var("") != 0.0);

		var xi = "";
		TRACE(xi.toInt())
		xi.dump("xi");
		assert(xi != 0.0);

		var xd = "";
		TRACE(xd.toDouble())
		xd.dump("xd");
		assert(xd != 0.0);

		assert(var(0.0)  eq 0.0);
		assert(var(3.3)  eq 3.3);
		assert(var(-3.3) eq -3.3);
		assert(var(3.4) != 3.3);

		assert(var("0.0")  eq 0.0);
		assert(var("3.3")  eq 3.3);
		assert(var("-3.3") eq -3.3);
		assert(var("3.4") != 3.3);

		assert(var(0)  eq 0.0);
		assert(var(0) != 3.3);
		assert(var(4) != 3.3);
		assert(var(3)  eq 3.0);
		assert(var(-3) eq -3.0);

		assert(var("0")  eq 0.0);
		assert(var("0") != 3.3);
		assert(var("4") != 3.3);
		assert(var("3")  eq 3.0);
		assert(var("-3") eq -3.0);

		assert(var("xxxx") != 3.0);
	}
	{
		//empty string is never equal to any double
		assert(var("") != 3);
		assert(var("") != 0);

		var xi = "";
		TRACE(xi.toInt())
		xi.dump("xi");
		assert(xi != 0);

		var xd = "";
		TRACE(xd.toDouble())
		xd.dump("xd");
		assert(xd != 0);

		assert(var(0)   eq 0);
		assert(var(0.)  eq 0);
		assert(var(3.)  eq 3);
		assert(var(-3.) eq -3);
		assert(var(4.) != 3);

		assert(var("0.0") eq 0);
		assert(var("3.")  eq 3);
		assert(var("-3.") eq -3);
		assert(var("3.0") != 0);

		assert(var(0)  eq 0);
		assert(var(0)  eq -0);
		assert(var(0) != 3);
		assert(var(4) != 3);
		assert(var(3)  eq 3);
		assert(var(-3) eq -3);

		assert(var("0")  eq 0);
		assert(var("-0") eq 0);
		assert(var("0") != 3);
		assert(var("4") != 3);
		assert(var("3")  eq 3);
		assert(var("-3") eq -3);

		assert(var("xxxx") != 3);
		assert(var("xxxx") != 0);
		assert(var("xxxx") != -0);
	}

	// Test comparison with bool true/false
	{
		assert(var("")    eq false);
		assert(var("0")   eq false);
		assert(var("0.")  eq false);
		assert(var("0.0") eq false);

		assert(!var("x")    eq false);
		assert(!var("x")    eq false);
		assert(var("x")     ne false);
		assert(var("x")     ne false);

		assert(!(var("x")    eq true));
		assert(!(var("x")    eq true));
		assert(var("x")     ne true);
		assert(var("x")     ne true);

		assert(var("10")   ne false);
		assert(var("10.")  ne false);
		assert(var("10.0") ne false);

		assert(var("10")   ne true);
		assert(var("10.")  ne true);
		assert(var("10.0") ne true);

		assert(var("1")    eq true);
		assert(var("1.")   eq true);
		assert(var("1.")   eq true);
	}
	{
		assert(false       eq var(""));
		assert(false       eq var("0"));
		assert(false       eq var("0."));
		assert(false       eq var("0.0"));

		assert(!(true eq var("x")));
		assert(!(true eq var("10")));
		assert(!(true eq var("10.")));
		assert(!(true eq var("10.0")));
		assert(true ne var("x"));
		assert(true ne var("10"));
		assert(true ne var("10."));
		assert(true ne var("10.0"));

		assert(!(false eq var("x")));
		assert(!(false eq var("10")));
		assert(!(false eq var("10.")));
		assert(!(false eq var("10.0")));
		assert(false ne var("x"));
		assert(false ne var("10"));
		assert(false ne var("10."));
		assert(false ne var("10.0"));
	}
	{
		assert(var("") != true);
		assert(var("0") != true);
		assert(var("0.") != true);
		assert(var("0.0") != true);

		assert(var("x") != false);
		assert(var("10") != false);
		assert(var("10.") != false);
		assert(var("10.0") != false);
	}
	{
		assert(true != var(""));
		assert(true != var("0"));
		assert(true != var("0."));
		assert(true != var("0.0"));

		assert(false != var("x"));
		assert(false != var("10"));
		assert(false != var("10."));
		assert(false != var("10.0"));
	}

	{
		var x = 1;

		//////////
		//positive
		//////////

		//lvalue

		printl(x++, 1);
		printl(x++, 2);
		printl(x, 3);
		printl(++++x, 4);
		printl(x, 4);

		x = 1;
		assert(x++ eq 1);
		assert(x   eq 2);
		assert(++x eq 3);
		assert(x   eq 3);

		//rvalue

		x = 1;
		//assert(x.f(1)++ eq 1);//pointless. should not compile
		assert(x.f(1)     eq 1);
		//assert(++x.f(1) eq 2);//will not compile because "." priority > "++"
		assert((++x).f(1) eq 2);
		assert(x.f(1)     eq 2);

		//////////
		//negative
		//////////

		//lvalue

		x = 1;
		assert(x-- eq 1);
		assert(x   eq 0);
		assert(--x eq -1);
		assert(x   eq -1);

		//rvalue

		x = 1;
		//assert(x.f(1)-- eq 1);//pointless. should not compile
		assert(x.f(1)     eq 1);
		//assert(--x.f(1) eq 2);//will not compile because "." priority > "--"
		assert((--x).f(1) eq 0);
		assert(x.f(1)     eq 0);
	}

	{
		assert('1' + var(1.1)   eq 2.1);
		assert("1" + var(1.1)   eq 2.1);
		assert("1.1" + var(1.3) eq 2.4);
		assert(1 + var(2.1)     eq 3.1);
		assert(1.1 + var(2.1)   eq 3.2);

		assert(var(1.1) + '1'   eq 2.1);
		assert(var(1.1) + "1"   eq 2.1);
		assert(var(1.3) + "1.1" eq 2.4);
		assert(var(2.1) + 1     eq 3.1);
		assert(var(2.1) + 1.1   eq 3.2);

		assert('1' - var(1.1)   eq -0.1);
		assert("1" - var(1.1)   eq -0.1);
		assert("1.1" - var(1.3) eq -0.2);
		assert(1 - var(2.1)     eq -1.1);
		assert(1.1 - var(2.1)   eq -1.0);

		assert(var(1.1) - '1'   eq 0.1);
		assert(var(1.1) - "1"   eq 0.1);
		assert(var(1.3) - "1.1" eq 0.2);
		assert(var(2.1) - 1     eq 1.1);
		assert(var(2.1) - 1.1   eq 1.0);
	}

	printl(elapsedtimetext());
	printl("Test passed");

	return 0;
}

programexit()