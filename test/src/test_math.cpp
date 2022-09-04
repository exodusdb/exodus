#undef NDEBUG //because we are using assert to check actual operations that cannot be skipped in release mode testing
#include <cassert>
#include <cmath>

#include <exodus/program.h>
programinit()

function main() {
	printl("test_math says 'Hello World!'");

	gosub test_basic_ops();
	gosub test_mod();

	printl("Test passed");

	return 0;
}

subroutine test_mod() {

//a	b	exodus	arev	remainder
var moddata = R"(
-2	-2	0	0	0
-2	-1.5	-0.5	-0.5	-0.5
-2	-1	0	0	0
-2	-0.5	0	0	0
-2	0.5	0	0	0
-2	1	0	0	0
-2	1.5	1	1	-0.5
-2	2	0	0	0
-1.5	-2	-1.5	-1.5	0.5
-1.5	-1.5	0	0	0
-1.5	-1	-0.5	-0.5	0.5
-1.5	-0.5	0	0	0
-1.5	0.5	0	0	0
-1.5	1	0.5	0.5	0.5
-1.5	1.5	0	0	0
-1.5	2	0.5	0.5	0.5
-1	-2	-1	-1	-1
-1	-1.5	-1	-1	0.5
-1	-1	0	0	0
-1	-0.5	0	0	0
-1	0.5	0	0	0
-1	1	0	0	0
-1	1.5	0.5	0.5	0.5
-1	2	-1	1	-1
-0.5	-2	-0.5	-0.5	-0.5
-0.5	-1.5	-0.5	-0.5	-0.5
-0.5	-1	-0.5	-0.5	-0.5
-0.5	-0.5	0	0	0
-0.5	0.5	0	0	0
-0.5	1	0.5	0.5	-0.5
-0.5	1.5	1	1	-0.5
-0.5	2	1.5	1.5	-0.5
0	-2	0	0	0
0	-1.5	0	0	0
0	-1	0	0	0
0	-0.5	0	0	0
0	0.5	0	0	0
0	1	0	0	0
0	1.5	0	0	0
0	2	0	0	0
0.5	-2	-1.5	-1.5	0.5
0.5	-1.5	-1	-1	0.5
0.5	-1	-0.5	-0.5	0.5
0.5	-0.5	0	0	0
0.5	0.5	0	0	0
0.5	1	0.5	0.5	0.5
0.5	1.5	0.5	0.5	0.5
0.5	2	0.5	0.5	0.5
1	-2	1	-1	1
1	-1.5	-0.5	-0.5	-0.5
1	-1	0	0	0
1	-0.5	0	0	0
1	0.5	0	0	0
1	1	0	0	0
1	1.5	1	1	-0.5
1	2	1	1	1
1.5	-2	-0.5	-0.5	-0.5
1.5	-1.5	0	0	0
1.5	-1	-0.5	-0.5	-0.5
1.5	-0.5	0	0	0
1.5	0.5	0	0	0
1.5	1	0.5	0.5	-0.5
1.5	1.5	0	0	0
1.5	2	1.5	1.5	-0.5
2	-2	0	0	0
2	-1.5	-1	-1	0.5
2	-1	0	0	0
2	-0.5	0	0	0
2	0.5	0	0	0
2	1	0	0	0
2	1.5	0.5	0.5	0.5
2	2	0	0	0)";

	converter(moddata,"\n\r\t",FM^FM^VM);

//PICKOS modulo limits the value to
// [0 , limit) if limit is positive
// (limit, 0] if limit is positive
// unlike c++ which is acts more like a divisor/remainder function

#define USE_PICKOS_MODULUS
#ifdef USE_PICKOS_MODULUS
#define TARGET_COLN 4
#else
#define TARGET_COLN 3
#endif
	for (var line: moddata) {

		if (not line)
			continue;

		printl("mod", line);

		var a = line.a(1,1);
		var b = line.a(1,2);
		var target = line.a(1,TARGET_COLN);

		// var % var
		assert((a % b) == target);
		assert(mod(a, b) == target);

		// test symmetry around zero of limit
		assert((-a % -b) == -target);
		assert(mod(-a, -b) == -target);

		if (index(b,".")) {
			// var % double
			assert((a % double(b)) == target);
			assert(mod(a, double(b)) == target);

			//self assign
			{
				var c = a;
				c %= double(b);
				assert(c == target);
			}

			// var % double symmetry
			assert((-a % -double(b)) == -target);
			assert(mod(-a, -double(b)) == -target);

			//self assign symmetry
			{
				var c = -a;
				c %= double(-b);
				assert(c == -target);
			}

		}
		else {
			// var % int
			assert((a % int(b)) == target);
			assert(mod(a, int(b)) == target);

			//self assign
			{
				var c = a;
				c %= int(b);
				assert(c == target);
			}

			// var % int symmetry
			assert((-a % -int(b)) == -target);
			assert(mod(-a, -int(b)) == -target);

			//self assign symmetry
			{
				var c = -a;
				c %= int(-b);
				assert(c == -target);
			}

		}

		assert(std::remainder(line.a(1,1).toDouble(),line.a(1,2).toDouble()) == line.a(1,5));

	}

	return;
}

