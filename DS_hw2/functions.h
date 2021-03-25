#ifndef functions_h
#define functions_h

#include "Network.h"
using namespace std;

int str_length(char *string){
	int length = 0;
	while(string[length] != '\0'){
		length++;
	}
	//string[length] == '\0' now, length should be decremented
	length--;
	return length;
}

void copy_str(char *to_be_copied, char *target){
	int i;
	for(i = 0; to_be_copied[i] != '\0'; i++){
		target[i] = to_be_copied[i];
	}
	target[i] = '\0';
}

void Mobile_Phone::receive_message(Mobile_Phone *phone, char *message){
	phone->message = new char[str_length(message) + 1]; //Plus 1 is for the '\0' character
	copy_str(message, phone->message);
}

void Network::create_network(){
	central_controller = new Base_Station;
	central_controller->station_number = 0;
	central_controller->top = NULL;
	central_controller->next = NULL;
	central_controller->phone_head = NULL;
	central_controller->station_head = NULL;
}

Base_Station* Network::create_station(int number){
	Base_Station *station = new Base_Station;
	station->station_number = number;
	station->top = NULL;
	station->next = NULL;
	station->phone_head = NULL;
	station->station_head = NULL;
	return station;
}

Mobile_Phone* phone_returned = NULL;
Mobile_Phone* Network::find_phone(Base_Station *station, Mobile_Phone *phone){//Always find_phone(central_controller,...)
	if(station == central_controller){
		phone_returned = NULL;
	}
	if(!phone_returned){
		cout<<station->station_number<<" "; //This is for the output "Traversing: 0 5 2..."
	}
	
	Mobile_Phone *travel_phone = station->phone_head;
	while(travel_phone != NULL && !phone_returned){
		if(travel_phone->phone_number == phone->phone_number){
			phone_returned = travel_phone;
			break;
		}
		travel_phone = travel_phone->next;
	}
	
	if(!phone_returned){
		if(station->station_head != NULL)
			find_phone(station->station_head, phone);
		if(station->next != NULL)
			find_phone(station->next, phone);
	}
	return phone_returned;
}

Base_Station* station_returned = NULL;
Base_Station* Network::find_station(Base_Station* station, int target){//Always find_station(central_controller, target)
	if(station == central_controller){
		station_returned = NULL;
	}
	if(station->station_number == target){
		station_returned = station;
	}
	
	if(!station_returned){
		if(station->station_head != NULL)
			find_station(station->station_head, target);
		if(station->next != NULL)
			find_station(station->next, target);
	}
	return station_returned;
}

void Network::add_station(Base_Station *station, Base_Station *add){
	if(station->station_head == NULL){
		station->station_head = add;
		add->top = station;
		return;
	}
	Base_Station *travel = station->station_head;
	while(travel->next != NULL){
		travel = travel->next;
	}
	travel->next = add;
	add->top = station;
}

void Network::add_phone(Base_Station *station, Mobile_Phone *phone){
	if(station->phone_head == NULL){
		station->phone_head = phone;
		phone->connected_station = station->station_number;
		phone->next = NULL;
		return;
	}
	
	Mobile_Phone *travel = station->phone_head;
	while(travel->next != NULL)
		travel = travel->next;
	travel->next = phone;
	phone->connected_station = station->station_number;
	phone->next = NULL;
}

void Network::display(Base_Station *station){//Always display(central_controller) <---For Debugging
	if(station != NULL){
		cout<<station->station_number<<endl;
		Mobile_Phone *travel = station->phone_head;
		while(travel != NULL){
			cout<<travel->phone_number<<" ";
			travel = travel->next;
		}
		cout<<endl;
		display(station->station_head);
		display(station->next);
	}
}

#ifndef SIZE
#define SIZE 1000
#endif
void Network::path_to_cc(Mobile_Phone* phone){
	Base_Station *connected = find_station(central_controller, phone->connected_station);
	int arr[SIZE];
	int i = 0, size = 0;
	while(connected != NULL){
		arr[i] = connected->station_number;
		size++;
		i++;
		connected = connected->top;
	}
	for(int u = size - 1; u >= 0; u--){
		cout<<arr[u]<<" ";
	}
}

void Network::close_network(Base_Station *station){//Always close_network(central_controller)
	if(station == NULL){
		return;
	}
	close_network(station->station_head);
	close_network(station->next);
	Mobile_Phone *travel_phone = station->phone_head;
	Mobile_Phone *tail = travel_phone;
	if(travel_phone != NULL){
		do{
			tail = travel_phone;
			travel_phone = travel_phone->next;
			delete[] tail->message;
			delete tail;
		}while(travel_phone != NULL);
	}
	delete station;
}

#endif