/*********************
Name: Berke Dönmez
ID: 150170045
*********************/

#include "classes.h"
#include <iostream>
using namespace std;//Visual studio once detected an error "the call of cin is ambiguous" though it was already written in the header file, so I wrote it here too
//string library is included in "classes.h"
node::node(const string& name){
	this->name = name;
	count = 1;
	next = NULL;
}

Pizza::Pizza(){
	size = "medium";
	crust_type = "normal";
	name = "margherita";
	ingr_head = NULL;
	add_ingr("mozarella");
	next = NULL;
}

//FOR CONVENIENCE
string chicken[] = { "mozarella", "chicken", "mushroom", "corn", "onion", "tomato" };
string broccoli[] = { "mozarella", "broccoli", "pepperoni", "olive", "corn", "onion" };
string sausage[] = { "mozarella", "sausage", "tomato", "olive", "mushroom", "corn" };
//FOR CONVENIENCE

const int INGR_COUNT = 6;//Total number of ingredients is always 6
Pizza::Pizza(const string& size, const string& crust_type, const int& pizza_type){
	this->size = size;
	this->crust_type = crust_type;
	if (pizza_type == 1)
		name = "chicken";
	else if (pizza_type == 2)
		name = "broccoli";
	else if (pizza_type == 3)
		name = "sausage";
	
	//Adding ingredients
	for (int cur_ingr = 0; cur_ingr < INGR_COUNT; cur_ingr++){
		if (pizza_type == 1)
			add_ingr(chicken[cur_ingr]);
		else if (pizza_type == 2)
			add_ingr(broccoli[cur_ingr]);
		else if (pizza_type == 3)
			add_ingr(sausage[cur_ingr]);
	}
	next = NULL;
}
Pizza::Pizza(const Pizza& pizza){
	//Copying phase
	this->name = pizza.name;
	this->crust_type = pizza.crust_type;
	this->size = pizza.size;
	node *copy = pizza.ingr_head;
	while (copy){
		add_ingr(copy->get_name());
		copy = copy->go_next();
	}
	//Copying ended
	
	cout << "Please enter the number of the ingredient you want to remove from your pizza.\n";
	node *show = ingr_head;
	int num = 1;//Number for listing ingredients
	while (show){//Printing all the ingredients of the pizza
		cout << num << ". " << show->get_name() << endl;
		show = show->go_next();
		num++;
	}
	cout << "Press 0 to save it.\n";
	
	//Ingredient deletion phase
	int del = -1;
	cin >> del;
	while (del != 0){
		string erase_this;
		if (pizza.name == "chicken"){
			erase_this = chicken[del - 1];//Storing ingredients is useful at this point
		}
		else if (pizza.name == "broccoli"){
			erase_this = broccoli[del - 1];
		}
		else if (pizza.name == "sausage"){
			erase_this = sausage[del - 1];
		}
		erase_ingr(erase_this);
		cin >> del;
	}
	//Ingredient deletion ended
	
	next = NULL;
}
void Pizza::add_ingr(const string& ingredient){
	node *new_ingr = new node(ingredient);
	node *goto_end = ingr_head;//Pointer whose job is to go to the end of the linked list
	
	if (ingr_head == NULL){//If there is no ingredient yet
		ingr_head = new_ingr;
		return;
	}
	while (goto_end->go_next()){//Condition is equal to "goto_end->next != NULL"
		goto_end = goto_end->go_next();
	}
	goto_end->set_next(new_ingr);//Adding the new ingredient to the end
}
void Pizza::erase_ingr(const string& ingredient){
	node *search = ingr_head;
	node *tail = search;
	if (ingr_head->get_name() == ingredient){//If the ingredient is the head of the linked list
		ingr_head = ingr_head->go_next();
		delete search;
		return;
	}
	while (search != NULL && search->get_name() != ingredient){//Search until finding the ingredient
		tail = search;
		search = search->go_next();
	}
	if (search == NULL){//If the ingredient is already deleted, it cannot find the element in the linked list
		cout << "Ingredient has already been deleted.\n";
		return;
	}
	tail->set_next(search->go_next());//tail -> search -> search->next becomes tail -> search->next
	delete search;
}
void Pizza::printPizza(){
	if (name == "broccoli"){
		cout << "Broccoli Pizza";
	}
	else if (name == "chicken"){
		cout << "Chicken Pizza";
	}
	else if (name == "sausage"){
		cout << "Sausage Pizza";
	}
	cout << "(";
	node *show_ingr = ingr_head;
	while (show_ingr){//Printing ingredients in a single line
		cout << show_ingr->get_name() << ", ";
		show_ingr = show_ingr->go_next();
	}
	cout << ")\n";
	cout << "size: " << size << ", crust: " << crust_type << endl << endl;
}
Pizza::~Pizza(){
	//An ordinary linked list destroyer
	node *eraser = ingr_head;
	while (ingr_head){
		eraser = ingr_head;
		ingr_head = ingr_head->go_next();
		delete eraser;
	}
}

