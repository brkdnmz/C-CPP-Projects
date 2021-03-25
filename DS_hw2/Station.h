#ifndef station_h
#define station_h

#include "Phone.h"

struct Base_Station{
	int station_number; //It's ID
	Mobile_Phone *phone_head; //This points to the first phone connected to the station
	Base_Station *station_head; //This points to the first child station
	Base_Station *next; //This points to the next neighbor station
	Base_Station *top;	//This points to the parent station
};

#endif