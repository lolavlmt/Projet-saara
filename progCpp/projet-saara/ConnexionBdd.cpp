#include "ConnexionBdd.h"

using namespace std;

sqlite3 * ConnexionBdd::open(const char *srcFile){
	sqlite3 *db_handle; //base de donnée 
	
	//Ouverture de la base
	sqlite3_open(srcFile, &db_handle);

	return db_handle;
}

void ConnexionBdd::insert(string request, sqlite3 &db_handle){
	sqlite3_stmt* _pStmt;
	sqlite3_prepare_v2(&db_handle, request.c_str(), request.length(), &_pStmt, NULL);
	sqlite3_step(_pStmt);
	sqlite3_finalize(_pStmt);
	//test commit
}

ConnexionBdd::tableResult ConnexionBdd::select(string request, sqlite3 &db_handle){
	sqlite3_stmt* _pStmt;
	sqlite3_prepare_v2(&db_handle, request.c_str(), request.length(), &_pStmt, NULL);

	int iColumnCount = sqlite3_column_count(_pStmt);
	int iResult=0;
	tableResult result;

	while ((iResult = sqlite3_step(_pStmt)) != SQLITE_DONE){
		if (iResult == SQLITE_ROW){
			for (int i = 0 ; i < iColumnCount ; i++){
				result.back().field = (char *)sqlite3_column_text(_pStmt, i);
				result.back().value = (char *)sqlite3_column_name(_pStmt, i);
			}
		}
	}

	return result;
}

void ConnexionBdd::close(sqlite3* db_handle){
	//Fermeture
	sqlite3_close(db_handle);
	db_handle = NULL;
}