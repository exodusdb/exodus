#include <exodus/program.h>
programinit()

/* This program is not in use currently */

/*
template1 is the default database that is used to create all new databases.

Annoyingly we cannot simply do the following
1. add pgexodus functions to template1
2. create user and database

because opening the template1 one database cannot be closed
in time for it to be copied to create the new database
so we do this instead

1. create user and database
2. add pgexodus functions to database
3. add pgexodus functions to template1 database (for all future database creations)
*/

function main() {

	printl("Exodus Copyright (c) 2009 steve.bush@neosys.com");
	printl("Licence: http://www.opensource.org/licenses/mit-license.php");

	//get existing/defaults

	//first from the command
	var origconfig = field(SENTENCE, " ", 2, 999999999);

	//second from environment
	//TODO should be per parameter
	var temp;
	if (temp.osgetenv("EXO_CONN"))
		origconfig ^= " " ^ temp;

	//third from config file
	//should be same logic in mvdbpostgres and configexodus
	var configfilename = "";
	if (OSSLASH == "\\") {
		let userprofile_exodusdir = osgetenv("USERPROFILE") ^ OSSLASH ^ "Exodus";
		if (not osdir(userprofile_exodusdir)) {
			if (not(osmkdir(userprofile_exodusdir)))
				printl("Cannot mkdir ", userprofile_exodusdir, " Maybe you dont have sufficient rights");
		}
		configfilename = userprofile_exodusdir ^ "\\.exodus";
	} else
		configfilename = osgetenv("HOME") ^ OSSLASH ^ ".exodus";
	if (temp.osread(configfilename))
		origconfig ^= " " ^ temp;

	//fourth from hard-coded
	origconfig ^= " host=127.0.0.1 port=5432 dbname=exodus user=exodus password=somesillysecret connect_timeout=10";
	origconfig.converter("\x0d\x0a", "  ");

	/// extract various config options from collected config

	printl("-- Exodus Postgres Configuration ---");

	printl(oconv("Input options and connect to postgres", "L#40"));
	var serverconfig;
	let adminconfig = input_adminconfig(origconfig, serverconfig);
	if (not adminconfig)
		abort("Stopping. Cannot continue without a working connection.");

	var connecttimeout = "10";
	getorigparam(origconfig, "connect_timeout", connecttimeout);

	//default database to create/use
	//for now exodus a default database to play in with a non-secret password
	var dbusername = "exodus";
	var dbname = "exodus";
	var dbuserpass = "somesillysecret";

	getorigparam(origconfig, "dbname", dbname);
	getorigparam(origconfig, "user", dbusername);
	getorigparam(origconfig, "password", dbuserpass);

	getinput("New database code", dbname);
	getinput("New user code", dbusername);
	getinput("New user password", dbuserpass);

	//if (OSSLASH == "\\")
	//	configure_via_connection(adminconfig, dbname, dbusername, dbuserpass);
	//else
	//	configure_via_script(adminconfig, dbname, dbusername, dbuserpass);

	//ask to save configuration
	var saveconfig = "Y";
	while (true) {

		getinput("Save user configuration in .exodus", saveconfig);
		if (not var(saveconfig.first()).ucase().contains("Y"))
			break;

		oconv("Saving user configuration in .exodus", "L#40").output();
		let userconfig = serverconfig ^ " dbname=" ^ dbname ^ " user=" ^ dbusername ^ " password=" ^ dbuserpass;
		if (oswrite(userconfig, configfilename)) {
			printl("done!");
			break;
		}
		printl("Cannot update ", configfilename, " Maybe you dont have sufficient rights");
	}

	printx("Press Enter ... ");
	input();
	return 0;
}

//get the first definition of each parameter
subroutine getorigparam(in config, in name, out value) {
	let pos = index(config, name);
	if (!pos)
		return;
	value = config.b(pos).field(" ", 1).field("=", 2, 999999999);
	return;
}

subroutine getinput(in prompt, io data) {
	var text = prompt ^ " (" ^ data ^ ")";
	if (len(text) < 30)
		text = oconv(text, "L#30");
	printx(text, " ? ");
	let result = input();
	if (result != "")
		data = result;
}

