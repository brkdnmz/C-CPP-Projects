/*********************
Name: Berke Dönmez
ID: 150170045
*********************/

#include <string>
using namespace std;
class node{
private:
	node *next;
	string name;//Ingredient name / drink name
	int count;//Only for the drinks, total number of drink
public:
	node(const string&);
	string get_name(){
		return name;
	}
	int get_count(){
		return count;
	}
	void incr_count(){//Getter and setter methods are written inline
		count++;
	}
	node* go_next(){
		return this->next;
	}
	void set_next(node* const& new_node){
		next = new_node;
	}
	//Default destructor is fine since when we delete an object of type node, it is enough.
	//"node" does not contain anything allocated dynamically.
};

class Pizza{
private:
	string name;
	string size;
	node *ingr_head;
	string crust_type;
	Pizza *next;//Think "Pizza" as another type of node
public:
	Pizza();
	Pizza(const string&, const string&, const int&);
	Pizza(const Pizza&);
	string get_size(){
		return size;
	}
	string get_name(){
		return name;
	}
	string get_crust_type(){
		return crust_type;
	}
	node* get_ingr_head(){//Getter and setter methods
		return ingr_head;
	}
	Pizza* go_next(){
		return next;
	}
	void set_next(Pizza* const& new_pizza){
		next = new_pizza;
	}
	void printPizza();
	void add_ingr(const string&);
	void erase_ingr(const string&);
	~Pizza();
};

class Order{
private:
	string customer;
	Pizza *pizza_head;
	node *drink_head;
	Order *next;//Think "Order" as another type of node
public:
	Order(const string&, Pizza* const&, node* const&);
	Order(const string&, Pizza* const&);
	string get_customer(){
		return customer;
	}
	Pizza* get_pizza_head(){
		return pizza_head;
	}
	node* get_drink_head(){//Getter and setter methods
		return drink_head;
	}
	Order* go_next(){
		return next;
	}
	void set_next(Order* const& new_order){
		next = new_order;
	}
	double getPrice();
	void printOrder();
	~Order();
};
class OrderList{
private:
	int order_count;
	Order *head;
public:
	OrderList();
	void takeOrder();
	void listOrders();
	void deliverOrders();
	Order* searchOrder(const string&);
	void printOrder(Order* const&);
	void push_order(Order* const&);
	void remove_order(const string&);
	node* search_drink(node* const&, node* const&);
	~OrderList();
};