subroutine test_basic_ops() {

	//test all combinations of doubles and integers
	test("102.5","5.7");
	test("102"  ,"5.7");
	test("102.5","5");
	test("102"  ,"5");
	return ;
}

subroutine test(in as, in bs) {

	//test all combinations of negatives
	test2(as       ,       bs);
	test2("-" ^ as ,       bs);
	test2(as       , "-" ^ bs);
	test2("-" ^ as , "-" ^ bs);

	return;
}

subroutine test2(in as, in bs) {

	var z;
	var ai=as;
	var bi=bs;
	printl(as,ai.toInt(),round(as,0));
	//debug();
	printl(bs,bi.toInt(),round(bs,0));
	//assert(ai.toInt() == round(as,0));
	//assert(bi.toInt() == round(bs,0));
	ai.toInt();
	bi.toInt();

	var va,vb,vc;

	//plus
	va=ai;
	vb=bi;
	vc=va+vb;
	z=as+bs;
	printl(va,"+",vb,"=",vc,"add should be",z, vc == z ? "ok" : "FAIL");
	assert(vc == z);

	//multiply
	va=ai;
	vb=bi;
	vc=va*vb;
	z=as*bs;
	printl(va,"*",vb,"=",vc,"mul should be",z, vc == z ? "ok" : "FAIL");
	assert(vc == z);

	//minus
	va=ai;
	vb=bi;
	vc=va-vb;
	z=as-bs;
	printl(va,"-",vb,"=",vc,"sub should be",z, vc == z ? "ok" : "FAIL");
	assert(vc == z);

	//divide
	va=ai;
	vb=bi;
	vc=va/vb;
	z=as/bs;
	printl(va,"/",vb,"=",vc,"div be",z, vc == z ? "ok" : "FAIL");
	assert(vc == z);

	//modulus
	va=ai;
	vb=bi;
	vc=va%vb;
	z=as%bs;
	printl(va,"%",vb,"=",vc,"mod be",z, vc == z ? "ok" : "FAIL");
	assert(vc == z);

	{
		//check cannot divide var decimal y var decimal 0
		try {
			var zero = 0.0;
			var x = 100.0;
			x /= zero;
			assert(false);
		} catch(MVDivideByZero){}
	}
	{
		//check cannot divide var decimal by var int 0
		try {
			var zero = 0;
			var x = 100.0;
			x /= zero;
			assert(false);
		} catch(MVDivideByZero){}
	}

	{
		//check cannot divide var int by var decimal 0
		try {
			var zero = 0.0;
			var x = 100;
			x /= zero;
			assert(false);
		} catch(MVDivideByZero){}
	}
	{
		//check cannot divide var int by var int 0
		try {
			var zero = 0;
			var x = 100;
			x /= zero;
			assert(false);
		} catch(MVDivideByZero){}
	}
	{
		//Check rhs conversion from string;
		var x = 100;
		x += var("1.12");
		x -= var("1.12");
		x *= var("1.12");
		x /= var("1.12");
		x %= var("1.12");
	}
	{
		//Check lhs conversion from string;
		var x = 100;
		x="1.23"; x += var("1.12");assert(x eq 1.23 + 1.12);
		x="1.23"; x -= var("1.12");assert(x eq 1.23 - 1.12);
		x="1.23"; x *= var("1.12");assert(x eq 1.23 * 1.12);
		x="1.23"; x /= var("1.12");assert(x eq 1.23 / 1.12);
		x="1.23"; x %= var("1.12");assert(x eq fmod(1.23, 1.12));

		x="1.23"; x += 1.12;assert(x eq 1.23 + 1.12);
		x="1.23"; x -= 1.12;assert(x eq 1.23 - 1.12);
		x="1.23"; x *= 1.12;assert(x eq 1.23 * 1.12);
		x="1.23"; x /= 1.12;assert(x eq 1.23 / 1.12);
		x="1.23"; x %= 1.12;assert(x eq fmod(1.23, 1.12));

		x="1.23"; x += 112;assert(x eq 1.23 + 112);
		x="1.23"; x -= 112;assert(x eq 1.23 - 112);
		x="1.23"; x *= 112;assert(x eq 1.23 * 112);
		x="1.23"; x /= 112;assert(x eq 1.23 / 112);
		x="1.23"; x %= 112;assert(x eq fmod(1.23, 112));
	}
	{

		//math.h seems to have been included in one of the boost or other special headers
		//in this main.cpp file and that causes confusion between math.h and exodus.h sin() and other functions.
		//we resolved the issue here by being specific about the namespace

		//trig on integers
		assert( exodus::sin(30).round(8) eq 0.5);
		assert( exodus::cos(60).round(8) eq 0.5);
		assert( exodus::tan(45).round(8) eq 1);
		assert( exodus::atan(1).round(6) eq 45);
		//trig on doubles
		assert( exodus::sin(30.0).round(8) eq 0.5);
		assert( exodus::cos(60.0).round(8) eq 0.5);
		assert( exodus::tan(45.0).round(8) eq 1);
		assert( exodus::atan(1.0).round(6) eq 45);

		// abs positives
		assert( exodus::abs(0)    eq 0);
		assert( exodus::abs(30)   eq 30);
		assert( exodus::abs(30.00) eq 30);
		assert( exodus::abs(30.10) eq 30.1);
		assert( exodus::abs(30.90) eq 30.9);
		//abs negatives
		assert( exodus::abs(-0)    eq 0);
		assert( exodus::abs(-30)   eq 30);
		assert( exodus::abs(-30.00) eq 30);
		assert( exodus::abs(-30.10) eq 30.1);
		assert( exodus::abs(-30.90) eq 30.9);

		// exponents and roots on integers and doubls
		assert( exodus::exp(1).round(9) eq 2.718281828);
		assert( exodus::exp(1.0).round(9) eq 2.718281828);
		assert( exodus::loge(1) eq 0);
		assert( exodus::loge(2.718281828).round(9) eq 1);
		assert( exodus::loge(10).round(9) eq 2.302585093);
		assert( exodus::sqrt(100) eq 10);
		assert( exodus::sqrt(100.0) eq 10);

		var dividend=100;
		TRACE(mod(dividend,30));

		assert(mod(-4,3)==2);
		assert(mod(-3,3)==0);
		assert(mod(-2,3)==1);
		assert(mod(-1,3)==2);
		assert(mod(-0,3)==0);
		assert(mod(0,3)==0);
		assert(mod(1,3)==1);
		assert(mod(2,3)==2);
		assert(mod(3,3)==0);
		assert(mod(4,3)==1);

		assert(mod(-4.0,3)==2);
		assert(mod(-3.0,3)==0);
		assert(mod(-2.0,3)==1);
		assert(mod(-1.0,3)==2);
		assert(mod(-0.0,3)==0);
		assert(mod(0.0,3)==0);
		assert(mod(1.0,3)==1);
		assert(mod(2.0,3)==2);
		assert(mod(3.0,3)==0);
		assert(mod(4.0,3)==1);

		assert(mod(-4.0,3.0)==2);
		assert(mod(-3.0,3.0)==0);
		assert(mod(-2.0,3.0)==1);
		assert(mod(-1.0,3.0)==2);
		assert(mod(-0.0,3.0)==0);
		assert(mod(0.0,3.0)==0);
		assert(mod(1.0,3.0)==1);
		assert(mod(2.0,3.0)==2);
		assert(mod(3.0,3.0)==0);
		assert(mod(4.0,3.0)==1);

		assert(mod(-4,3.0)==2);
		assert(mod(-3,3.0)==0);
		assert(mod(-2,3.0)==1);
		assert(mod(-1,3.0)==2);
		assert(mod(-0,3.0)==0);
		assert(mod(0,3.0)==0);
		assert(mod(1,3.0)==1);
		assert(mod(2,3.0)==2);
		assert(mod(3,3.0)==0);
		assert(mod(4,3.0)==1);

		//negative dividend
	/*
		assert(mod(-4,-3)==-1);
		assert(mod(-3,-3)==0);
		assert(mod(-2,-3)==-2);
		assert(mod(-1,-3)==-1);
		assert(mod(-0,-3)==0);
		assert(mod(0,-3)==0);
		TRACE(mod(1,-3));
		assert(mod(1,-3)==-2);
		assert(mod(2,-3)==-1);
		assert(mod(3,-3)==0);
		assert(mod(4,-3)==-2);
	*/
		assert(mod(-4.0,-3.0)==-1);
		assert(mod(-3.0,-3.0)==0);
		assert(mod(-2.0,-3.0)==-2);
		assert(mod(-1.0,-3.0)==-1);
		assert(mod(-0.0,-3.0)==0);
		assert(mod(0.0,-3.0)==0);
		TRACE(mod(1.0,-3.0));
		TRACE(fmod(1.0,-3.0));
		TRACE(1 % -3)
		TRACE(-1 % 3)
		assert(mod(1.0,-3.0)==-2);
		assert(mod(2.0,-3.0)==-1);
		assert(mod(3.0,-3.0)==0);
		assert(mod(4.0,-3.0)==-2);

		//check floating point modulo
		TRACE(mod(2.3,var(1.499)).round(3));
		assert(mod(2.3,var(1.499)).round(3) eq 0.801);
		TRACE(mod(-2.3,var(-1.499)).round(3));
		assert(mod(-2.3,var(-1.499)).round(3) eq -0.801);
		TRACE(mod(-2.3,var(1.499)).round(3));
		assert(mod(-2.3,var(1.499)).round(3) eq 0.698);
		TRACE(mod(2.3,var(-1.499)).round(3));
		assert(mod(2.3,var(-1.499)).round(3) eq -0.698);

	}

	// pickos in is actually the floor function

	assert(integer(-1.9) eq -2);
	assert(integer(-1.5) eq -2);
	assert(integer(-1.1) eq -2);

	assert(integer(0) eq 0);

	assert(integer(1.9) eq 1);
	assert(integer(1.5) eq 1);
	assert(integer(1.1) eq 1);

	return;
}

programexit()