function input_adminconfig(in origconfig, out serverconfig) {

	var host = "127.0.0.1";
	var port = "5432";
	var user = "postgres";
	var adminconfig = "";
	getorigparam(origconfig, "host", host);
	getorigparam(origconfig, "port", port);

	do {

		//var pass="";
		var pass = "";
		var yesno = "Y";

		getinput("Postgres host", host);
		getinput("Postgres port", port);
		getinput("Postgres admin user", user);
		getinput("Postgres admin password", pass);

		adminconfig = "host=" ^ host;
		adminconfig ^= " port=" ^ port;
		adminconfig ^= " user=" ^ user;

		adminconfig ^= " dbname=postgres";

		printl("\n", adminconfig, "\n");
		adminconfig ^= " password=" ^ pass;

		printx(oconv("Attempting to connect ... ", "L#40"));
		if (connect(adminconfig)) {
			printl("done!");
			break;
		}

		getinput("\nCannot connect. Try Again? ", yesno);
		if (yesno.ucase().first() != "Y")
			return false;  //stop("Cancelled.");

	} while (true);

	serverconfig = "host=" ^ host;
	serverconfig ^= " port=" ^ port;

	return adminconfig;
}

function runsomesql(in sql) {

	var errmsg;
	if (not sql.sqlexec(sql, errmsg)) {
		//printl("\nSQL " ^ errmsg ^ sql);
		printl("\nSQL " ^ errmsg);
		return false;
	}

	printl("done!");
	return true;
}

function add_pgexodus_postgres_plugin() {

//	let sql =
//		"CREATE OR REPLACE FUNCTION exodus_call(bytea, bytea, bytea, bytea, bytea, int4, int4) RETURNS bytea AS 'pgexodus', 'exodus_call' LANGUAGE C IMMUTABLE;\n"
//		"CREATE OR REPLACE FUNCTION exodus_extract_bytea(bytea, int4, int4, int4) RETURNS bytea AS 'pgexodus', 'exodus_extract_bytea' LANGUAGE C IMMUTABLE;\n"
//		"CREATE OR REPLACE FUNCTION exodus.extract_text(bytea, int4, int4, int4) RETURNS text AS 'pgexodus', 'exodus_extract_text' LANGUAGE C IMMUTABLE;\n"
//		"CREATE OR REPLACE FUNCTION exodus_extract_sort(bytea, int4, int4, int4) RETURNS text AS 'pgexodus', 'exodus_extract_sort' LANGUAGE C IMMUTABLE;\n"
//		//-- Remaining functions are STRICT therefore never get called with NULLS -- also return NULL if passed zero length strings
//		"CREATE OR REPLACE FUNCTION exodus_extract_text2(bytea, int4, int4, int4) RETURNS text AS 'pgexodus', 'exodus_extract_text2' LANGUAGE C IMMUTABLE STRICT;\n"
//		"CREATE OR REPLACE FUNCTION exodus.extract_date(bytea, int4, int4, int4) RETURNS date AS 'pgexodus', 'exodus_extract_date' LANGUAGE C IMMUTABLE STRICT;\n"
//		"CREATE OR REPLACE FUNCTION exodus.extract_time(bytea, int4, int4, int4) RETURNS time AS 'pgexodus', 'exodus_extract_time' LANGUAGE C IMMUTABLE STRICT;\n"
//		"CREATE OR REPLACE FUNCTION exodus.extract_datetime(bytea, int4, int4, int4) RETURNS timestamp AS 'pgexodus', 'exodus_extract_datetime' LANGUAGE C IMMUTABLE STRICT;";
	let sql = "CREATE EXTENSION IF NOT EXISTS pgexodus;";
	return runsomesql(sql);
}

function create_dbuser(in dbusername, in dbuserpass) {

	var sql =
		"CREATE ROLE $dbusername$ LOGIN"
		" PASSWORD '$dbuserpass$'"
		" CREATEDB"
		//"  CREATEROLE"
		";";

	replacer(sql, "$dbusername$", dbusername);
	replacer(sql, "$dbuserpass$", dbuserpass);

	return runsomesql(sql);
}

function create_db(in dbname, in dbusername) {

	var sql =
		"CREATE DATABASE $dbname$"
		" WITH ENCODING='UTF8'"
		" OWNER=$dbusername$"
		";";

	replacer(sql, "$dbname$", dbname);
	replacer(sql, "$dbusername$", dbusername);

	return runsomesql(sql);
}

