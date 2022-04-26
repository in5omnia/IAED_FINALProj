/*
* File: flight.c
* Author: Beatriz Gavilan - 102463
* Description: Contains all the functions related to the flights.
 */


#include "BG_102463.h"

/*	Includes global variables */
extern int g_TotalOfFlights;
extern Airport *airportBank;
extern Flight* flightBank_Tail;
extern Flight* flightBank_Head;


/* Initializes the doubly linked list of flights */
void initFlightBank(Flight* new) {
    
	flightBank_Head = new;
	flightBank_Tail = new;
	flightBank_Tail->next = NULL;
	flightBank_Head->prev = NULL;
}


/* Inserts a flight at the end of the linked list of flights. */
void insertEnd(Flight* new) {
    
	new->prev = flightBank_Tail;
	flightBank_Tail->next = new;
	flightBank_Tail = new;
}


/* Creates a Flight.	*/
Flight createFlight(char flightID[], char departureAirportID[MAX_AIRPORT_ID],
					char arrivalAirportID[MAX_AIRPORT_ID],
					DateTime departureDateTime, DateTime arrivalDateTime,
					int capacity) {

	Flight newFlight;

	strcpy(newFlight.ID, flightID);
	strcpy(newFlight.departureAirport, departureAirportID);
	strcpy(newFlight.arrivalAirport, arrivalAirportID);

	newFlight.departureDateTime = departureDateTime;
	newFlight.arrivalDateTime = arrivalDateTime;
	newFlight.capacity = capacity;

	newFlight.numPassengers = 0;
	newFlight.flightResHead = NULL;

	newFlight.next = NULL;
	newFlight.prev = NULL;

	return newFlight;
}



/*	Checks flightID's validity - checks if it has 2, all upper case, letters
 * and if its number is within the correct interval. Returns 1 if it's valid
 * and 0 if not.	*/
int validFlightID(char flightID[]) {
    
	int i;

	if (strlen(flightID) > FLIGHT_ID_MAXLEN) {
		printf(INVALID_FLIGHT_ID);
		return 0;
	}

	for (i=0; flightID[i] != '\0' && i < 2; i++) {

		if (flightID[i] < 'A' || flightID[i] > 'Z') {
			printf(INVALID_FLIGHT_ID);
			return 0;
		}
	}

	if (flightID[2] == '0') {
		printf(INVALID_FLIGHT_ID);
		return 0;
	}

	for (i=3; flightID[i] != '\0'; i++) {
		if (flightID[i] < '0' || flightID[i] > '9' || i > 5) {
			printf(INVALID_FLIGHT_ID);
			return 0;
		}
	}
	return 1;
}


/*	Checks capacity's validity - checks if it's within the correct interval	*/
int validCapacity(int capacity) {

	if (capacity < 10) {

		printf(INVALID_CAPACITY);
		return 0;
	}
	return 1;
}


/*	Checks if flight already exists (is duplicate). Returns 0 if it exists and
 * 1 if not. */
Flight* duplicateFlight(char flightID[], Date departureDate, char flag) {

	Flight* aux;
	for (aux = flightBank_Head; aux != NULL ; aux = aux->next) {

		if (!strcmp(flightID, aux->ID)
			&& sameDate(departureDate,aux->departureDateTime.date)) {

			if (flag == 'v')
				printf(DUPLICATE_FLIGHT);
			return aux;
		}
	}
	if (flag == 'r')
		printf(FLIGHT_DOESNT_EXIST, flightID);
	return NULL;
}


/*	Checks if adding a flight will exceed the system's limit of flights.
 * Returns 1 if it's valid and 0 if not.	*/
int tooManyFlights() {

	int num_flights = g_TotalOfFlights;

	if (++num_flights > MAX_FLIGHTS) {
		printf(TOO_MANY_FLIGHTS);
		return 1;
	}
	return 0;
}


/*	Creates and adds new flight to the system	*/
void addFlight(Date departure_date, Time departureTime, Time duration,
			   int capacity, char flightID[],
			   char depAirportID[MAX_AIRPORT_ID],
			   char arrAirportID[MAX_AIRPORT_ID]) {

	DateTime departureDateTime;
	DateTime arrivalDateTime;
	Flight *newFlight = (Flight*)malloc(sizeof(Flight));

	departureDateTime = createDateTime(departure_date,departureTime);

	arrivalDateTime = sumDuration(departureDateTime, duration);

	if (newFlight == NULL)
		noMemory();
	

	(*newFlight) = createFlight(flightID,depAirportID, arrAirportID,
							  departureDateTime, arrivalDateTime,
							  capacity);

	if (flightBank_Head==NULL)
		initFlightBank(newFlight);
	
	else
		insertEnd(newFlight);

	g_TotalOfFlights++;

}


