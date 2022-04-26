/*
* File: commands.c
* Author: Beatriz Gavilan - 102463
* Description: file with functions that execute or validate commands.
 */

#include "BG_102463.h"


/*	Includes global variables */
extern int g_TotalOfAirports;
extern int g_TotalOfFlights;
extern Airport *airportBank;



/*	Checks validity for "case a" : returns 1 if the airportID is valid and if
 * the limit of airports won't be exceeded when adding one. */
int validate_case_a(char airportID[]) {

	int num_airports = g_TotalOfAirports;
	if (!validAirportID(airportID))
		return 0;

	if ((++num_airports) > MAX_AIRPORTS) {
		printf(TOO_MANY_AIRPORTS);
		return 0;
	}
	return (!airportExist(airportID, 'a'));
}


/*	Checks validity for case v: checks if flightID and its airports' IDs
 * are valid; if the flight is duplicate and if the airports exist;
 * if adding a flight will exceed the limit of flights; if the flight date,
 * duration and capacity are valid.	*/

int valid_case_v(char flightID[], char arrivalAirportID[MAX_AIRPORT_ID],
				 char departureAirportID[MAX_AIRPORT_ID], Date departureDate,
				 Time duration, int capacity, Date today) {

	int departureIndexPlus1, arrivalIndexPlus1;

	if (validFlightID(flightID) &&
		duplicateFlight(flightID, departureDate, 'v') == NULL &&
		validAirportID(departureAirportID) &&
		validAirportID(arrivalAirportID)) {

		departureIndexPlus1 = airportExist(departureAirportID,CASE_V);

		arrivalIndexPlus1 = airportExist(arrivalAirportID,CASE_V);

		if (!(!departureIndexPlus1 || !arrivalIndexPlus1) &&
			!tooManyFlights() &&
			check_date(departureDate, today) &&
			validDuration(duration) && validCapacity(capacity)) {
			airportBank[departureIndexPlus1-1].n_Departure_Flights++;
			airportBank[arrivalIndexPlus1-1].n_Arrival_Flights++;
			return 1;
		}
	}
	return 0;
}


/*	Receives input for command "a" and, if it's valid, creates and adds a
 * new airport, presenting the corresponding message in standard output.	*/
void commandA() {

	char airportID[MAX_AIRPORT_ID], country[MAX_COUNTRY], city[MAX_CITY];
	scanf(IN_STR, airportID);
	scanf(IN_STR, country);
	scanf(IN_CITY, city);

	if (validate_case_a(airportID)) {

		Airport new_airport = createAirport(airportID, country, city);
		addAirport(new_airport);

		printf(OUT_AIRPORT_ID, airportID);
		g_TotalOfAirports++;

	}
}


/*	Receives remaining input for command "l" and: if there isn't any, presents
 * sorted airports; if there is, stores the airports asked for and presents the
 * valid ones. */
void commandL() {

	char airportID[MAX_AIRPORT_ID];
	if (getchar() == '\n') {
		sortAirports(0, g_TotalOfAirports - 1);
		listAirports(airportBank, 0);
	}
	else {
		char requested_IDs[MAX_AIRPORTS][MAX_AIRPORT_ID];
		int num_IDs = 0;
		do {
			scanf(IN_STR, airportID);
			strcpy(requested_IDs[num_IDs], airportID);
			num_IDs++;

		} while (getchar() != '\n');

		listRequestedAirports(requested_IDs, num_IDs);
	}
}


/*	Receives remaining input for command "v" and: if there isn't any, presents
* all flights; if there is, checks if it's valid and, if so, adds new flight to
 * FlightBank.	*/
void commandV(Date today) {

	if (getchar() == '\n')
		listAllFlights();

	else {

		int capacity;
		char depAirportID[MAX_AIRPORT_ID], arrAirportID[MAX_AIRPORT_ID],
			flightID[MAX_FLIGHT_ID];
		Date departure_date;
		Time duration, departureTime;

		scanf(IN_STR, flightID);
		scanf(IN_STR, depAirportID);
		scanf(IN_STR, arrAirportID);

		scanf(IN_DATE, &departure_date.day, &departure_date.month,
			  &departure_date.year);

		scanf(IN_TIME, &departureTime.hour, &departureTime.min);
		scanf(IN_TIME, &duration.hour, &duration.min);
		scanf(IN_CAPACITY, &capacity);

		if (!valid_case_v(flightID, arrAirportID,depAirportID, departure_date,
						  duration, capacity, today))
			return;

		addFlight(departure_date, departureTime, duration, capacity, flightID,
				  depAirportID, arrAirportID);

	}
}


/*	Receives airportID for P and C commands and finds associated flights.	*/
void command_P_C(char flag) {

	char airportID[MAX_AIRPORT_ID];
	scanf(IN_STR, airportID);
	findFlights(airportID, flag);
}


/*	Gets a date from input and if it's valid, updates "today".	*/
Date command_T(Date today) {

	int day, month, year;
	Date possibleDate;

	scanf(IN_DATE, &day, &month, &year);
	possibleDate = createDate(day, month, year);

	if (check_date(possibleDate, today)) {
		today = newDate(possibleDate, today);
		outputDate(today);
	}
	return today;
}


/*	Receives flightID and date and when there's no more input, it lists the
 * flight's reservations, otherwise, it receives the reservation's info and
 * adds it to the system.	*/
void commandR(Date today) {
    
	char *reservation_code=NULL, temp[MAX_CMD_R], flightID[MAX_FLIGHT_ID];
	int passengerNum;
	Date flightDate;

	scanf(IN_STR, flightID);
	scanf(IN_DATE, &flightDate.day, &flightDate.month, &flightDate.year);

	if (getchar()!='\n') {

		scanf(IN_RES_CODE_AND_PASS, temp, &passengerNum);

		reservation_code = (char*)malloc(sizeof (char)*(strlen(temp)+1));

		if (reservation_code == NULL)
			noMemory();
		
		strcpy(reservation_code, temp);

		if (!add_Reservation(flightID, flightDate, reservation_code,
							 passengerNum, today))
			free(reservation_code);
		
	}
	else {
		listReservations(flightID, flightDate, today);
	}
}


/* Receives a code and deletes the corresponding flight or reservation. */
void commandE() {
    
	char *code = malloc(sizeof (char)*MAX_CMD_E);
	int len;

	if (code == NULL)
		noMemory();
	

	scanf(IN_STR, code);
	len = strlen(code);
	code = realloc(code, sizeof (char)*(len+1));

	if (len < 10) {
		if (!deleteFlight(code))
			printf(NOT_FOUND);
		else
			--g_TotalOfFlights;
	}
	else {
		if (!deleteReservation(code, 1))
			printf(NOT_FOUND);
	}
	free(code);
}


