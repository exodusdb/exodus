#include <exodus/library.h>
libraryinit()

function main(in cmd) {
	// if @username='EXODUS' then oswrite cmd on 'XSELECT.'
	// oswrite cmd on 'XSELECT.'
	let supersilent = index(cmd, "(SS");
	if (SYSTEM.f(33) and not supersilent) {
		logputl();
		logput(cmd, ":");
	}
	let started = timestamp();
	select(cmd);
	if (SYSTEM.f(33) and not supersilent) {
		logputl(" done", elapsedtimetext(started, timestamp()));
	}
	return 0;
}

libraryexit()