Order::Order(const string& name, Pizza* const& pizza, node* const& drink){
	//There is not much to explain
	customer = name;
	pizza_head = pizza;
	drink_head = drink;
	next = NULL;
}
Order::Order(const string& name, Pizza* const& pizza){
	customer = name;
	pizza_head = pizza;
	drink_head = NULL; //If no drink is ordered, this must be done in order to prevent bugs
	next = NULL;
}
double Order::getPrice(){
	double result = 0;//Double because of the promotion code && fruit juice
	Pizza *pizza_traveler = pizza_head;
	while (pizza_traveler){//Getting the prices of all the pizzas
		string check = pizza_traveler->get_size();
		if (check == "small"){
			result += 15;
		}
		else if (check == "medium"){
			result += 20;
		}
		else if (check == "big"){
			result += 25;
		}
		pizza_traveler = pizza_traveler->go_next();
	}
	node *drink_traveler = drink_head;
	while (drink_traveler){//Getting the prices of all the drinks
		string check = drink_traveler->get_name();
		int count = drink_traveler->get_count();
		if (check == "cola"){
			result += 4 * count;
		}
		else if (check == "soda"){
			result += 2 * count;
		}
		else if (check == "ice tea"){
			result += 3 * count;
		}
		else if (check == "fruit juice"){
			result += 3.5 * count;
		}
		drink_traveler = drink_traveler->go_next();
	}
	return result;
}
void Order::printOrder(){
	cout << "Name: " << customer << endl << endl;
	Pizza *pizza_traveler = pizza_head;
	while (pizza_traveler){//Printing all the pizzas in the order (While pizza_traveler != NULL)
		string name = pizza_traveler->get_name();
		if (name == "broccoli"){
			cout << "Broccoli Pizza";
		}
		else if (name == "chicken"){
			cout << "Chicken Pizza";
		}
		else if (name == "sausage"){
			cout << "Sausage Pizza";
		}
		cout << "(";
		node *ingr = pizza_traveler->get_ingr_head();
		while (ingr){//Printing all the ingredients of current pizza (While ingr != NULL)
			cout << ingr->get_name() << ", ";
			ingr = ingr->go_next();
		}
		cout << ")" << endl;
		cout << "size: " << pizza_traveler->get_size() << ", ";
		cout << "crust: " << pizza_traveler->get_crust_type() << endl << endl;
		pizza_traveler = pizza_traveler->go_next();
	}
	node *drink_traveler = drink_head;
	while (drink_traveler){//Printing all the drinks in the order
		cout << drink_traveler->get_count() << " " << drink_traveler->get_name() << ", ";
		drink_traveler = drink_traveler->go_next();
	}
	cout << endl;
}
Order::~Order(){
	//An order contains two linked lists, pizza list and drink list
	Pizza *pizza_eraser = pizza_head;
	while (pizza_head){//Deleting pizzas
		pizza_eraser = pizza_head;
		pizza_head = pizza_head->go_next();
		delete pizza_eraser;
	}
	node *drink_eraser = drink_head;
	while (drink_head){//Deleting drinks
		drink_eraser = drink_head;
		drink_head = drink_head->go_next();
		delete drink_eraser;
	}
}

