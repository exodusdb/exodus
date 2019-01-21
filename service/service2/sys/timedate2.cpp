#include <exodus/library.h>
libraryinit()

#include <getdatetime.h>

#include <gen.h>

var localdate;
var localtime;
var x3;
var x4;
var x5;
var x6;

function main(in localdate0="", in localtime0="") {
	//c sys "",""

	//caserevised*

	//use parameters only if both are provided
	if (localtime0.unassigned()) {
		call getdatetime(localdate, localtime, x3, x4, x5, x6);
	}else{
		localdate = localdate0;
		localtime = localtime0;
	}

	var temp = "";
	if (not temp) {
		temp = "MTH";
	}
	temp = localtime.oconv(temp);
	if (temp[1] == "0") {
		temp.splicer(1, 1, "");
	}
	if (temp.substr(-2,2) == "AM") {
		temp.splicer(-2, 2, gen.glang.a(16));
	}
	if (temp.substr(-2,2) == "PM") {
		temp.splicer(-2, 2, gen.glang.a(17));
	}

	if (DATEFORMAT == "") {
		DATEFORMAT = "D/E";
	}
	temp.splicer(1, 0, localdate.oconv("[DATE,*4]") ^ " ");

	return temp;

}


libraryexit()