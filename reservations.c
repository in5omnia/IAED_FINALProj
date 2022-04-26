/*
* File: reservations.c
* Author: Beatriz Gavilan - 102463
* Description: Contains functions related to the reservations.
*/

#include "BG_102463.h"


/* includes global variables */
extern int g_TotalOfFlights;
extern Flight* flightBank_Tail;
extern Flight* flightBank_Head;
extern ResNode* hashTable[HASH_TABLE_SIZE];



/* Checks if the reservation code is valid. Returns 1 if so, and 0 if not. */
int validReservationCode(char* reservationCode) {
    
   int i;
   if (strlen(reservationCode) < 10) {
	   printf(OUT_INVALID_RES_CODE);
	   return 0;
   }

   for (i = 0; reservationCode[i] != '\0'; i++) {
	   /* invalid if a string character isn't a digit or a capital letter */
	   if (!((reservationCode[i] >= 'A' && reservationCode[i] <= 'Z') ||
			 (reservationCode[i] >= '0' && reservationCode[i] <= '9'))) {
		   printf(OUT_INVALID_RES_CODE);
		   return 0;
	   }
   }
   return 1;
}


/* Checks if a reservation with the same code already exists */
int duplicateReservation(char* reservation_code) {

   int hashValue = hash(reservation_code);

   if (searchReservation(reservation_code, hashValue) != NULL) {
	   printf(DUPLICATE_RESERVATION, reservation_code);
	   return 1;
   }
   return 0;
}


/* Checks if the flight is full. Returns 1 if so and 0 if not. */
int tooManyReservations(int reservationPassengers, Flight* flight_ptr) {

   if ((flight_ptr->numPassengers + reservationPassengers) >
	   flight_ptr->capacity) {
	   printf(TOO_MANY_RESERVATIONS);
	   return 1;
   }
   return 0;
}


/* Checks if the reservation is valid. */
Flight* validReservation(char flightId[], Date flightDate,
						char* reservationCode, int passengerNum, Date today) {
   Flight* flight_ptr;

   if (!validFlightID(flightId)) {
	   return NULL;
   }

   if (!validReservationCode(reservationCode)) {
	   return NULL;
   }

   flight_ptr = duplicateFlight(flightId, flightDate, 'r');
   if (flight_ptr == NULL) {
	   return NULL;
   }

   if (duplicateReservation(reservationCode) ||
	   tooManyReservations(passengerNum, flight_ptr) ||
	   !check_date(flightDate, today)) {
	   return NULL;
   }

   if (!VALID_PASSENGER_NUM) {
	   printf(OUT_INVALID_PASSENGER_NUM);
	   return NULL;
   }

   return flight_ptr;
}


/* Lists the flight's reservations. */
void listReservations(char flightId[], Date flightDate, Date today) {
    
   Reservation* res;
   Flight* flight_ptr;

   if (!validFlightID(flightId)) return;


   flight_ptr = duplicateFlight(flightId, flightDate, 'r');

   if (flight_ptr == NULL || !check_date(flightDate, today)) return;

   for (res = flight_ptr->flightResHead; res != NULL; res = res->next) {
	   printf(OUT_RES_CODE_AND_PASS, res->reservationCode, res->passengerNum);
   }
}


/* Adds the first reservation of a flight. */
void flightResList_Init(Flight* flight_ptr, Reservation* new) {
    
   flight_ptr->flightResHead = new;
   new->next = NULL;
   new->prev = NULL;
}


/* Checks if a reservation (res1) should come before another (res2) in
* lexicographic order. */
int beforeRes(Reservation* res1, Reservation* res2) {
    
    return (strcmp(res1->reservationCode, res2->reservationCode) < 0);
}


/* Adds a reservation to the flight's reservation list. */
void insertBetween(Reservation* prev, Reservation* next, Reservation* new,
				  Flight* flight_ptr) {

   if (prev == NULL) {
	   new->prev = NULL;
	   flight_ptr->flightResHead = new;
   }
   else
	   new->prev = prev;

   if (next == NULL)
	   new->next = NULL;
   else
	   new->next = next;

   if (prev != NULL)
       prev->next = new;

   if (next != NULL)
       next->prev = new;
}


/* Adds a reservation to the flight's list of reservations, already sorted. */
void addFlightRes_InOrder(Flight* flight_ptr, Reservation* new) {

   Reservation *aux = flight_ptr->flightResHead, *prev = NULL;

   while (aux != NULL && beforeRes(aux, new)) {
	   prev = aux;
	   aux = aux->next;
   }

   insertBetween(prev, aux, new, flight_ptr);
}


/* Adds a reservation if it's valid. Returns 1 if succesful, 0 if not. */
int add_Reservation(char flightId[], Date flightDate, char* reservationCode,
				   int passengerNum, Date today) {

   Reservation* new = (Reservation*)malloc(sizeof(Reservation));
   Flight* flight_ptr = validReservation(flightId, flightDate, reservationCode,
										 passengerNum, today);
   int hashValue;

   if (new == NULL)
	   noMemory();
   

   if (flight_ptr == NULL) return 0;

   new->reservationCode = reservationCode;
   new->passengerNum = passengerNum;
   new->flight_ptr = flight_ptr;
   new->next = NULL;
   new->prev = NULL;

   if (flight_ptr->flightResHead == NULL)
	   flightResList_Init(flight_ptr, new);
   else
	   addFlightRes_InOrder(flight_ptr, new);
   

   hashValue = hash(reservationCode);
   insertInHashTable(new, hashValue);

   flight_ptr->numPassengers += passengerNum;

   return 1;
}


/* Deletes a reservation from the corresponding flight's list of reservations.*/
void deleteResFromFlight(Reservation* res) {

   res->flight_ptr->numPassengers -= res->passengerNum;

   if (res->next != NULL)
	   res->next->prev = res->prev;

   if (res->prev != NULL)
	   res->prev->next = res->next;

   else
	   res->flight_ptr->flightResHead = res->next;
}


/* Deletes all reservations of a flight. */
void deleteFlightReservations(Flight* flight_ptr) {
    
   Reservation *res, *next;

   for (res = flight_ptr->flightResHead; res != NULL; res = next) {
	   next = res->next;
	   deleteReservation(res->reservationCode, 0);
   }
    
}

