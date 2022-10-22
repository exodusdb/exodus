#include <exodus/library.h>
libraryinit()

#include <readhostsallow.h>
#include <shell2.h>
#include <whois.h>

#include <system_common.h>

#include <sys_common.h>

	var sentencex;
var ip12;
var ip1;
var ip2;
var allowedips;
var xx;
var cmd;
var exe;
var errors;

function main(in /*mode*/, in ipno, out text) {

	if (SENTENCE.field(" ", 1) eq "WHOIS") {
		SENTENCE.move(sentencex);
		call whois("", sentencex.field(" ", 2), text);
		call mssg(text);
		stop();
	}

	//skip standard LAN or EXODUS ip no and exit - no delay for these ips
	//medialine lan is 100.100.100.* iana "carrier grade NAT"
	//test in order of frequency installed at clients

	ip12 = ipno.field(".", 1, 2);
	if (ip12 eq "192.168") {
		goto returnzero;
	}

	//skip 10.*.*.*
	ip1 = ipno.field(".", 1);
	if (ip1 eq "10") {
		goto returnzero;
	}

	//skip 172.16-32.*.* and 100.64-127.*.*
	ip2 = ipno.field(".", 2);
	if ((ip1 eq 172 and ip2 ge 16) and ip2 le 31) {
		goto returnzero;
	}
	if ((ip1 eq 100 and ip2 ge 64) and ip2 le 127) {
		goto returnzero;
	}

	//skip 127.*.*.*
	if (ip1 eq "127") {
		goto returnzero;
	}

	//skip EXODUS internet ip numbers and those in system configuration file
	//ZZZ should really detect net ranges like /24 and .*
	//allowedips=' ':xlate('GBP','$HOSTS.ALLOW','','X'):' '
	call readhostsallow(allowedips);
	allowedips(-1) = SYSTEM.f(39);
	allowedips.converter(_FM _VM
						 ","
						 "\r\n",
						 var(100).space());
	if (allowedips.locateusing(" ", ipno, xx)) {
returnzero:
		text = "0";
		return 0;
	}

	if (true) {
		text =
			"http:/"
			"/geoiplookup.net/ip/" ^
			ipno;
	} else {
		cmd = SYSTEM.f(50);

		//check cygwin whois present otherwise quit
		exe = oscwd().contains(":") ? ".exe" : "";

		//build the command
		cmd ^= "whois " ^ ipno;

		//dont wait for a long time - in case internet access is blocked
		if ((cmd ^ "timeout.exe").osfile()) {
			cmd.prefixer(SYSTEM.f(50) ^ "timeout 5 ");
		}

		//capture the output of the whois command
		text = shell2(cmd, errors);

		if (errors) {
			text ^= errors;
		}

		text.converter("\r\n", _FM _FM).trimmer(_FM);
	}

	return 0;
}

libraryexit()
