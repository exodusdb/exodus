#ifndef VARDBCONNS_H
#define VARDBCONNS_H

//
// Exodus -
//
// vardbconn.h - keep table of connections in the way, that connection is linked to filename/file
//		variable (that is linked to SQL TABLE name - via connection number in field 2 of filename)
//
// Requirement 1. Current threadwise default connection id: tss_pgconnids.get();

// Requirement 2. When open(filename) - store connection in field 2

// Requirement 3. read/write: if filename has not connection no then use default connection

// Requirement 5. NO ! There is connection table, where all connection variables should be
// registered (at 2 and 3)

// Requirement 6. No action is performed in destructor of 'filename' since connection is common

// Requirement 7. disconnect() releases the default connection

// Requirement 8. filename.disconnect() closes the connection, linked to the 'filename', and 'frees'
// this var

//Requirement 9. Closed connection: just erased from map

//Requirement 10. Attempt to use 'invalidated' connection raises exception 

//Requirement 11. ConnectionLockss (which accompany DB table locks), are added to connection table and
//					stored/deleted within connection record

//for postgresql's PGconn connection handle type
#include <libpq-fe.h>

// Using map generally instead of unordered_map since it is faster
// up to about 400 elements according to https://youtu.be/M2fKMP47slQ?t=258
// and perhaps even more since it doesnt require hashing time.
// Perhaps switch to this https://youtu.be/M2fKMP47slQ?t=476
#include <map>

//currently only for dbcache
#include <unordered_map>

#include <exodus/var.h>

namespace exodus {

using ConnectionLocks = std::map<uint64_t, int>;
using DBCache = std::unordered_map<uint64_t, std::string>;
using PGCONN_DELETER = void (*)(PGconn*);

class DBConn;
using DBConns = std::map<int, DBConn>;
//using DBConnector = std::unordered_map<int, DBConn>;

// Holds a pointer to a PGconn, caches for records, and a lock table
class DBConn	 // used as 'second' in pair, stored in connection map
{
   public:

	// ctors

	DBConn()
		: pgconn_(0) {}

	DBConn(PGconn* pgconn, std::string conninfo)
		: pgconn_(pgconn), conninfo_(conninfo) {
	}

	// members
	//////////

	// 1=entry is in use
	// 0=entry is not longer used and may be reused
	int flag = 0;

	// postgres connection handle
	PGconn* pgconn_;

	// postgres locks per dbconn
	// used to fail lock (per mv standard_ instead of stack locks (per postgres standard)
	//ConnectionLocks* locks_;
	ConnectionLocks locks_;

	//?
	int extra_ = 0;

	DBCache dbcache_;

	std::string conninfo_;

	bool in_transaction_ = false;

};

// "final" to avoid declaring the destructor as virtual as a precaution
class DBConnector final {

	// data members
	///////////////

	// function to close pg connection
	PGCONN_DELETER del_;

	// number of db connections
	int dbconn_no_;

	// container
	DBConns dbconns_;

   public:

	// ctors/dtors
	//////////////

	DBConnector(PGCONN_DELETER del_);

	//class marked as final so no need for virtual
	//virtual ~DBConnector();
	~DBConnector();

	// manipulators
	///////////////

	int add_dbconn(PGconn* connection_with_file, const std::string conninfo);
	void del_dbconn(int dbconn_no);
	void del_dbconns(int from_dbconn_no);

	// observers
	////////////

	PGconn* get_pgconn(int dbconn_no) const;
	DBConn* get_dbconn(int dbconn_no) const;

	DBCache* get_dbcache(int dbconn_no) const;
	std::string getrecord(const int dbconn_no, uint64_t file_and_key) const;
	void putrecord(const int dbconn_no, uint64_t file_and_key, const std::string& record);
	void delrecord(const int dbconn_no, uint64_t file_and_key);
	void cleardbcache(const int dbconn_no);

   private:

};

}  // namespace exodus
#endif	// VARDBCONNS_H
