#include <exodus/program.h>
programinit()

	var file1;
	var file2;
	var targetfilename;
	var targetdb;
	var targetname;
	var sql;
	dim sqltext;
	var nlines;
	var ln;
	var sourcefilename;
	var recn;
	var dictonly;

function main() {

	if (not COMMAND.a(2) or not COMMAND.a(3)) {

		var syntax =
		"Syntax is copyfile [<source>:][<sourcefilename>,...] [<targetdb>:[<targetfilename>] {OPTIONS}\n"
		"\n"
		"source can be a database name or an sql file containing COPY data like that produced by pg_dump";

		abort(syntax);
	}

	//parse source
	var sourcename = "";
	var sourcefilenames = COMMAND.a(2);
	if (index(sourcefilenames,":")) {
		sourcename = field(sourcefilenames,":",1);
		sourcefilenames = field(sourcefilenames,":",2);
	}

	//parse target
	targetname = "";
	var targetfilenames = COMMAND.a(3);
	if (index(targetfilenames,":")) {
		targetname = field(targetfilenames,":",1);
		targetfilenames = field(targetfilenames,":",2);
	}

	//detect sql source
	sql = sourcename.ends(".sql");

	//connect to source db if source not .sql file
	var sourcedb;
	if (not sql and not sourcedb.connect(sourcename))
		abort(sourcename.quote() ^ " Cannot connect");

	//connect to target db
	if (not targetdb.connect(targetname))
		abort(targetname.quote() ^ " Cannot connect");

	//dict means filter all and only dict files
	dictonly = sourcefilenames eq "dict";
	if (dictonly and not sql)
		sourcefilenames = "";

	//determins all files in source db if source not .sql
	if (not sql and not sourcefilenames) {
		sourcefilenames = sourcedb.listfiles();
		targetfilenames = "";
	}

	//go through files one by one if source is a db
	if (not sql)
		sourcefilenames.converter(",", FM);

	for (const var& temp : sourcefilenames) {

		sourcefilename = temp;

		//never do dict.all which is an sql view of all dict. files
		if (sourcefilename eq "dict.all")
			continue;

		//filter dict. files
		if (not sql and dictonly and sourcefilename.substr(1,5) ne "dict.")
			continue;

		// source is .sql file
		if (sql) {

			//read in the .sql file text
			targetfilename = "";
			var txt;
			if (not txt.osread(sourcename))
				abort(sourcename.quote() ^ " does not exist or cannot be accessed");

			//split the sql text into an array
			nlines = sqltext.split(txt,"\n");
			ln = 0;

		}

		// source is database
		else {

			targetfilename = targetfilenames;
			if (not targetfilename)
				targetfilename = sourcefilename;

			//open source file
			if (not file1.open(sourcefilename, sourcedb) )
				abort(sourcefilename.quote() ^ " cannot be opened in source db " ^ sourcename);

			//open target file
			if (not file2.open(targetfilename, targetdb)) {
				if (not OPTIONS.index("C") or not targetdb.createfile(targetfilename) or not file2.open(targetfilename, targetdb))
					abort(targetfilename.quote() ^ " cannot be opened in target db " ^ targetname);
			}

		}

		//speed up
		targetdb.begintrans();

		//select source file if source is db
		if (not sql) {
			printl(sourcefilename);
			file1.select(sourcefilename ^ " (R)");
		}

		//process source file
		recn = 0;
		while(getrec()) {

			//user interrupt
			if (not mod(recn,1000)) {
				//print(at(-40), sourcefilename, recn ^ ".", ID);
				if (esctoexit())
					abort("");
			}

			//skip if not changed
			var oldrec;
			if (oldrec.read(file2,ID)) {
				if (RECORD eq oldrec) {
					print("\tNot changed");
					continue;
				}
				printl("\tChanged");
			} else {
				printl("\tNew");
			}

			//if (OPTIONS.index("W"))
				RECORD.write(file2, ID);

		}

		//commit all target db updates
		targetdb.committrans();
		printl();

	}

	return 0;
}

///////////////////
function getrec() {
///////////////////

	// If source is database then simply readnext RECORD and ID
	if (not sql) {

		var result = file1.readnext(RECORD, ID, MV);

		if (result) {
			recn++;
			print(at(-40) ^ recn ^ ".", ID);
		}

		return result;
	}

	//otherwise get the next file and record from the source .sql file
	while (true) {

		ln++;

		if (ln gt nlines)
			return false;

		RECORD = sqltext(ln);

		// If we dont have a filename then find a line starting COPY and extract filename from it
		if (not targetfilename) {

			if (RECORD.field(" ", 1) != "COPY")
				continue;

			targetfilename = RECORD.field(" ", 2);

			// Remove public. from start of filename but not dict.
			if (targetfilename.starts("public."))
				targetfilename = targetfilename.substr(8);

			// Skip unwanted files
			if (not dictonly and sourcefilename and not sourcefilename.locateusing(targetfilename, ","))
				continue;

			// Skip file if not dict. and only dicts wanted
			if (dictonly and not targetfilename.starts("dict.")) {
				targetfilename = "";
				continue;
			}

			// Output filename by itself
			printl();
			printl(targetfilename);
			recn = 0;

			// Open the target file
			if (not file2.open(targetfilename, targetdb) )
				abort(targetfilename.quote() ^ " cannot be opened in target db " ^ targetname);

			continue;

		}

		// A line with just "\." indicates no more records for the current file
		if (RECORD eq "\\.") {

			// Trigger a search for the next line starting COPY
			targetfilename = "";
			continue;
		}

		break;

	}

	recn++;

	ID = RECORD.field("\t",1);
	//RECORD[1,len(ID)+1] = ""
	RECORD.splicer(1,ID.length()+1,"");

	gosub unescape(ID);
	gosub unescape(RECORD);
	//TRACE(ID)

	print(at(-40) ^ recn ^ ".", ID);

	return true;
}

subroutine unescape(io arg1) {

	if (not arg1.index("\\"))
		return;

	arg1.swapper("\\n","\n");
	arg1.swapper("\\b","\b");
	arg1.swapper("\\t","\t");
	arg1.swapper("\\v","\v");
	arg1.swapper("\\f","\f");
	arg1.swapper("\\r","\r");
	arg1.swapper("\\\\","\\");

	return;

}

programexit()
