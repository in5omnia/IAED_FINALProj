/*
* File: BG_102463.c
* Author: Beatriz Gavilan - 102463
* Description: main file with main function, the function that frees all memory                 allocated and the function that identifies commands.
 */

#include "BG_102463.h"


/*	Initializes global variables */
int g_TotalOfAirports = 0;
int g_TotalOfFlights = 0;

/* Pointer to array of airports in the system */
Airport *airportBank = NULL;

/* Pointers to the beginning and end of a doubly linked list of flights,
 * respectively */
Flight* flightBank_Head = NULL;
Flight* flightBank_Tail = NULL;

/* Declaration of the hashTable of ResNodes */
ResNode* hashTable[HASH_TABLE_SIZE];



/*	Identifies command and redirects to associated function.	*/
Date readCommand(char cmd, Date today) {

	switch (cmd) {
		case 'a': commandA();
			break;

		case 'l': commandL();
			break;

		case 'v': commandV(today);
			break;

		case 'p': command_P_C('p');
			break;

		case 'c': command_P_C('c');
			break;

		case 't': today = command_T(today);
			break;

		case 'r': commandR(today);
			break;

		case 'e': commandE();
			break;

	}
	return today;
}


/* Quits the program if there's no memory left. */
void noMemory() {
    
	printf(NO_MEMORY);
	freeAll();
	exit(1);
}


/* Frees memory associated with a flight and its reservations. */
void freeFlight() {

	Flight *temp = flightBank_Head;

	deleteFlightReservations(temp);
	flightBank_Head = flightBank_Head->next;

	free(temp);
	if (flightBank_Head != NULL)
        flightBank_Head->prev = NULL;

}


/* Frees all memory allocated manually */
void freeAll() {
    
	while (flightBank_Head != flightBank_Tail) freeFlight();

	if (flightBank_Tail != NULL)
        freeFlight();

	if (airportBank != NULL)
        free(airportBank);
}


int main() {
    
	Date today = FIRST_TODAY;
	char cmd;
	Init_HashTable();

	while ((cmd = getchar()) != EOF && cmd != END_PROGRAM) {
		today = readCommand(cmd, today);
	}
	freeAll();
	return 0;
}


