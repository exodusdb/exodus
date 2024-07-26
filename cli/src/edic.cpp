#include <exodus/program.h>
programinit()

	let verbose = OPTIONS.contains("V");
	let exo_HOME = osgetenv("EXO_HOME") ?: osgetenv("HOME");
	let inc_dir = exo_HOME ^ OSSLASH ^ "inc" ^ OSSLASH;

function main() {

	// edic history dir/file
	let edic_hist_dir = osgetenv("HOME") ^ "/.config/exodus/";
	let edic_hist = edic_hist_dir ^ "edic_hist.txt";

	// Default to previous edit/compile - similar code in edic and compile
	if (fcount(COMMAND, FM) < 2) {
		if (osread(COMMAND from edic_hist)) {
			OPTIONS = COMMAND.f(2);
			COMMAND = raise(COMMAND.f(1));
		} else {
			abort("Syntax is 'edic osfilename'");
		}
	}

	// Save edic history
	//if (not osdir(edic_hist_dir))
	//	osmkdir(edic_hist_dir);
	if (not osdir(edic_hist_dir)) {
		if (not osmkdir(edic_hist_dir)) {
			abort(lasterror());
		}
	}
	if (not COMMAND.f(2).starts(OSSLASH))
		COMMAND(2) = oscwd() ^ OSSLASH ^ COMMAND.f(2);
	if (not oswrite(lower(COMMAND) ^ FM ^ OPTIONS on edic_hist))
		printl("Cannot write to ", edic_hist);

	// Look for installed nano or other editor
	var editor;
	var linenopattern;
	gosub geteditor(editor, linenopattern);

////////////
// initfile:
////////////

	let filenames = field(COMMAND, FM, 2, 99999);
	let nfiles = fcount(filenames, FM);

////////////
// nextfile:
////////////

	var filen = 0;
	var startatlineno = 0;
	while (filen < nfiles) {
		filen += 1;
		var filename = filenames.f(filen).unquote();

		if (not filename.len())
			continue;

		if (filename.starts("+") and filename.cut(1).isnum()) {
			startatlineno = filename.cut(1);
			continue;
		}

////////////
// fileinit:
////////////

		// Split out trailing line number after :
		// common compiler/linter error reporting like 'somefile.cpp:21:33'
		startatlineno = field(filename, ":", 2);
		if (startatlineno.isnum())
			filename = field(filename, ":", 1);
		else
			startatlineno = "";

		filename.trimmerlast(".");
		if (not field2(filename, OSSLASH, -1).contains("."))
			filename ^= ".cpp";

		// Search paths and convert to absolute filename
		// SIMILAR CODE IN EDIC and COMPILE
		if (not(osfile(filename)) and not(filename.contains(OSSLASH))) {
			let paths = osgetenv("CPLUS_INCLUDE_PATH").convert(";", ":");
			if (verbose)
				paths.outputl("paths=");
			for (let path : paths.convert(":", FM)) {
				let filename2 = path ^ "/" ^ filename;
				if (verbose)
					filename2.outputl("osfilename=");
				if (osfile(filename2)) {
					filename = filename2;
					break;
				}
			}
		}

		//also look in ~/inc for backlinks to source
		//SIMILAR CODE IN EDIC and COMPILE
		if (not(osfile(filename)) and not(filename.contains(OSSLASH))) {
			var headerfilename = inc_dir ^ filename;

//			// Try .h files for library subroutines first
//			headerfilename.fieldstorer(".", -1, 1, "h");
//			if (verbose)
//				headerfilename.outputl("headerfile=");
//
//			// If no .h file, try .H header files for programs
//			if (!osfile(headerfilename)) {
//				headerfilename.fieldstorer(".", -1, 1, "H");
//				if (verbose)
//					headerfilename.outputl("headerfile=");
//				if (!osfile(headerfilename)) {
//					headerfilename.fieldstorer(".", -1, 1, "hpp");
//					if (verbose)
//						headerfilename.outputl("headerfile=");
//				}
//			}
			// Look for .H .hpp etc.
			headerfilename.fieldstorer(".", -1, 1, "*");
			if (verbose)
				headerfilename.outputl("headerfile=");
			headerfilename=inc_dir ^ oslistf(headerfilename).f(1);
			if (verbose)
				headerfilename.outputl("headerfile=");

			if (osfile(headerfilename)) {
				let headerline1 = osread(headerfilename).field("\n", 1), field("\r", 1);
				if (verbose)
					headerline1.outputl("headerline2=");
				//generated by exodus "compile /root/neosys/src/gen/addcent.cpp"
				let filename2 = headerline1.field2(" ", -1).field("\"", 1);
				if (osfile(filename2))
					filename = filename2;
			}
		}

		// Make path absolute in case EDITOR changes current working directory
		var editcmd = editor;
		if (editcmd.contains("$ABSOLUTEFILENAME")) {
			editcmd.replacer("$ABSOLUTEFILENAME", "$FILENAME");

			filename = oscwd() ^ OSSLASH ^ filename;
		}

		// If file doesnt exist and filename has a compilable extension
		// Prepare a skeleton exodus cpp file
		var newfile = false;
		if (not osfile(filename)) {

			var basefilename = field2(filename, OSSLASH, -1);
			basefilename = basefilename.field(".", fcount(basefilename, ".") - 1);

			var progtype;
			var question = "1=Command line program, 2=Callable function or subroutine";
			question ^= "\n" ^ basefilename.quote() ^ " does not exist. Create what? (1-2) ";
			while (true) {

				if (basefilename.lcase().starts("dict."))
					progtype = 5;
				else
					progtype.input(question);

				if (progtype == 2)      progtype = "classlib";
				else if (progtype == 3)	progtype = "main";
				else if (progtype == 4)	progtype = "mainlib";
				else if (progtype == 1) progtype = "class";
				else if (progtype == 5)	progtype = "dict";
				else
					abort("");
				break;
			}

			newfile = true;
			var blankfile = "";
			if (progtype == "main" or progtype == "mainlib") {
				startatlineno = "4,9";
				blankfile ^= "#include <exodus/exodus.h>\n";
				blankfile ^= "\n";
				blankfile ^= "program() {\n";
				blankfile ^= "\tprintl(\"" ^ basefilename ^ " says 'Hello World!'\");\n";
				if (progtype == "mainlib")
					blankfile ^= "\treturn 0;\n";
				blankfile ^= "}\n";
				if (progtype == "mainlib")
					blankfile.replacer("program()", "function " ^ basefilename ^ "()");
			} else if (progtype == "class" or progtype == "classlib") {
				startatlineno = "6,9";
				blankfile ^= "#include <exodus/program.h>\n";
				//programinit() as 2nd line to avoid ppl in external functions before programinit
				//blankfile^="\n";
				blankfile ^= "programinit()\n";
				blankfile ^= "\n";
				blankfile ^= "function main(";
				//the .h maker not able to parse this yet and is rather clumsy anyway
				//if (progtype == "classlib")
				//	blankfile^="/*in arg1, out arg2*/";
				blankfile ^= ") {\n";
				blankfile ^= "\tprintl(\"" ^ basefilename ^ " says 'Hello World!'\");\n";
				blankfile ^= "\treturn 0;\n";
				blankfile ^= "}\n";
				blankfile ^= "\nprogramexit()";
				blankfile ^= "\n";

				if (progtype == "classlib")
					blankfile.replacer("program", "library");
			} else if (progtype == "dict") {
				startatlineno = "6,9";
				blankfile ^= "#include <exodus/dict.h>\n\n";
				//programinit() as 2nd line to avoid ppl in external functions before programinit
				//blankfile^="\n";
				blankfile ^= "dict(EXAMPLEDICTID1) {\n";
				blankfile ^= "\tANS=RECORD(1)^\"x\";\n";
				blankfile ^= "}\n\n";

				blankfile ^= "dict(EXAMPLEDICTID2) {\n";
				blankfile ^= "\tANS=RECORD(2)^\"x\";\n";
				blankfile ^= "}\n";
			}

			//ensure ends in eol
			if (not blankfile.ends("\n"))
				blankfile ^= "\n";

			//convert to DOS format on Windows
			if (OSSLASH_IS_BACKSLASH)
				blankfile.replacer("\n", _EOL);

			if (not oswrite(blankfile, filename))
				abort("Cannot create " ^ filename ^ ". Invalid file name, or no rights here.");
			//      startatlineno="4,9";
			//startatlineno="";
		}

// initedit:
////////////

		let editcmd0 = editcmd;
		let linenopattern0 = linenopattern;

////////////
// nextedit:
////////////

		//keep editing and compiling until no errors
		while (true) {

// editinit:
////////////
			editcmd = editcmd0;
			linenopattern = linenopattern0;

			//record the current file update timestamp
			let fileinfo = osfile(filename);

			//build the edit command
			if (editcmd.contains("$LINENO")) {
				if (not startatlineno)
					linenopattern = "";
				else
					linenopattern = "+" ^ startatlineno;
				editcmd.replacer("+$LINENO", linenopattern);
			}

			if (editcmd.contains("$FILENAME"))
				editcmd.replacer("$FILENAME", filename);
			else
				editcmd ^= " " ^ filename;

			///////
			// Edit
			///////
			if (verbose)
				printl(editcmd);
			//osshell(editcmd);
			if (not osshell(editcmd)) {
				abort(lasterror());
			}

// editexit:
////////////

			//if the file hasnt been updated
			let fileinfo2 = osfile(filename);
			if (fileinfo2 != fileinfo)
				newfile = false;
			else {
				//delete the skeleton
				printl("File unchanged. Not saving.");
				//if (newfile)
				//	osremove(filename);
				if (newfile) {
					if (osfile(filename) and not osremove(filename)) {
						abort(lasterror());
					}
				}
				//move to the next file
				break;
			}

			//clear the screen (should be cls on win)
			if (OSSLASH == "/")
				//osshell("clear");
				if (not osshell("clear")) {
					abort(lasterror());
				}
			//else
			//	osshell("cls");

			//build the compiler command
			let compiler = "compile";
			let compileoptions = "{C}";
			var compilecmd = compiler ^ " " ^ filename.quote() ^ " " ^ compileoptions;

			//capture the output
			var compileoutputfilename = filename;
			compileoutputfilename ^= ".~";
			//var compileoutputfilename=filename ^ ".2";
			if (OSSLASH == "/") {
				compilecmd ^= " 2>&1";
				compilecmd ^= " | tee " ^ compileoutputfilename.quote();
				// --RAW-CONTROL-CHARS allows colors from compiler to be shown properly in compiler output
				// --chop-long-lines prevents long lines wrapping and forces scrolling right to see them
				// --quit-if-one-screen passes control immedately back to user shell if all compiler output fits on one page
//				compilecmd ^= " | pager --RAW-CONTROL-CHARS --chop-long-lines --quit-if-one-screen";
				// Remove long line scroll right because it prevents cut and pasting long lines
				compilecmd ^= " | pager --RAW-CONTROL-CHARS --quit-if-one-screen";

			}
			else
			    compilecmd ^= " > " ^ compileoutputfilename.quote() ^ " 2>&1";

			//////////
			// Compile
			//////////
			if (verbose)
				printl(compilecmd);
			//osshell(compilecmd);
			if (not osshell(compilecmd)) {
				abort(lasterror());
			}

			// If any errors then optionally loop back to edit again
			var errors;
			if (osread(errors, compileoutputfilename, "utf8")) {
				//osremove(compileoutputfilename);
				if (osfile(compileoutputfilename) and not osremove(compileoutputfilename)) {
					abort(lasterror());
				}
				if (OSSLASH != "/")
					printx(errors);

				startatlineno = "";
				// gnu style error lines
				// There's a de facto standard format for compiler or linter error messages,
				// which is the same format as grep -n: FILE_NAME:LINE_NUMBER:MESSAGE
				let matches = errors.match("\\w" ^ filename ^ ":(\\d+):(\\d+):[^\n]*error");
				if (matches) {

					startatlineno = matches.f(1, 2);

					// skip column because nano treats tab as one column
					//startatlineno ^= "," ^ matches.f(1,3);

				}
				if (startatlineno) {

					// Ask user if they want to resume edit or quit.
					var reply = "";
					for (;;) {
						reply = "";
						output("Re-edit at line " ^ startatlineno ^ "? (Y/n)");
						osflush();
						reply.inputn(1);
						printl();
						if (reply.convert("YyNn\n", "") == "")
							break;
					}
					if (reply.convert("Yy\n", "") == "")
						continue;

					abort("");
				}

			} // handle compile errors

			// If no errors, break out of edit/compile loop and go to the next file
			//printx(compileoutputfilename);
			break;

		} // next edit/compile

	} // next filen

	return 0;
}

