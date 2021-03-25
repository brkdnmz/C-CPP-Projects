/*
Name: Berke Dönmez
ID: 150170045
*/

#include <iostream>
#include "classes.hpp"


/*******************************************FUNCTIONS*************************************/

Person::Person(){//I did not want it to be empty
	if (!name.empty()){
		name.clear();
	}
	if (!surname.empty()){
		surname.clear();
	}
}

Person::Person(const string& name, const string& surname){
	this->name = name;
	this->surname = surname;
}

const string& Person::getName() const{
	return name;
}

const string& Person::getSurname() const{
	return surname;
}

Owner::Owner(){//I did not want it to be empty :D
	ownership = 0;
}

Owner::Owner(const string& name, const string& surname) : Person(name, surname){
	ownership = 0;
}

void Owner::adjust_ownership(const double& percentage){//Indirect adjustment
	ownership = percentage;
}

const double& Owner::get_ownership() const{//Indirect access
	return ownership;
}
Courier::Courier(const string& name, const string& surname, const string& vehicle) : Person(name, surname){
	this->vehicle = vehicle;
	if (vehicle == "car"){
		capacity = 200;
	}
	else if (vehicle == "motorcycle"){
		capacity = 35;
	}
	else if (vehicle == "bicycle"){
		capacity = 10;
	}
	else{//It is still created but with no capacity, with an error message
		capacity = 0;
		cout << "A courier cannot use that vehicle, sir / madam!" << endl;
	}
}

const string& Courier::getVehicle() const{//Indirect access
	return vehicle;
}

const int& Courier::getCapacity() const{//Indirect access
	return capacity;
}

const bool Courier::operator==(const Courier& courier) const{
	const string& thisName = this->getName();
	const string& otherName = courier.getName();
	if (thisName != otherName) return false;

	const string& thisSurname = this->getSurname();
	const string& otherSurname = courier.getSurname();
	if (thisSurname != otherSurname) return false;

	if (vehicle != courier.vehicle) return false;

	return true;//If not returned false yet, it should return true because everything is same
}

CourierNode::CourierNode(const Courier& courier){//Allocates memory for a new object
	this->courier = new Courier(courier.getName(), courier.getSurname(), courier.getVehicle());
	next = NULL;
}

CourierNode* CourierNode::getNext(){//Indirect access
	return next;
}

void CourierNode::setNext(CourierNode* const new_node){//Indirect setting
	next = new_node;
}

Courier* CourierNode::getCourier(){//Indirect access
	return courier;
}

CourierNode::~CourierNode(){
	delete courier;//After this, destructor of the "Courier" class is called
}

Business::Business(const string& name, const string& address, Owner* const owner_arr, const int& size){
	this->name = name;
	this->address = address;
	//I copied "owner_arr" to my own "owners" array for safety
	//(a possible change in "owner_arr" in main() will not affect my "owners" array)
	owners = new Owner[size];
	const double WHOLE = 100;
	for (int i = 0; i < size; i++){
		owners[i] = owner_arr[i];
		owners[i].adjust_ownership(WHOLE / size);
	}
	owner_count = size;
	head = NULL;
}

void Business::hire_courier(const Courier& courier){
	if (head == NULL){
		head = new CourierNode(courier);
		return;
	}

	CourierNode *goEnd = head;
	CourierNode *tail = goEnd;
	while (goEnd){
		tail = goEnd;
		goEnd = goEnd->getNext();
	}//Go to the end of the linked list
	//goEnd == last node's next == NULL
	//tail == last node
	CourierNode *tmp = new CourierNode(courier);
	tail->setNext(tmp);
}

void Business::fire_courier(const Courier& courier){
	if (head == NULL){
		cout << "Courier not found" << endl;
		return;
	}
	if (*(head->getCourier()) == courier){//If courier is in "head", head must shift to head->next
		CourierNode *tmp = head;
		head = head->getNext();
		delete tmp;
		return;
	}
	CourierNode *search = head;
	CourierNode *tail = search;
	while (search){
		Courier *compare = search->getCourier();
		if (*compare == courier){//If it is found, rearrange the linked list, delete it and return
			tail->setNext(search->getNext());
			delete search;
			return;
		}
		tail = search;
		search = search->getNext();
	}
	//search == NULL meaning that it is not found
	cout << "Courier not found" << endl;
}

void Business::list_couriers() const{
	CourierNode *travel = head;
	while (travel){
		const Courier* courier = travel->getCourier();
		const string& name = courier->getName();
		const string& surname = courier->getSurname();
		const string& vehicle = courier->getVehicle();
		cout << name << " " << surname << " " << vehicle << endl;
		travel = travel->getNext();
	}
}

void Business::list_owners() const{
	for (int i = 0; i < owner_count; i++){
		cout << owners[i].getName() << " " << owners[i].getSurname() << " " << owners[i].get_ownership() << endl;
	}
}

int Business::calculate_shipment_capacity() const{
	int total = 0;
	CourierNode *travel = head;
	while (travel){
		const Courier* courier = travel->getCourier();
		total += courier->getCapacity();
		travel = travel->getNext();
	}
	return total;
}

void Business::operator()() const{
	cout << name << " " << address << endl;
	list_owners();
	list_couriers();
}

const Courier& Business::operator[](int index){
	//If an invalid index is entered, i still return first/last courier in the linked list with printing the error message
	//PDF does not say that I should not do this
	if (index < 0){
		cout << "Invalid index" << endl;
		index = 0;//Avoiding possible errors(index < 0)
	}
	CourierNode *reach = head;
	while (index && reach){//index == 0 means that we reached the target
		reach = reach->getNext();
		index--;
		if (reach->getNext() == NULL){//Avoiding possible errors(index >= size)
			break;
		}
	}
	if (index > 0){
		cout << "Invalid index" << endl;
	}
	const Courier& courier = *(reach->getCourier());
	return courier;
}

Business::~Business(){
	delete[] owners;
	CourierNode *eraser = head;
	while (head){
		eraser = head;
		head = head->getNext();
		delete eraser;
	}
}

/*******************************************END OF FUNCTIONS*************************************/

int main(){
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // Create owners of the business
    //Constructor parameters: name, surname
    Owner o1 = Owner("Owner_name1", "surname1");
    Owner o2 = Owner("Owner_name2", "surname2");
    Owner* owner_arr = new Owner[2];
    owner_arr[0] = o1;
    owner_arr[1] = o2;
    //Crete the business itself
    //Constructor parameters: name, address, owner_array, number_of_owners
    Business atlas = Business("Atlas", "Maslak Istanbul/Turkey", owner_arr, 2);
    
    // Print owner info to screen: name, surname, ownership
    atlas.list_owners();

    // Add new employees
    // Constructor parameters: name, surname, vehicle_type
    Courier c1 = Courier("Courier", "surname1", "car");
    Courier c2 = Courier("Courier", "surname2", "motorcycle");
    Courier c3 = Courier("Courier", "surname3", "motorcycle");    

    atlas.hire_courier(c1);
    atlas.hire_courier(c2);
    atlas.hire_courier(c3);


    // Print business info to screen
    // Name, address, owners, couriers
    atlas();

    // Get courier with index 1 and remove it from the list/array
    Courier cour = atlas[1];
    atlas.fire_courier(cour);

    // Print remaining couriers
    atlas.list_couriers();
    
    // Print current maximum shipment capacity
    std::cout << atlas.calculate_shipment_capacity() << std::endl;
	delete[] owner_arr;//This should be written due to my implementation
	system("PAUSE");
    return 0;
}