OrderList::OrderList(){
	order_count = 0;
	head = NULL;
}
//Another array for convenience
string drinks[] = { "cola", "soda", "ice tea", "fruit juice" };
void OrderList::takeOrder(){
	order_count++; //Well, I did not use it at all, but it's not hazardous
	cout << "Pizza Menu\n";
	cout << "1. Chicken Pizza (mozarella, chicken, mushroom, corn, onion, tomato)\n";
	cout << "2. Broccoli Pizza (mozarella, broccoli, pepperoni, olive, corn, onion)\n";
	cout << "3. Sausage Pizza (mozarella, sausage, tomato, olive, mushroom, corn)\n";
	cout << "0. Back to main menu\n";
	string selected_str;
	cin >> selected_str;
	while (selected_str != "0" && selected_str != "1" && selected_str != "2" && selected_str != "3"){//Preventing probable bugs with checking the input
		cout << "Please select a valid choice: 0, 1, 2, 3.\n";
		cin >> selected_str;
	}
	int selected = selected_str[0] - '0'; //Converting string, which is going to consist of a single digit, to integer
	if (selected == 0) return; //Do nothing and return to the main menu
	cout << "Select size: small (15 TL), medium (20 TL), big (25 TL)\n";
	string size;
	cin >> size;
	while (size != "small" && size != "medium" && size != "big"){//Same input control stuff
		cout << "Please select a valid size: small, medium, big.\n";
		cin >> size;
	}
	cout << "Select crust type: thin, normal, thick\n";
	string crust_type;
	cin >> crust_type;
	while (crust_type != "thin" && crust_type != "normal" && crust_type != "thick"){//Same input control stuff
		cout << "Please select a valid crust type: thin, normal, thick.\n";
		cin >> crust_type;
	}
	cout << "Enter the amount (Please enter an integer):";
    int amount = 0;
	cin >> amount;
	while (amount <= 0){//... and, as expected, same input control stuff
		cout << "Amount must be positive :) Please enter again.\n";
		cin >> amount;
	}
	Pizza *first_pizza = new Pizza(size, crust_type, selected);//The first pizza of the order, which is also the head of the linked list
	Pizza *combine_pizzas = first_pizza;//A pointer which combines the pizzas in a linked list
	for (int i = 1; i < amount; i++){
		Pizza *new_pizza = new Pizza(*first_pizza);//At this step, copy constructor is called. Thus, customer is asked to select pizzas, drinks etc.
		combine_pizzas->set_next(new_pizza);//This pointer always points to the last pizza.
		combine_pizzas = combine_pizzas->go_next();//If new pizza is inserted, I should update the combiner pointer
	}
	cout << "Please choose a drink:\n";
	cout << "0. no drink\n";
	cout << "1. cola 4 TL\n";
	cout << "2. soda 2 TL\n";
	cout << "3. ice tea 3 TL\n";
	cout << "4. fruit juice 3.5 TL\n";
	cout << "Press -1 for save your order\n";
	string drink_str;
	cin >> drink_str;
	while (drink_str != "-1" && drink_str != "0" && drink_str != "1" && drink_str != "2" && drink_str != "3" && drink_str != "4"){//Input control
		cout << "Please choose a valid option: -1, 0, 1, 2, 3, 4.\n";
		cin >> drink_str;
	}
	int drink;
	if (drink_str == "-1") drink = -1;//The only case which must be treated differently
	else drink = drink_str[0] - '0';//Converting string, which consists of a single digit, to integer

	node *first_drink = NULL;//Head of the drink list
	if (drink != 0 && drink != -1){
		first_drink = new node(drinks[drink - 1]);
		node *combine_drinks = first_drink;;//Drink combiner, like the pizza one
		while (1){
			cin >> drink_str;
			while (drink_str != "-1" && drink_str != "0" && drink_str != "1" && drink_str != "2" && drink_str != "3" && drink_str != "4"){//Input control
				cout << "Please choose a valid option: -1, 0, 1, 2, 3, 4.\n";
				cin >> drink_str;
			}
			if (drink_str == "-1") drink = -1;
			else drink = drink_str[0] - '0';
			if (drink == -1){//Just break to save
				break;
			}
			else if (drink == 0){//If this appears, I should delete all the drinks taken before, because you wanted us to do so
				//An ordinary linked list erasing operation
				node *eraser = first_drink;
				while (first_drink){
					eraser = first_drink;
					first_drink = first_drink->go_next();
					delete eraser;
				}
				break;
			}
			node *new_drink = new node(drinks[drink - 1]);
			node *exists = search_drink(first_drink, new_drink);//Find out whether this drink is ordered before
			if (exists == NULL){//If this is the first time it is ordered, add it to the end of the linked list
				combine_drinks->set_next(new_drink);
				combine_drinks = combine_drinks->go_next();
			}
			else{//Else, increment the number of the drink
				exists->incr_count();
			}
		}
	}
	cout << "Please enter your name:\n";
	cin.ignore(100, '\n');//Getline stops if it reads a newline, so I should ignore the incoming newline(s)
	string name;
	getline(cin, name);//If the name consists of two or more separate parts separated with spaces, I should use getline
	Order *to_print;//A pointer which is used to send argument to the printOrder function
	if (first_drink == NULL){//Constructor type changes depending on this condition
		Order *new_order = new Order(name, first_pizza);
		push_order(new_order);
		to_print = new_order;
	}
	else{
		Order *new_order = new Order(name, first_pizza, first_drink);
		push_order(new_order);
		to_print = new_order;
	}
	cout << "Your order is saved, it will be delivered when it is ready...\n";
	printOrder(to_print);
}

