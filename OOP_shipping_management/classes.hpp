/*
Name: Berke Dönmez
ID: 150170045
*/

#include <string>
using namespace std;//To avoid writing std::string

class Person{
private:
	string name;
	string surname;
public:
	Person();
	Person(const string& name, const string& surname);
	//Getters
	const string& getName() const;
	const string& getSurname() const;

	//Default destructor is sufficient
};

class Owner: public Person{
private:
	double ownership;
public:
	Owner();
	Owner(const string& name, const string& surname);

	//Getters and Setters
	void adjust_ownership(const double&);
	const double& get_ownership() const;

	//Default destructor is sufficient
};

class Courier : public Person{
private:
	string vehicle;
	int capacity;
public:
	Courier(const string& name, const string& surname, const string& vehicle);

	//Getters
	const string& getVehicle() const;
	const int& getCapacity() const;

	//Operator== overloaded
	const bool operator==(const Courier& courier) const;

	//Default destructor is sufficient
};

class CourierNode{//Linked list node of a courier
private:
	Courier *courier;
	CourierNode *next;
public:
	CourierNode(const Courier& courier);

	//Getters and Setters
	CourierNode* getNext();
	void setNext(CourierNode* const);
	Courier* getCourier();

	~CourierNode();
};

class Business{
private:
	string name;
	string address;
	Owner *owners;//Dynamic array for the owners
	int owner_count;
	CourierNode *head;//Linked list head of the couriers
public:
	Business(const string&, const string&, Owner* const, const int&);
	void hire_courier(const Courier&);
	void fire_courier(const Courier&);
	void list_couriers() const;
	void list_owners() const;
	int calculate_shipment_capacity() const;
	void operator()() const;
	const Courier& operator[](int);
	~Business();
};