/*	Checks if the airport exists and if so, finds them in flightBank, sorts
 * and presents them in standard output	*/
void findFlights(char airportID[MAX_AIRPORT_ID], char flag) {

	Flight wantedFlights[MAX_FLIGHTS], *aux;
	int number_flights, num_flights_found=0;
	int airportIndexPlus1 = airportExist(airportID, CASE_V);

	if (!airportIndexPlus1)
		return;

	if (CASE_P)
		number_flights = airportBank[airportIndexPlus1-1].n_Departure_Flights;

	else
		number_flights = airportBank[airportIndexPlus1-1].n_Arrival_Flights;


	for (aux = flightBank_Head; aux != NULL; aux = aux->next) {

		if (num_flights_found == number_flights)
			break;

		if ((CASE_P && !strcmp(aux->departureAirport, airportID))
			|| (CASE_C && !strcmp(aux->arrivalAirport, airportID))) {

			wantedFlights[num_flights_found] = *aux;
			num_flights_found++;
		}
	}
	sortFlights(wantedFlights, 0, num_flights_found -1, flag);
	outputFlights_P_C(wantedFlights, num_flights_found, flag);
}


/*	Sorts flights by date from the oldest to the most recent, based on
 * insertion sort's algorithm	*/
void sortFlights(Flight desiredFlights[MAX_FLIGHTS], int left, int right,
				 char flag) {
	int i,j;
	Flight v;

	for (i = left+1; i <= right; i++) {

		v = desiredFlights[i];
		j = i-1;

		if (CASE_P) {
			while ( j >= left && beforeDateTime(v.departureDateTime,
											   desiredFlights[j].departureDateTime))
			{
				desiredFlights[j + 1] = desiredFlights[j];
				j--;
			}
		} else {
			while ( j >= left && beforeDateTime(v.arrivalDateTime,
											   desiredFlights[j].arrivalDateTime))
			{
				desiredFlights[j + 1] = desiredFlights[j];
				j--;
			}
		}
		desiredFlights[j+1] = v;
	}
}


/*	Lists all the system's flights (stored in flightBank) in the standard
 * output	*/
void listAllFlights() {
    
	Date f_date;
	Time f_time;
	Flight f, *aux;

	for (aux=flightBank_Head; aux != NULL; aux=aux->next) {

		f = *aux;
		f_date = f.departureDateTime.date;
		f_time = f.departureDateTime.time;

		printf(OUT_FLIGHT, f.ID, f.departureAirport,
			   f.arrivalAirport, f_date.day, f_date.month, f_date.year,
			   f_time.hour, f_time.min);
	}
}


/*	Lists flights (stored in wantedFlights) for P and C commands in the
 * standard output */
void outputFlights_P_C(Flight wantedFlights[MAX_FLIGHTS], int num_flights,
					   char flag) {
	int i;
	Date f_date;
	Time f_time;
	Flight f;
	char airportInOutput[MAX_AIRPORT_ID];

	if (CASE_P) {
		for (i=0; i < num_flights; i++) {

			f = wantedFlights[i];
			f_date = f.departureDateTime.date;
			f_time = f.departureDateTime.time;
			strcpy(airportInOutput, f.arrivalAirport);

			printf(OUT_P_C_FLIGHT, OUT_P_C_FLIGHT_VARIABLES);
		}
	} else {

		for (i=0; i < num_flights; i++) {

			f = wantedFlights[i];
			f_date = f.arrivalDateTime.date;
			f_time = f.arrivalDateTime.time;
			strcpy(airportInOutput, f.departureAirport);

			printf(OUT_P_C_FLIGHT, OUT_P_C_FLIGHT_VARIABLES);
		}
	}
}


/* Deletes a flight from the list of flights. */
int deleteFlight(char flightID[]) {
    
	Flight *aux, *next;
	int flag = 0;
	for (aux = flightBank_Head; aux != NULL; aux = next) {

		if (!strcmp(flightID, aux->ID)) {
			deleteFlightReservations(aux);

			if (aux == flightBank_Head)
				flightBank_Head = aux->next;
			else
				aux->prev->next = aux->next;

			if (aux == flightBank_Tail)
				flightBank_Tail = aux->prev;
			else
				aux->next->prev = aux->prev;

			next = aux->next;
			free(aux);
			flag = 1;

		} else {
			next = aux->next;
		}
	}
	return flag;
}

