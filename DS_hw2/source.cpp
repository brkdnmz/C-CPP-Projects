/* @Author
Student Name: Berke Dönmez
Student ID: 150170045
Date: 10.12.2019
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

#define SIZE 1000

struct Mobile_Phone{
	char *message;
	int phone_number;
	int connected_station;
	Mobile_Phone *next;
	void receive_message(Mobile_Phone *phone, char *message);
};

struct Base_Station{
	int station_number;
	Mobile_Phone *phone_head;
	Base_Station *station_head;
	Base_Station *next;
	Base_Station *top;//This points to the parent station
};

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

int main(int arg_count, char **argv){
	FILE *fptr1, *fptr2;
	fptr1 = fopen(argv[1], "r");
	fptr2 = fopen(argv[2], "r");
	
	Network network;
	network.create_network();
	#define CC Central_Controller
	Base_Station *CC = network.central_controller;
	
	if(fptr1 == NULL || fptr2 == NULL){
		cout<<"One or more files cannot be opened :("<<endl;
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
				network.add_phone(targetptr, add);
			}
		}
		fclose(fptr1);
		
		char scanned[SIZE];
		number = 0;
		while(fgets(scanned, sizeof(scanned), fptr2) != NULL){
			/*	scanned[str_length(scanned) - 1] == '\n' now. Actually the next character is also '\n' but I do not know why */
			scanned[str_length(scanned) - 1] = '\0';
			char message[SIZE];
			int i;
			for(i = 0; scanned[i] != '>'; i++){
				message[i] = scanned[i];
			}
			/*	 scanned[i] = '>' now	*/
			message[i] = '\0';
			char num_str[10];
			int index = 0;
			for(i = i+1; scanned[i] != '\0'; i++){
				num_str[index] = scanned[i];
				index++;
			}
			/*	index == size(num_str) now	*/
			num_str[index] = '\0';
			number = atoi(num_str);
			
			Mobile_Phone *add = new Mobile_Phone;
			add->phone_number = number;
			add->message = new char[SIZE];
			copy_str(message, add->message);
			
			cout<<"Traversing:";
			Mobile_Phone *target = network.find_phone(CC, add);
			cout<<endl;
			if(target == NULL){
				cout<<"Can not be reached the mobile host mh_"<<add->phone_number<<" at the moment";
			}else{
				//target->message = new char[1000];
				target->message = add->message;
				cout << "Message:" << target->message << " To:";
				network.path_to_cc(target);
				cout<<"mh_"<<add->phone_number;
			}
			cout<<endl;
			number = 0;
		}
	}
	//network.display(network.central_controller);
	network.close_network(CC);
	fclose(fptr2);
	return 0;
}