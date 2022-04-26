/*
* File: airport.c
* Author: Beatriz Gavilan - 102463
* Description: Contains all the functions related to the airports.
*/


#include "BG_102463.h"
    
/*	includes global variables */
extern int g_TotalOfAirports;
extern Airport *airportBank;


/*	Creates an airport	*/
Airport createAirport(char airportID[MAX_AIRPORT_ID],
					  char country[MAX_COUNTRY], char city[MAX_CITY]) {
	Airport airport;

	strcpy(airport.ID, airportID);
	strcpy(airport.country, country);
	strcpy(airport.city, city);
	airport.n_Departure_Flights = 0;

	return airport;
}


/*	Adds an airport to the system	*/
void addAirport(Airport new_airport) {
    
	airportBank = (Airport*)realloc(airportBank, sizeof(Airport)*(g_TotalOfAirports+1));
	airportBank[g_TotalOfAirports] = new_airport;
}


/*	Checks if an airportID is valid: if it's made of 3 upper case letters.
 * Returns 1 if it's valid and 0 if not.	*/
int validAirportID(char airportID[MAX_AIRPORT_ID]) {

	int i;

	for (i=0; airportID[i] != '\0'; i++) {

		if (i >= MAX_AIRPORT_ID-1) {
			printf(INVALID_AIRPORT_ID);
			return 0;
		}

		if (airportID[i] < 'A' || airportID[i] > 'Z') {
			printf(INVALID_AIRPORT_ID);
			return 0;
		}
	}

	if (i < MAX_AIRPORT_ID-1) {
		printf(INVALID_AIRPORT_ID);
		return 0;
	}

	return 1;
}


/*	Checks if an airport exists in the system. Returns 1 if so and 0 if not. */
int airportExist(char airportID[MAX_AIRPORT_ID], char flag) {
    
	int i;

	for (i=0; i < g_TotalOfAirports; i++) {

		if (!strcmp(airportID, airportBank[i].ID)) {
			if (CASE_A)
				printf(DUPLICATE_AIRPORT);
			return ++i;
		}
	}
	if (!CASE_A)
		printf(OUT_NO_AIRPORT_ID, airportID);

	return 0;
}


/*	Compares 2 words made of 3 letters in relation to their alphabetical order.
 * Returns 1 if "beforeWord" should come before "afterWord" and 0 if not.	*/
int beforeLetters(char beforeWord[MAX_AIRPORT_ID],
				  char afterWord[MAX_AIRPORT_ID]) {

	return (less(beforeWord[0], afterWord[0]) ||

			((beforeWord[0] == afterWord[0])
			 && less(beforeWord[1], afterWord[1])) ||

			((beforeWord[0] == afterWord[0])
				&& (beforeWord[1] == afterWord[1])
				&& less(beforeWord[2], afterWord[2])));
}


/*	Exchanges 2 airports' placement in the airportBank array. */
void exch(int i, int right) {

	Airport t = airportBank[i];
	airportBank[i] = airportBank[right];
	airportBank[right] = t;

}

/*	Creates partitions of airports (based on quick sort's algorithm). */
int partition(int left, int right) {
    
	int i = left-1;
	int j = right;

	char v[MAX_AIRPORT_ID];
	strcpy(v, airportBank[right].ID);

	while (i < j) {
		while (beforeLetters(airportBank[++i].ID, v));

		while (beforeLetters(v, airportBank[--j].ID)) {
			if (j == left)
				break;
		}
		if (i < j)
			exch(i, j);
	}
	exch(i, right);
	return i;
}


/*	Sorts an array of airports in alphabetical order (based on quick sort's
 * algorithm) with the partition function and the recursive use of its
 * algorithm, creating partitions from other partitions until all the airports
 * are in order.	*/
void sortAirports(int left, int right) {
    
	int i;

	if (right <= left)
		return;

	i = partition(left, right);
	sortAirports(left, i - 1);
	sortAirports(i + 1, right);
}


/*	Lists airports of an array in the standard output.	*/
void listAirports(Airport* airportList, int num) {
    
	int i;

	if (!num)
        num = g_TotalOfAirports;

	for (i=0; i < num; i++)

		printf(OUT_AIRPORT, airportList[i].ID, airportList[i].city,
			   airportList[i].country, airportList[i].n_Departure_Flights);

}


/*	Finds the airports that exist with the requested IDs and stores them in the
 * requestedAirports array for posterior access and returns the number of
 * airports found. */
int findAirports(int num_IDs, int existingID[MAX_AIRPORTS],
				 Airport* requestedAirports,
				 char requested_IDs[MAX_AIRPORTS][MAX_AIRPORT_ID]) {

	int i, e, c, num_airports_found=0;

	for (c=0; c < num_IDs; c++) {
		existingID[c] = 0;
	}

	for (i=0; i < g_TotalOfAirports; i++) {

		if (num_airports_found == num_IDs)
			break;

		for (e=0; e < num_IDs; e++) {

			if (!strcmp(airportBank[i].ID, requested_IDs[e])) {

				requestedAirports[e] = airportBank[i];
				num_airports_found++;
				existingID[e] = 1;
				break;
			}
		}
	}
	return num_airports_found;
}


/*	Lists in the standard output the airports associated to the requested IDs
 * if they exist, and the error messages to those that don't.   */
void listRequestedAirports(char requested_IDs[MAX_AIRPORTS][MAX_AIRPORT_ID],
						   int num_IDs) {

	int a, num_airports_found, existingID[MAX_AIRPORTS];
	Airport requestedAirports[MAX_AIRPORTS];

	num_airports_found = findAirports(num_IDs, existingID,
									  requestedAirports, requested_IDs);

	if (num_airports_found != num_IDs) {

		for (a=0; a < num_IDs; a++) {

			if (!existingID[a])
				printf(OUT_NO_AIRPORT_ID, requested_IDs[a]);
			
			else {
				printf(OUT_AIRPORT, requestedAirports[a].ID,
					   requestedAirports[a].city,
					   requestedAirports[a].country,
					   requestedAirports[a].n_Departure_Flights);
			}
		}
	}

	else
		listAirports(requestedAirports, num_IDs);

}
