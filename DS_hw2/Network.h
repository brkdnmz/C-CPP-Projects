#ifndef network_h
#define network_h

#include "Station.h"

struct Network{
	Base_Station *central_controller;
	void create_network();
	Base_Station* create_station(int station_number);
	void add_station(Base_Station *target, Base_Station *add_to_target);
	void add_phone(Base_Station *target, Mobile_Phone *phone);
	Mobile_Phone* find_phone(Base_Station *travel, Mobile_Phone *phone);
	Base_Station* find_station(Base_Station* travel, int target);
	void display(Base_Station* station);//Not a part of the homework, for debugging
	void path_to_cc(Mobile_Phone* phone);
	void close_network(Base_Station* head);
};

#endif