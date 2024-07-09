#include <exodus/program.h>
programinit()

function main() {
	var sql		  = "";
	let filenames = listfiles();
	// var nfiles=filenames.count(FM);
	// for ( var filen=1;filen<=nfiles;filen++) {
	// 	let filename=filenames.f(filen);
	for (var filename : filenames) {
//		if (filename.first(5) != "DICT.")
		if (not filename.starts("DICT."))
			continue;
		// var filesql =" select '" ^ filename ^ "' as filename,convert_from(key, 'UTF-8') as key, translate(convert_from(data, 'UTF-8'),'" ^ RM ^ FM ^ VM ^ SM ^ TM ^ ST ^ L"','\u02FF\u02FE\u02FD\u02FC\u02FB\u02FA') as data from " ^ filename ^ "\n";
		// var filesql =" select '" ^ filename ^ "' as filename, key, data from " ^ filename ^ "\n";
		let filesql = " select ('" ^ filename ^ "'::bytea || '*' || key) as key, data from " ^ filename ^ "\n";
		if (not var().sqlexec(filesql)) {
			printl("OMITTED FROM VIEW BECAUSE PROBLEM EXECUTING:\n", sql);
			continue;
		}
		if (sql)
			sql ^= " union";
		sql ^= filesql;
	}
	let viewname = "dicts";
	sql.paster(1, "create or replace view " ^ viewname ^ " as \n");
	sql ^= " order by key";

	//var().sqlexec("drop view " ^ viewname);
	if (not var().sqlexec("drop view " ^ viewname)) {
		loglasterror();
	}

	printl(sql);
	if (not var().sqlexec(sql))
		printl("failed to create view");
	return 0;
}

programexit()
