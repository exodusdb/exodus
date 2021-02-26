#include <exodus/program.h>
programinit()

function main() {

	logputl("compdbs says 'Hello World!'");

	var timestarted=time();

	var db1=COMMAND.a(2);
	var db2=COMMAND.a(3);

	if (not(db1.connect())) {
		call fsmsg();
		stop();
	}

	if (not(db2.connect())) {
		call fsmsg();
		stop();
	}

	var filename;
	var filen=0;
	while (filename = COMMAND.a(++filen+3)) {
		logput(filename,"");

		var file1;
		if (not file1.open(filename,db1)) {
			call fsmsg();
			stop();
		}

		logputl(file1.reccount());

		var file2;
		if (not file2.open(filename,db2)) {
			call fsmsg();
			stop();
		}

		var keys;
		if (keys.osread("qwerty")) {
			keys.converter("\n",FM);
			file1.makelist("", keys);
		}
		else
			file1.select("SELECT " ^ filename ^ " (SR)");

		var recn=0;
		while (file1.readnext(RECORD, ID, MV)) {

			if (esctoexit())
				stop();

			recn++;
			logput(at(-40) ^ recn ^ ".", ID);
			//printl("",RECORD.length());
			if ((recn % 1000) == 1)
				osflush();

			if (not RECORD) {
				if (not RECORD.read(file1,ID)) {
					printl(ID,"missing from filename",db1.a(1));
					continue;
				}
			}

			var rec2;
			if (not read(rec2, file2, ID)) {
				printl(ID," missing from", db2.a(1), filename);
				continue;
			}

			if (rec2 ne RECORD) {

				//revert both records to leading zero 0.1, -0.1 instead of old arev .1 and -.1;
				RECORD.replacer("([\x1A-\x1F]-?)0.","$1.");
				rec2.replacer("([\x1A-\x1F]-?)0.","$1.");

				//if still different, output
				if (rec2 ne RECORD) {
					printl(ID," different.");
					var nfs=dcount(RECORD,FM);
					var nfs2=dcount(rec2,FM);
					if (nfs2 gt nfs)
						nfs=nfs2;
					for (var fn=1;fn<=nfs;++fn) {
						var f1=RECORD.a(fn);
						var f2=rec2.a(fn);
						if (f1 ne f2) {
							printl(fn^"-",f1);
							printl(fn^"+",f2);
						}
					}
				}
			continue;
			}
		}

	}

	//call stopper("",timestarted,datestarted);

	logputl();
	var seconds = time()-timestarted;
	while (seconds<0) seconds += 86400;
	logputl("Finished in", int(seconds/60), "minutes and", mod(seconds,60), "seconds.");

	return 0;
}

programexit()
