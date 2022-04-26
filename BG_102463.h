/*
* File: BG_102463.h
* Author: Beatriz Gavilan - 102463
* Description: Header file with definitions and prototypes useful to the
* 				project functions.
 */


#ifndef BG_102463_H
#define BG_102463_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CITY 51
#define MAX_COUNTRY 31
#define MAX_AIRPORT_ID 4
#define MAX_AIRPORTS 40
#define MAX_FLIGHTS 30000
#define MAX_FLIGHT_ID 7
#define FLIGHT_ID_MAXLEN 6
#define MAX_CMD_R 65518
#define MAX_CMD_E 65535
#define HASH_TABLE_SIZE 32297

#define FIRST_TODAY {1,1,2022}

#define END_PROGRAM 'q'

#define CASE_P flag == 'p'
#define CASE_C flag == 'c'
#define CASE_A (flag == 'a')
#define CASE_V 'v'

#define OUT_AIRPORT_ID "airport %s\n"
#define OUT_AIRPORT "%s %s %s %d\n"
#define OUT_FLIGHT "%s %s %s %02d-%02d-%d %02d:%02d\n"
#define OUT_P_C_FLIGHT "%s %s %02d-%02d-%d %02d:%02d\n"
#define OUT_RES_CODE_AND_PASS "%s %d\n"
#define OUT_DATE "%02d-%02d-%d\n"


#define OUT_NO_AIRPORT_ID "%s: no such airport ID\n"
#define DUPLICATE_AIRPORT "duplicate airport\n"
#define INVALID_AIRPORT_ID "invalid airport ID\n"
#define TOO_MANY_AIRPORTS "too many airports\n"
#define INVALID_DURATION "invalid duration\n"
#define INVALID_CAPACITY "invalid capacity\n"
#define TOO_MANY_FLIGHTS "too many flights\n"
#define DUPLICATE_FLIGHT "flight already exists\n"
#define INVALID_DATE "invalid date\n"
#define INVALID_FLIGHT_ID "invalid flight code\n"


#define FLIGHT_DOESNT_EXIST "%s: flight does not exist\n"
#define OUT_INVALID_PASSENGER_NUM "invalid passenger number\n"
#define OUT_INVALID_RES_CODE "invalid reservation code\n"
#define DUPLICATE_RESERVATION "%s: flight reservation already used\n"
#define TOO_MANY_RESERVATIONS "too many reservations\n"
#define NO_MEMORY "No memory\n"
#define NOT_FOUND "not found\n"


#define IN_DATE "%d-%d-%d"
#define IN_TIME "%d:%d"
#define IN_CAPACITY "%d"
#define IN_STR "%s"
#define IN_CITY " %[^\n]"
#define IN_RES_CODE_AND_PASS "%s %d"


#define OUT_P_C_FLIGHT_VARIABLES f.ID, airportInOutput,\
f_date.day, f_date.month, f_date.year, f_time.hour, f_time.min

#define VALID_PASSENGER_NUM (passengerNum > 0)
#define less(A, B) (A < B)



typedef struct {
	char ID[MAX_AIRPORT_ID];
	char country[MAX_COUNTRY];
	char city[MAX_CITY];
	int n_Departure_Flights;
	int n_Arrival_Flights;
} Airport;


typedef struct {
	int day;
	int month;
	int year;
} Date;


typedef struct {
	int hour;
	int min;
} Time;


typedef struct {
	Date date;
	Time time;
} DateTime;


typedef struct flight {

	char ID[MAX_FLIGHT_ID];
	char departureAirport[MAX_AIRPORT_ID];
	char arrivalAirport[MAX_AIRPORT_ID];
	DateTime departureDateTime;
	DateTime arrivalDateTime;
	int capacity;
	int numPassengers;
	struct reservation *flightResHead;
	struct flight *next;
	struct flight *prev;

} Flight;


typedef struct reservation {
	char* reservationCode;
	int passengerNum;
	Flight *flight_ptr;
	struct reservation * prev;
	struct reservation *next;
} Reservation;


typedef struct node {
	Reservation *reservation;
	struct node *prevNode;
	struct node *nextNode;
} ResNode;


/*	flight.c functions	*/

