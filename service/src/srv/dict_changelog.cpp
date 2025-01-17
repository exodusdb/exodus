#include <exodus/library.h>


libraryinit(keywords)
//-------------------
function main() {
	ANS = RECORD.f(2);
	ANS.replacer(VM, ", ");
	ANS.replacer(SM, ", ");
	return ANS;
}
/*pgsql
	ans := exodus.extract_text(DATA, 2, 0, 0);
	ans := replace(ans, VM, ', ');
	ans := replace(ans, SM, ', ');
*/
libraryexit(keywords)


libraryinit(text2)
//----------------
function main() {
	ANS = RECORD.f(3);
	var distrib = RECORD.f(5);
	distrib.converter(SM, ":");
	if (distrib and not(distrib.contains("User"))) {
		ANS.prefixer(distrib.field(":", 1) ^ ": ");
	}
	return ANS;
}
libraryexit(text2)
