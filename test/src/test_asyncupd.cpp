#include <thread>
#include <cassert>

#include <exodus/program.h>
programinit()

	var key = 1;
	var max = 5;
	var nprocesses = 50;

	// Create a temporary file (ending _temp)
	var filename = "xo_test";

	var file;

	var silent = OPTIONS.count("S");

function main() {

	//Pass if no default database connection
	if (not connect()) {
		printl("No default db connection to perform test_asyncupd");
		printl("Test passed");
		return 0;
	}

	// Will FAIL without locking
	var no_locking = OPTIONS.count("L");

	// Will work with and without transactions
	var no_transaction = OPTIONS.count("T");

	if (not TERMINAL)
		silent = 1;

	if (not open(filename to file)) {

		printl("Creating", filename);

		createfile(filename);

		open(filename to file);

		assert(file);

	}
	assert(file);

	PROCESSNO = COMMAND.a(2);

	// Start multiple processes, wait for them to finish, and check result as expected
	// and exit
	if (not PROCESSNO) {

		// Test WITH transactions (ie WITHOUT T option!)
		var ok = test("");

		// Test WITHOUT transactions (i.e. WITH T option!)
		if (ok)
			ok = test("T");

		if (ok)
			printl("Test passed");

		deletefile(filename);

		return not ok;
	}

	// Perform one process updates
	if (silent < 2)
		printl("Starting", PROCESSNO, OPTIONS);

	var n = "";
	while (n < max and not esctoexit()) {

		// Start transaction
		if (not no_transaction)
			assert(begintrans());

		if (no_locking || lock(file, key)) {

			if (not read(RECORD from file, key))
				RECORD = "";

			if (n eq "")
				n = 1;
			else {
				var o = RECORD.a(PROCESSNO);
				assert(o eq n);
				n = o + 1;
			}

			RECORD.r(PROCESSNO, n);

			write(RECORD on file, key);

			if (not no_locking)
				unlock(file,key);

			if (not silent)
				printl(PROCESSNO, RECORD);

			// Commit transaction
			if (not no_transaction)
				assert(committrans());

		} else {
			//printl("Cannot lock", PROCESSNO);
			if (not no_transaction)
				assert(rollbacktrans());
		}

	}

	if (silent < 2)
		printl("Finished", PROCESSNO, OPTIONS);

	return 0;
}

function test(in option) {

		deleterecord(file, key);

		var cmd = "";
		var options = OPTIONS.convert("{}", "") ^ option;
		for (var processn = 1 ; processn <= nprocesses; processn++)
			// Hard code path so make test can find it without any installation
			cmd ^= "./test_asyncupd " ^ processn ^ " {" ^ options ^ "} & ";

		//osshell("test_asyncupd 1 & test_asyncupd 2 & test_asyncupd 3 & test_asyncupd 4 & test_asyncupd 5 &");
		osshell(cmd);
		while (osshellread("pgrep test_asyncupd").count("\n") gt 1) {

			if (not silent)
				printl("Waiting for test_asyncupd processes to complete");

			ossleep(1000);
		}

		if (not read(RECORD from file, key))
			RECORD = "";

		var target = str(max ^ FM, nprocesses).pop();

		if (RECORD ne target) {
			errputl("ERROR: " ^ RECORD.quote() ^ " not equal to target " ^ target.quote());
			return false;
		}

		if (not silent)
			printl("OK: " ^ RECORD.quote() ^ " equal to target");

		return true;

}

programexit()