function configure_via_connection(in adminconfig, in dbname, in dbusername, in dbuserpass) {
	printx(oconv("Creating new user " ^ dbusername ^ " ... ", "L#40"));
	if (not create_dbuser(dbusername, dbuserpass))
		//stop();
		printl("Error: Could not create user ", dbusername);

	printx(oconv("Creating new database " ^ dbname ^ " ... ", "L#40"));
	if (not create_db(dbname, dbusername))
		//stop();
		printl("Error: Could not create database ", dbname);

	printx(oconv("Detaching from postgres database ... ", "L#40"));
	disconnect();
	printl("done!");

	printx(oconv("Connecting to new database ... ", "L#40"));
	let connstr2 = adminconfig ^ " dbname=" ^ dbname;

	//^" user="^dbusername^" password="^dbuserpass;
	if (not connect(connstr2))
		abort("Stopping. Cannot connect to new database");
	printl("done!");

	if (OSSLASH == "\\") {

		printl(oconv("Installing pgexodus postgres plugin ... ", "L#40"));

		//locate pgexodus dll to install
		var pgexodusdll = "pgexodus.dll";
		if (not osfile(pgexodusdll)) {
			let exodusbinpath = field(EXECPATH, OSSLASH, 1, fcount(EXECPATH, OSSLASH) - 1);
			pgexodusdll = exodusbinpath ^ OSSLASH ^ "pgexodus.dll";
			if (not osfile(pgexodusdll)) {
				printl("Cant find pgexodus.dll or " ^ pgexodusdll);
				pgexodusdll = "";
			}
		}

		//locate target directory (postgresql libdir)
		var pglibdir;
		var pgenvvar = "POSTGRESQL";
		if (PLATFORM.contains("64"))
			pgenvvar ^= "64";
		else
			pgenvvar ^= "32";
		pglibdir = osgetenv(pgenvvar);
		if (pglibdir)
			pglibdir ^= "\\lib";
		else
			pglibdir = osshellread("pg_config --libdir").field("\n", 1);
		if (not pglibdir) {
			pglibdir = "\\PROGRA~1\\PostgreSQL\\9.0";
		}
		pglibdir.converter("/", OSSLASH);
		//exodus oscopy cannot handle old 8.3 file names (boost library issue)
		pglibdir.replacer("PROGRA~1", "Program Files");
		pglibdir.replacer("PROGRA~2", "Program Files (x86)");
		pglibdir.replacer("POSTGR~1", "PostgreSQL");
		let targetfilename = pglibdir ^ OSSLASH ^ "pgexodus.dll";
		while (not osdir(pglibdir)) {
			printl(pglibdir ^ " does not exist");
			printl("Cannot locate PostgreSQL lib directory to install plugin");
			pglibdir = input("PostgreSQL lib dir?");
			if (not pglibdir)
				abort("");
		}

		//delete any existing pgexodus.dll. restart service if cannot initially delete
		if (osfile(targetfilename) and not osremove(targetfilename)) {
			//determine postgres service name
			let pgversion = osshellread("pg_config --version").field(" ", 2).field(".", 1, 2);
			var servicename = (pgversion < 9) ? "pgsql" : "postgresql";
			servicename ^= "-" ^ pgversion;

			printl("Stopping " ^ servicename ^ " (to update pgexodus.dll plugin) ...");
			if (not osshell("NET STOP " ^ servicename)) {
				printl("Cant stop " ^ servicename ^ ". Please restart it manually.");
			} else {
				printl("Restarting " ^ servicename ^ " ...");
				if (not osshell("NET START " ^ servicename))
					printl("Cant restart " ^ servicename ^ ". Please restart it manually");
			}
			if (not osremove(targetfilename))
				printl("Cant delete existing pgexodus.dll plugin");
		}

		//copy pgexodus.dll plugin into place
		//pgexodusdll.converter(OSSLASH,"/");
		//targetfilename.converter(OSSLASH,"/");
		//targetfile="D:/Program Files/PostgreSQL/8.3/lib/pgexodus.dll";
		printl(quote(pgexodusdll));
		printl("-> ", quote(targetfilename));
		if (pgexodusdll and oscopy(pgexodusdll, targetfilename)) {
			printl("done!");
			printl("Installed pgexodus.dll");
		} else {
			printl("failed!");
			printl("Did not install pgexodus.dll");
		}
	}

	printx(oconv("Configuring pgexodus postgres plugin ... ", "L#40"));
	if (not add_pgexodus_postgres_plugin())
		printl("Not enough privileges?");

	printx(oconv("Detaching from new database ... ", "L#40"));
	disconnect();
	printl("done!");

	printx(oconv("Connecting to template1 database ... ", "L#40"));
	let connstr3 = adminconfig ^ " dbname=template1";
	if (not connect(connstr3))
		abort("Stopping. Cannot connect to template1 database");
	printl("done!");

	printx(oconv("Add pgexodus postgres plugin ... ", "L#40"));
	if (not add_pgexodus_postgres_plugin())
		printl(" Not enough privileges");

	printx(oconv("Detaching from template1 database ... ", "L#40"));
	disconnect();
	printl("done!");

	return true;
}

//function configure_via_script(in adminconfig, in dbname, in dbusername, in dbuserpass) {
//	return true;
//}

programexit()