Flight createFlight(char flightID[], char departureAirportID[MAX_AIRPORT_ID],
					char arrivalAirportID[MAX_AIRPORT_ID],
					DateTime departureDateTime, DateTime arrivalDateTime,
					int capacity);

int validFlightID(char flightID[]);

int validCapacity(int capacity);

Flight* duplicateFlight(char flightID[], Date departureDate,
						char flag);

int tooManyFlights();

void addFlight(Date departure_date, Time departureTime, Time duration,
			   int capacity, char flightID[],
			   char depAirportID[MAX_AIRPORT_ID],
			   char arrAirportID[MAX_AIRPORT_ID]);

void findFlights(char airportID[MAX_AIRPORT_ID], char flag);

void sortFlights( Flight desiredFlights[MAX_FLIGHTS], int left, int right,
				 char flag);

void listAllFlights();

void outputFlights_P_C(Flight wantedFlights[MAX_FLIGHTS], int num_flights,
					   char flag);

int deleteFlight(char flightID[]);


/*	date_time.c functions	*/

Date createDate(int day, int month, int year);

DateTime createDateTime(Date date, Time time);

Date newDate(Date possibleDate, Date today);

DateTime sumDuration(DateTime departure, Time duration);

int pastDate(int day, int month, int year, Date today);

int afterOneYear(int day, int month, int year, Date today);

void outputDate(Date date);

int check_date(Date date, Date today);

int validDuration(Time duration);

int sameDate(Date date1, Date date2);

int beforeTime(Time time1, Time time2);

int beforeDateTime(DateTime dateTime1, DateTime dateTime2);


/*	airport.c functions	*/

Airport createAirport(char airportID[MAX_AIRPORT_ID],
					  char country[MAX_COUNTRY], char city[MAX_CITY]);

void addAirport(Airport new_airport);

int validAirportID(char airportID[MAX_AIRPORT_ID]);

int airportExist(char airportID[MAX_AIRPORT_ID], char flag);

int beforeLetters(char beforeWord[MAX_AIRPORT_ID],
				  char afterWord[MAX_AIRPORT_ID]);

void exch(int i, int right);

int partition(int left, int right);

void sortAirports(int left, int right);

void listAirports(Airport* airportList, int num);

int findAirports(int num_IDs, int existingID[MAX_AIRPORTS],
				 Airport* requestedAirports,
				 char requested_IDs[MAX_AIRPORTS][MAX_AIRPORT_ID]);

void listRequestedAirports(char requested_IDs[MAX_AIRPORTS][MAX_AIRPORT_ID],
						   int num_IDs);


/*	main file functions	*/

Date readCommand(char cmd, Date today);

void noMemory();

void freeAll();


/*	commands.c file functions */

int validate_case_a(char airportID[]);

int valid_case_v(char flightID[], char arrivalAirportID[MAX_AIRPORT_ID],
				 char departureAirportID[MAX_AIRPORT_ID], Date departureDate,
				 Time duration, int capacity, Date today);

void commandA();

void commandL();

void commandV(Date today);

void command_P_C(char flag);

Date command_T(Date today);

void commandR(Date today);

void commandE();


/*	reservations.c file functions	*/

int validReservationCode(char* reservationCode);

int duplicateReservation(char* reservation_code);

int tooManyReservations(int reservationPassengers, Flight *flight_ptr);

Flight* validReservation(char flightId[], Date flightDate, char* reservationCode,
						 int passengerNum, Date today);

void listReservations(char flightId[], Date flightDate, Date today);

void flightResList_Init(Flight *flight_ptr, Reservation* new);

int beforeRes(Reservation* res1, Reservation* res2);

void insertBetween(Reservation* prev, Reservation* next, Reservation* new, Flight* flight_ptr);

void addFlightRes_InOrder(Flight* flight_ptr, Reservation* new);

int add_Reservation(char flightId[], Date flightDate,
					char* reservationCode, int passengerNum, Date today);

void deleteResFromFlight(Reservation* res);

void deleteFlightReservations(Flight* flight_ptr);


/* hashTable.c file functions */

void Init_HashTable() ;

int hash(char *code);

ResNode* searchReservation(char* code, int hashValue);

void insertInHashTable(Reservation* new, int hashValue);

int deleteReservation(char* code, int flag);



#endif

