#ifndef phone_h
#define phone_h

struct Mobile_Phone{
	char *message;
	int phone_number;
	int connected_station;
	Mobile_Phone *next;
	void receive_message(Mobile_Phone *phone, char *message);
};

#endif