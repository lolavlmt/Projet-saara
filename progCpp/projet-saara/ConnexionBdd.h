#include <string>
#include <vector>
#include "sqlite3.h"

class ConnexionBdd{
public:
	sqlite3 * open(const char *srcFile);
	void insert(std::string request, sqlite3 &db_handle);
	struct resultRequest{
		std::string field;
		std::string value;
	};
	typedef std::vector<struct resultRequest> tableResult;
	tableResult select(std::string request, sqlite3 &db_handle);
	void close(sqlite3* db_handle);
};