void OrderList::listOrders(){
	if (head == NULL){//If there isn't any customer waiting
		cout << "\nempty\n\n";
		return;
	}
	Order *travel = head;
	int num = 1;//Order number in the list
	while (travel){//Printing all the orders one by one
		cout << num << endl;
		printOrder(travel);
		travel = travel->go_next();
		num++;
	}
}
void OrderList::deliverOrders(){
	listOrders();
	if (head == NULL) return;
	cout << "Please write the customer name in order to deliver his/her order: ";
	string name;
	cin.ignore(100, '\n');
	getline(cin, name);
	Order *deliver = searchOrder(name);
	if (deliver == NULL){//If there isn't any customer whose name is the entered name
		cout << "Wrong name\n";;
		return;
	}
	cout << "Following order is delivering...\n";
	printOrder(deliver);
	double price = deliver->getPrice();
	cout << "Total price: " << price << endl;
	cout << "Do you have a promotion code? (y/n)\n";
	char choice;
	cin >> choice;
	while(choice == 'y'){
		cout << "Please enter your code: ";
		string code;
		cin.ignore(100, '\n');
		getline(cin, code);//Because the actual code is a string consists of words separated by spaces
		if (code == "I am a student"){
			cout << "Discounted price: " << price * 0.9 << endl;//10% discount == 0.9 * actual price
			break;
		}
		else{
			cout << "Promotion code not accepted.\n";
			cout << "Do you have a promotion code? (y/n)\n";
		}
		cin >> choice;
	}
	remove_order(name);
	cout << "The order is delivered successfully\n\n";
}
Order* OrderList::searchOrder(const string& name){
	Order *search = head;
	while (search != NULL && search->get_customer() != name){
		search = search->go_next();
	}
	return search;
}
void OrderList::remove_order(const string& name){
	order_count--;//If an order is being delivered, number of orders must be decremented
	Order *search = head;
	Order *tail = search;
	if (head->get_customer() == name){
		head = head->go_next();
		delete search;
		return;
	}
	while (search != NULL && search->get_customer() != name){
		tail = search;
		search = search->go_next();
	}
	tail->set_next(search->go_next());
	delete search;
}
void OrderList::printOrder(Order* const& order){
	cout << "------------\n";
	cout << "Name: " << order->get_customer() << endl;
	cout << endl;
	Pizza *show_pizzas = order->get_pizza_head();
	while (show_pizzas){//Print all the pizzas of the order
		show_pizzas->printPizza();
		show_pizzas = show_pizzas->go_next();
	}
	node *show_drinks = order->get_drink_head();
	while (show_drinks){//Print all the drinks
		cout << show_drinks->get_count() << " " << show_drinks->get_name() << ", ";
		show_drinks = show_drinks->go_next();
	}
	cout << endl;
	cout << "------------\n";

}
node* OrderList::search_drink(node* const& head, node* const& drink){//Search the given drink, find whether it exists or not
	node *search = head;
	while (search != NULL && search->get_name() != drink->get_name()){
		search = search->go_next();
	}
	return search;
}
void OrderList::push_order(Order* const& order){
	//Go to the end, add the given order to the end
	Order *goto_end = head;
	if (head == NULL){//If there are no orders yet
		head = order;
		return;
	}
	while (goto_end->go_next()){
		goto_end = goto_end->go_next();
	}
	goto_end->set_next(order);
}
OrderList::~OrderList(){
	//An ordinary linked list eraser
	Order *eraser = head;
	while (head){
		eraser = head;
		head = head->go_next();
		delete eraser;
	}
}
string main_menu(){
	cout << "Welcome to Unicorn Pizza!\n";
	cout << "1. Add an order\n";
	cout << "2. List orders\n";
	cout << "3. Deliver order\n";
	cout << "4. Exit\n";
	cout << "Choose what to do: ";
	string choice;
	cin >> choice;
	return choice;
}
int main(){
	string choice_str = main_menu();
	while (choice_str != "1" && choice_str != "2" && choice_str != "3" && choice_str != "4"){//Input control
		cout << "Please enter a valid choice: 1, 2, 3, 4.\n";
		cin >> choice_str;
	}
	int choice = choice_str[0] - '0';//String of single digit to int conversion
	if (choice == 4){
		cout << "Goodbye...\n";
		return 0;
	}
	OrderList list;
	while (choice != 4){
		if (choice == 1){
			list.takeOrder();
		}
		else if (choice == 2){
			list.listOrders();
		}
		else if (choice == 3){
			list.deliverOrders();
		}
		choice_str = main_menu();
		while (choice_str != "1" && choice_str != "2" && choice_str != "3" && choice_str != "4"){//Input control
			cout << "Please enter a valid choice: 1, 2, 3, 4.\n";
			cin >> choice_str;
		}
		choice = choice_str[0] - '0';
	}
	cout << "Goodbye...\n";
	return 0;
}