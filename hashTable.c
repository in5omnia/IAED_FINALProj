/*
* File: hashTable.c
* Author: Beatriz Gavilan - 102463
* Description: Contains the implementation of a hash table for
				reservations and the related functions.
*/

#include "BG_102463.h"

/* includes global variable */
extern ResNode* hashTable[HASH_TABLE_SIZE];



/* Initializes each ResNode from the HashTable as NULL. */
void Init_HashTable() {
    
	int i;
	for (i = 0; i < HASH_TABLE_SIZE; i++)
		hashTable[i] = NULL;
}


/* Hash function for reservation codes. */
int hash(char* code) {
    
	long int hashValue, a = 31415, b = 27183;

	for (hashValue = 0; *code != '\0';
		 code++, a = a * b % (HASH_TABLE_SIZE - 1))
		hashValue = (a * hashValue + *code) % HASH_TABLE_SIZE;

	return hashValue % HASH_TABLE_SIZE;

}


/* Searches for a reservation in the Hashtable. Returns the corresponding
 * ResNode if found, and NULL if not. */
ResNode* searchReservation(char* code, int hashValue) {
    
	ResNode* aux = NULL;
    
	if (hashTable[hashValue] == NULL)
        return NULL;
    
	if (hashTable[hashValue]->nextNode == NULL) {
        
		if (strcmp(hashTable[hashValue]->reservation->reservationCode, code) == 0)
			return hashTable[hashValue];
        
	} else {
        
		for (aux = hashTable[hashValue]; aux != NULL; aux = aux->nextNode) {
            
			if (!strcmp(aux->reservation->reservationCode, code))
                return aux;
		}
	}
	return NULL;
}


/* Inserts a new ResNode in the Hashtable. */
void insertInHashTable(Reservation* new, int hashValue) {
    
	ResNode* newNode = (ResNode*)malloc(sizeof(ResNode));
    
	if (newNode == NULL)
        noMemory();
    
	newNode->prevNode = NULL;
	newNode->reservation = new;
	newNode->nextNode = hashTable[hashValue];
    
	if (hashTable[hashValue] != NULL)
        hashTable[hashValue]->prevNode = newNode;
    
	hashTable[hashValue] = newNode;
}


/* Deletes a reservation from the HashTable and from the corresponding flight's
 * list. Returns 1 if successful and 0 if not. */
int deleteReservation(char* code, int flag) {
    
	int i = hash(code);
	ResNode* nodeToDelete = searchReservation(code, i);

	if (nodeToDelete == NULL)
        return 0;

	if (flag)
		deleteResFromFlight(nodeToDelete->reservation);

	free(nodeToDelete->reservation->reservationCode);
	free(nodeToDelete->reservation);

	if (nodeToDelete->prevNode != NULL)
		nodeToDelete->prevNode->nextNode = nodeToDelete->nextNode;
	else
		hashTable[i] = nodeToDelete->nextNode;

	if (nodeToDelete->nextNode != NULL)
		nodeToDelete->nextNode->prevNode = nodeToDelete->prevNode;

	return 1;
}



