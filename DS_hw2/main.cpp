/* @Author
Student Name: Berke Dönmez
Student ID: 150170045
Date: 22.12.2019
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <string>
using namespace std;

#define SIZE 1000

struct Mobile_Phone{
	char *message;	//Message keeper
	int phone_number;	//Phone's ID
	int connected_station;	//The station's ID which phone is connected to
	Mobile_Phone *next;	//Next phone that is connected to the same station
	void receive_message(Mobile_Phone *phone, char *message);	//Message receiver function
};

struct Base_Station{
	int station_number; //Station's ID
	Mobile_Phone *phone_head; //The first phone connected to the station
	Base_Station *station_head; //The first station connected to the station (Child)
	Base_Station *next; //The next station connected to the same parent station
	Base_Station *top;	//The parent station
};

struct Network{
	Base_Station *central_controller;	//The central controller of the network
	void create_network();	//This function creates the network
	Base_Station* create_station(int station_number);	//This function creates a station with the given ID
	void add_station(Base_Station *target, Base_Station *add_to_target);	//Adds a station to the network
	void add_phone(Base_Station *target, Mobile_Phone *phone);	//Adds a phone to the given station(target)
	
	/*	RECURSIVE FUNCTIONS	 */
	Mobile_Phone* find_phone(Base_Station *travel, Mobile_Phone *phone);	//Gives the phone's pointer if the phone exists, else returns NULL
	Base_Station* find_station(Base_Station* travel, int target);	//Gives the station's pointer if the station exists, else returns NULL
	void close_network(Base_Station* head);	//Releases all the memory allocated for the whole network
	void display(Base_Station* station);	//Not a part of the homework, for debugging
	/*  RECURSIVE FUNCTIONS	 */
	
	void path_to_cc(Mobile_Phone* phone);	//For the output "To:0..." (Path to the phone from central controller)
};

int str_length(char *string){	//An alternative function for strlen
	int length = 0;
	while(string[length] != '\0'){
		length++;
	}
	return length;
}

void copy_str(char *to_be_copied, char *target){	//An alternative function for strcpy
	int i;
	for(i = 0; to_be_copied[i] != '\0'; i++){
		target[i] = to_be_copied[i];
	}
	//target[i-1] is the last character now
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
	//"number" is the ID of the created station
	Base_Station *station = new Base_Station;
	station->station_number = number;
	station->top = NULL;
	station->next = NULL;
	station->phone_head = NULL;
	station->station_head = NULL;
	return station;
}

Mobile_Phone* phone_returned = NULL;
Mobile_Phone* Network::find_phone(Base_Station *station, Mobile_Phone *phone){	//Always find_phone(central_controller,...)
	//"station" is the recursive traveler and "phone" is the phone we want to find
	
	if(station == central_controller){
		phone_returned = NULL;
	}
	if(!phone_returned){
		cout<<station->station_number<<" "; //This is for the output "Traversing: 0 5 2..."
	}
	
	Mobile_Phone *travel_phone = station->phone_head;	//This travels in the phone linked list of the station
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
Base_Station* Network::find_station(Base_Station* station, int target){	//Always find_station(central_controller, ...)
	//"station" is the recursive traveler and "target" is the ID of the station we want to find
	
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
	//"station" is the parent and "add" is the station we want to add
	
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
	//"station" is the station that "phone" will be connected to
	
	if(station->phone_head == NULL){
		station->phone_head = phone;
		phone->connected_station = station->station_number;
		phone->next = NULL;
		return;
	}
	
	Mobile_Phone *travel = station->phone_head;	//This goes to the last phone connected to "station"
	while(travel->next != NULL)
		travel = travel->next;
	travel->next = phone;
	phone->connected_station = station->station_number;
	phone->next = NULL;
}

void Network::display(Base_Station *station){//Always display(central_controller) <---For Debugging
	if(station != NULL){
		cout << station->station_number << endl;
		Mobile_Phone *travel = station->phone_head;
		while(travel != NULL){
			cout << travel->phone_number << " ";
			travel = travel->next;
		}
		cout << endl;
		display(station->station_head);
		display(station->next);
	}
}

void Network::path_to_cc(Mobile_Phone* phone){	//Displays path from central controller to "phone"
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
		cout << arr[u] << " ";
	}
}

void Network::close_network(Base_Station *station){	//Always close_network(central_controller)
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

int main(int arg_count, char **argv){
	FILE *fptr1;
	fptr1 = fopen(argv[1], "r");
	
	ifstream fptr2;
	fptr2.open(argv[2]);	//I wanted to read the Messages.txt with C++ file operations
	
	Network network;
	network.create_network();
	
	#define CC network.central_controller
	
	if(fptr1 == NULL){
		cout << "One or more files cannot be opened :(" << endl;
		return 0;
	}else{
		char type[3];
		int number = 0, target = 0;
		while(fscanf(fptr1, "%s %d %d\n", type, &number, &target) == 3){
			Base_Station *targetptr = network.find_station(CC, target);
			if(strncmp(type, "BS", 2) == 0){
				Base_Station *add = network.create_station(number);
				network.add_station(targetptr, add);
			}else if(strncmp(type, "MH", 2) == 0){
				Mobile_Phone *add = new Mobile_Phone;
				add->phone_number = number;
				add->connected_station = target;
				add->next = NULL;
				network.add_phone(targetptr, add);
			}
		}
		fclose(fptr1);
	}
	if(!fptr2.is_open()){
		cout << "One or more files cannot be opened :(" << endl;
		return 0;
	}else{
		string scanned;	//This is the buffer that stores the line of the file
		int number = 0;	//This is the phone number ("message5>26", number is 26)
		while(getline(fptr2, scanned)){
			char message[SIZE];	//This the message part of the line
			unsigned int i;
			for(i = 0; scanned[i] != '>'; i++){	//Copy the characters until '>'
				message[i] = scanned[i];
			}
			//	 scanned[i] = '>' now
			message[i] = '\0';	//Make "message" a string
			char num_str[10];
			int index = 0;
			for(i = i+1; i < scanned.size(); i++){	//Copy the digits until '\0'
				num_str[index] = scanned[i];
				index++;
			}
			//	index == size(num_str) now
			num_str[index] = '\0';	//Make "num_str" a string
			number = atoi(num_str);	//Convert the string to an integer
			
			//Create a phone that stores the message and has an ID "number"
			Mobile_Phone *add = new Mobile_Phone;
			add->phone_number = number;
			add->message = new char[SIZE];
			copy_str(message, add->message);
			
			cout << "Traversing:";
			Mobile_Phone *target = network.find_phone(CC, add);
			cout << endl;
			if(target == NULL){
				cout << "Can not be reached the mobile host mh_" << add->phone_number << " at the moment";
			}else{
				target->message = add->message;
				cout << "Message:" << target->message << " To:";
				network.path_to_cc(target);
				cout << "mh_" << add->phone_number;
			}
			cout << endl;
			number = 0;
		}
		fptr2.close();
	}
	//network.display(network.central_controller);
	network.close_network(CC);
	return 0;
}