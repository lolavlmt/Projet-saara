#include "Navigation.h"

using namespace std;

Navigation::Navigation(void){
}

void Navigation::searchMediaTag(unsigned int idLink){
	ConnexionBdd connexionBdd;
	sqlite3 *db = connexionBdd.open("../../dataWiki.sqlite");
	
	char request[512];
	sprintf(request, "SELECT * FROM Link, Theme WHERE Link.id=%d AND Theme.id = Link.idTheme ;", idLink);

	ConnexionBdd::TableResult result;
	result = connexionBdd.select(request, db);

	//cout << result[2].field[1] << "\n";		= idTheme
	//cout << result[3].value[4] << "\n";		= valeur du 4ème champs à la 3ème ligne

	//writeDataThemeToFile(result);

	connexionBdd.close(db);
}