subroutine geteditor(out editor, out linenopattern) {

	editor = osgetenv("VISUAL");
	linenopattern = "$LINENO ";
	if (not editor)
		// get nano on linux possibly
		//editor.osgetenv("EDITOR");
		if (not editor.osgetenv("EDITOR")) {
			//null
		}

	//TODO simplify this editor finding code

	//enable edit at first error for crimson editor (no quotes around filename for cedt)
	if (editor.lcase().contains("cedt") and not editor.contains("$"))
		editor ^= " /L:$LINENO $FILENAME";

	var nanopath = "";
	if (OSSLASH_IS_BACKSLASH) {

		//look for nano.exe next to edic.exe
		if (not editor)
			nanopath = EXECPATH.replace("edic", "nano");
		if (nanopath.osfile())
			editor = "nano $LINENO'$FILENAME'";
	}

	//look for nano in parent bin
	if (not editor) {
		nanopath = "..\\bin\\nano.exe";
		if (nanopath.osfile())
			editor = "nano $LINENO'$FILENAME'";
	}

	//look for nano in release directory during exodus development
	//if (not editor) {
	if (not editor and OSSLASH_IS_BACKSLASH) {
		nanopath = "..\\..\\release\\cygwin\\bin\\nano.exe";
		if (nanopath.osfile())
			editor = "..\\..\\release\\cygwin\\bin\\nano $LINENO'$FILENAME'";
		else {
			nanopath = "..\\" ^ nanopath;
			if (nanopath.osfile()) {
				editor = "..\\..\\..\\release\\cygwin\\bin\\nano $LINENO'$FILENAME'";
			}
		}
	}

	if (editor.contains("nano"))
		linenopattern = "+$LINENO ";

	//otherwise on windows try to locate CYGWIN nano or vi
	var cygwinpath = "";
	if (not editor and OSSLASH == "\\") {
		//from environment variable
		cygwinpath = osgetenv("CYGWIN_BIN");
		//else from current disk
		if (not cygwinpath)
			cygwinpath = "\\cygwin\\bin\\";
		//else from c:
		if (not osdir(cygwinpath))
			cygwinpath = "c:\\cygwin\\bin\\";
		//else give up
		if (not osdir(cygwinpath))
			cygwinpath = "";

		if (cygwinpath and not cygwinpath.ends(OSSLASH))
			cygwinpath ^= OSSLASH;
		//editor=cygwinpath^"bash --login -i -c \"/bin/";
		editor = cygwinpath;
		if (osfile(cygwinpath ^ "nano.exe") or osfile("nano.exe")) {
			editor = "nano $LINENO'$FILENAME'";
			if (osfile(cygwinpath ^ "nano.exe"))
				editor.prefixer(cygwinpath);
			//editor^="\"";
			linenopattern = "+$LINENO ";
		} else if (osfile(cygwinpath ^ "vi.exe") or osfile("vi.exe")) {
			editor = "vi -c \":$LINENO\" $FILENAME";
			if (osfile(cygwinpath ^ "vi.exe"))
				editor.prefixer(cygwinpath);
			//editor^="\"";
		} else
			editor = "";
	}
	if (OSSLASH == "\\") {
		//configure nanorc (on windows)
		//TODO same for non-windows
		//nano on windows looks for nanorc config file as follows (probably merges all found)
		//C:\cygwin\usr\local\etc\nanorc
		//C:\cygwin\etc\nanorc (only if cygwin exists)
		//C:\Documents and Settings\USERNAME\.nanorc  ($HOMEDRIVE$HOMEPATH)
		var nanorcfilename;
		if (cygwinpath) {
			nanorcfilename = cygwinpath.field(OSSLASH, 1, fcount(cygwinpath, OSSLASH) - 2) ^ OSSLASH ^ "etc" ^ OSSLASH ^ "nanorc";
		} else {
			nanorcfilename = osgetenv("HOME");
			if (not nanorcfilename)
				nanorcfilename = osgetenv("HOMEDRIVE") ^ osgetenv("HOMEPATH");
			if (not nanorcfilename.ends(OSSLASH))
				nanorcfilename ^= OSSLASH;
			nanorcfilename ^= ".nanorc";
		}
		if (not osfile(nanorcfilename)) {
			//var nanorctemplatefilename=EXECPATH.field(OSSLASH,1,fcount(EXECPATH,OSSLASH)-1) ^ OSSLASH ^ "nanorc";
			var nanorctemplatefilename = nanopath.field(OSSLASH, 1, fcount(nanopath, OSSLASH) - 1) ^ OSSLASH ^ "nanorc";
			if (not osfile(nanorctemplatefilename))
				nanorctemplatefilename.replacer("release", "..\\release");
			if (oscopy(nanorctemplatefilename, nanorcfilename)) {
				printl("Copied " ^ nanorctemplatefilename.quote() ^ " to " ^ nanorcfilename.quote());
				var().input("Note: nano c++ syntax highlighting has been installed. Press Enter ... ");
			} else {
				errputl("Could not copy " ^ nanorctemplatefilename.quote() ^ " to " ^ nanorcfilename.quote());
				if (not osfile(nanorctemplatefilename))
					errputl("nano syntax highlighting file is missing.");
			}
		}
//		if (not osgetenv("HOME"))
//			ossetenv("HOME", osgetenv("HOMEDRIVE") ^ osgetenv("HOMEPATH"));
	}

	if (not editor) {
		if (OSSLASH == "/")
			editor = "nano ";
		else
			editor = "notepad";
		printl("Environment EDITOR not set. Using " ^ editor);
	}

	//editor="vi";
	editor.replacer(R"(^[^ ]*\bnano\b)"_rex, "nano --positionlog --const +$LINENO");
	//editor.replacer("nano ", "nano --positionlog --const --nowrap --autoindent --suspend --speller=compile +$LINENO ");

	if (editor.contains("nano"))
		printl("http://www.nano-editor.org/dist/v2.1/nano.html");

	return;
}
programexit()
