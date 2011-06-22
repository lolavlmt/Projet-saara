#include <string>
#include <vector>
#include <iostream>

#include "sqlite3.h"

class ConnexionBdd{
public:
	sqlite3 * open(const char *srcFile);
	void insert(std::string request, sqlite3 *db_handle);
	struct ResultRequest{
		std::vector <std::string> field;
		std::vector <std::string> value;
	};
	typedef std::vector<struct ResultRequest> TableResult;
	TableResult select(std::string request, sqlite3 *db_handle);
	void close(sqlite3* db_handle);
};