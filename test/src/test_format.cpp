#undef NDEBUG  //because we are using assert to check actual operations that cannot be skipped in release mode testing
#include <cassert>

#include <exodus/program.h>

programinit()

function main() {

#ifdef EXO_FORMAT

	{
		// Various exodus conversions using the {::XXXXXX} pattern

		assert(format("{::MD20}", var(123.456)).outputl() == "123.46");
		assert(format("{::MD20}", var(123456)).outputl() == "123456.00");
		// P has no effect if both number of decimals (2) and number to move (0) are specified
		assert(format("{::MD20P}", var(123.456)).outputl() == "123.46");
		assert(format("{::MD20P}", var(123456)).outputl() == "123456.00");

		// P = Preserve decimal point position

		// A single 2 means MOVE the decimal place left by two AND show 2 decimal places
		assert(format("{::MD2}", var(123.456)).outputl() == "1.23");
		assert(format("{::MD2}", var(123456)).outputl() == "1234.56");

		// P means "if a decimal point is found then do not move the decimal point"
		// otherwise a single 2 means MOVE the decimal point left by 2 AND use use two decimal places
		assert(format("{::MD2P}", var(123.456)).outputl() == "123.46");
		assert(format("{::MD2P}", var(123456)).outputl() == "123456.00");


		assert(format("{::MTS}", var(60*60*12 + 1)) == "12:00:01");

		assert(format("{::D4/E}", var(20000)) == "03/10/2022");
	}

	assert(format("{:9.1f}", 123.456) == "    123.5");
	assert(format("{:9.1f}", -123.456) == "   -123.5");
//	assert(std::format("{:9.1f}", var(123.456)) == "    123.5");
	assert(format("{:9.1f}", var(123.456)) == "    123.5");
	assert(format("{:9.1f}", var(-123.456)) == "   -123.5");
	assert(format("{:9d}", var(1234.56)) == "     1234");
	assert(format("{:9d}", var(-1234.56)) == "    -1234");
	//assert(format("{0}", var("\u03a3")) == "Σ");
	//assert(format("{0}", var("Σ")) == "Σ");
//stop();

	assert(format("{}",1000.0001) == "1000.0001");
	assert(format("{}",1000.00001) == "1000.00001");
	assert(format("{}",1000.000001) == "1000.000001");
	assert(format("{}",1000.0000001) == "1000.0000001");
	assert(format("{}",1000.00000001) == "1000.00000001");
	assert(format("{}",1000.000000001) == "1000.000000001");
	assert(format("{}",1000.0000000001) == "1000.0000000001");
	assert(format("{}",1000.00000000001) == "1000.00000000001");
	assert(format("{}",1000.000000000001) == "1000.000000000001");
	assert(format("{}",1000.0000000000001) == "1000.0000000000001");
	assert(format("{}",1000.00000000000001) == "1000");
	assert(format("{}",1000.000000000000001) == "1000");
	assert(format("{}",1000.0000000000000001) == "1000");

	assert(format("{}",var(1000.0001)) == "1000.0001");
	assert(format("{}",var(1000.00001)) == "1000.00001");
	assert(format("{}",var(1000.000001)) == "1000.000001");
	assert(format("{}",var(1000.0000001)) == "1000.0000001");
	assert(format("{}",var(1000.00000001)) == "1000.00000001");
	assert(format("{}",var(1000.000000001)) == "1000.000000001");
	assert(format("{}",var(1000.0000000001)) == "1000.0000000001");
	assert(format("{}",var(1000.00000000001)) == "1000.00000000001");
	assert(format("{}",var(1000.000000000001)) == "1000.000000000001");
	assert(format("{}",var(1000.0000000000001)) == "1000");
	assert(format("{}",var(1000.00000000000001)) == "1000");
	assert(format("{}",var(1000.000000000000001)) == "1000");
	assert(format("{}",var(1000.0000000000000001)) == "1000");

	assert(format("{:b}", var(256)) == "100000000");
	assert(format("{:B}", var(256)) == "100000000");
	assert(format("{:o}", var(256)) == "400");
	assert(format("{:x}", var(256)) == "100");
	assert(format("{:X}", var(256)) == "100");

	// CAT FACE
	// Unicode character 0x1F431
	// four bytes of utf-8
	// double width character on output
	assert(var("🐱").oconv("HEX").outputl("catface= hex ") == "F09F90B1");
	assert(var("🐱") == "\xF0\x9F\x90\xB1");
	assert(var("🐱") == "\U0001F431");

#if EXO_FORMAT == 1
	// Incorrect in g++13.2
	// Width is calculated as bytes not utf-8 character widths
	// Cat = four bytes so takes up "4" positions in formatting
	assert(format("{:.^5s}", var("🐱")) == "🐱.");

#elif EXO_FORMAT == 2
	// Correct in fmt library
	assert(format("{:.^5s}", "🐱") == ".🐱..");
	assert(format("{:.5s}", "🐱🐱🐱") == "🐱🐱🐱");
	assert(format("{:.<5.5s}", "🐱🐱🐱") == "🐱🐱🐱");

	assert(format("{:^5s}", "🐱") == " 🐱  ");
	assert(format("{:.5s}", "🐱🐱🐱") == "🐱🐱🐱");
	assert(format("{:<5.5s}", "🐱🐱🐱") == "🐱🐱🐱");
#endif

	var pi = 3.14f;
	assert(format("{:10f}", pi) == "  3.140000");
	assert(format("{:.5f}", pi) == "3.14000");
	assert(format("{:10.5f}", pi) == "   3.14000");

	assert(format("a {} b {} c", 100, 200) == "a 100 b 200 c");
	assert(format("a {0} b {1} c", var("x"), var("y")) == "a x b y c");
	TRACE(format("{:9.1f}", 123.456))
	TRACE(format("{:9.1f}", var(123.456)))
	TRACE(format("{}", var(123.456)))
	TRACE(format("{}", var(12345)))
	TRACE(format("{}", var("abcdefg")))

#endif

	printl(elapsedtimetext());
	printl("Test passed");

	return 0;
}
programexit()
