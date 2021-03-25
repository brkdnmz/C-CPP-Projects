#include <iostream>
#include <fstream> //For FILE I/O
#include <string>  //std::string
#include <cstring> //strlen()
#include <ctime>   //Used it for clock(), useless now
#include <chrono>  //High precision clock
#include <iomanip> //setprecision()
#include <cassert> //assert() (Checked if output is sorted)
using namespace std;

typedef std::chrono::high_resolution_clock Clock;

int str_to_int(const char* s){ //My own string-to-int function
	int ret = 0;
	for (int i = 0; s[i] != '\0'; i++){
		ret = ret * 10 + s[i] - '0';
	}
	return ret;
}

/****				Class for input elements 			****/
class Sale{
private:
	string country, item_type, order_ID;
	int units_sold;
	double total_profit;
public:
	/* Constructors */
	Sale(){}
	Sale(const string& country, const string& item_type, const string& order_ID, const int& units_sold, const double& total_profit){
		this->country = country;
		this->item_type = item_type;
		this->order_ID = order_ID;
		this->units_sold = units_sold;
		this->total_profit = total_profit;
	}
	Sale(const Sale& otherSale){
		country = otherSale.country;
		item_type = otherSale.item_type;
		order_ID = otherSale.order_ID;
		units_sold = otherSale.units_sold;
		total_profit = otherSale.total_profit;
	}

	/* Getters */
	string get_country() const{
		return country;
	}
	string get_item_type() const{
		return item_type;
	}
	string get_order_ID() const{
		return order_ID;
	}
	int get_units_sold() const{
		return units_sold;
	}
	double get_total_profit() const{
		return total_profit;
	}

	/* Operator overloads */
	void operator=(const Sale& otherSale){
		/* Actually currently unnecessary, overloaded it for swapping but I did not swap objects */
		country = otherSale.country;
		item_type = otherSale.item_type;
		order_ID = otherSale.order_ID;
		units_sold = otherSale.units_sold;
		total_profit = otherSale.total_profit;
	}

	bool operator<(const Sale& otherSale) const{
		/* This is overloaded for comparison */
		//Below is the fastest way I found to compare two "std::string"s
		const string& first = country;
		const string& second = otherSale.country;
		int cmp = first.compare(second);
		if(cmp < 0) return true;
		if(cmp > 0) return false;
		//first == second
		//country == otherSale.country
		return total_profit > otherSale.total_profit; //Descending order of total profits
	}
	bool operator==(const Sale& otherSale) const{
		/* Overloaded for <= operator */
		const string& first = country;
		const string& second = otherSale.country;
		int cmp = first.compare(second);
		return cmp == 0 && total_profit == otherSale.total_profit;
	}
	bool operator<=(const Sale& otherSale) const{
		/* Overloaded for checking if sorted after quickSort */
		return *this < otherSale || *this == otherSale;
	}
	bool operator>(const Sale& otherSale) const{
		/* Overloaded because why not? */
		const string& first = country;
		const string& second = otherSale.country;
		int cmp = first.compare(second);
		if(cmp < 0) return false;
		if(cmp > 0) return true;
		//first == second
		//country == otherSale.country
		return total_profit < otherSale.total_profit; //Ascending order of total profits
	}
	bool operator>=(const Sale& otherSale) const{
		/* Overloaded because why not? */
		return !(*this < otherSale);
	}
	friend ifstream& operator>>(ifstream& in, Sale& sale); //Reading input in a clean way
	friend ostream& operator<<(ostream& out, const Sale& sale); //Writing into file in a clean way
};

ifstream& operator>>(ifstream& in, Sale& sale){
	getline(in, sale.country, '\t');
	getline(in, sale.item_type, '\t');
	getline(in, sale.order_ID, '\t');
	in >> sale.units_sold;
	in >> sale.total_profit;
	string newline;
	getline(in, newline, '\n');
	return in;
}
ostream& operator<<(ostream& out, const Sale& sale){
	out << sale.country << "\t";
	out << sale.item_type << "\t";
	out << sale.order_ID << "\t";
	out << sale.units_sold << "\t";
	out << sale.total_profit << "\n";
	return out;
}

template<typename T> 
void mySwap(T& first, T& second){
	/* My own swap function. Don't know why but it works slightly faster */
	T temp = first;
	first = second;
	second = temp;
}
int reOrder(Sale**& sales, int L, int R){
	//Index of the pivot is always R
	int leftBar = L, rightBar = L;
	while (rightBar < R){
		if (*sales[rightBar] < *sales[R]){ //less than pivot
			//Swapping needed
			//Swap pointers instead of objects themselves, considerable difference in terms of efficiency
			mySwap(sales[leftBar], sales[rightBar]);
			leftBar++;
		}
		rightBar++;
	}
	mySwap(sales[leftBar], sales[R]); //Place pivot in the correct place
	return leftBar; //Final index of the pivot
}
void quickSort(Sale**& sales, int L, int R){
	if (L >= R) return; //If there is less than two elements (one or zero) in the current interval
	int pivotFinalIndex = reOrder(sales, L, R); //Reordering [L, R] interval
	quickSort(sales, L, pivotFinalIndex - 1); //sort [L, pivot-1]
	quickSort(sales, pivotFinalIndex + 1, R); //sort [pivot+1, R]
}
int main(int arg_count, char** arguments){
	/****************************************************************************************

		PLEASE COMPILE WITH "-std=c++11" FLAG, 
		SINCE I USED HIGH PRECISION CLOCK (chrono LIBRARY) FOR MORE PRECISE TIME CALCULATION.

	****************************************************************************************/
	if(arg_count < 2){
		cerr << "Please enter the total number of sales to be sorted next time :)\n";
		exit(1);
	}
	if(arg_count > 2){
		cerr << "Please enter ONLY the total number of sales to be sorted next time :)\n";
		exit(1);
	}
	const char* N = arguments[1]; //total number of sales to be sorted (string form)
	int checker = 0; //Iterator, let's say
	while(N[checker] != '\0'){
		//Checking if there is a non-digit character in N
		const char& cur = N[checker];
		if(!('0' <= cur && cur <= '9')){
			cerr << "Please enter a non-negative integer next time :) (every character must be a decimal digit)\n";
			exit(1);
		}
		checker++;
	}
	if((strcmp(N, "1000000") > 0 && strlen(N) >= 7)  || strlen(N) > 7){ //N is an integer greater than 1'000'000
		cerr << "Please enter a non-negative integer <= 1'000'000 next time :)\n";
		exit(1);
	}
	const int TOTAL = str_to_int(N); //Converting N to int
	ifstream salesFile("sales.txt");
	if (!salesFile.is_open()){
		cerr << "Input file couldn't be opened, sorry!\n";
		exit(1);
	}
	string firstLine;
	getline(salesFile, firstLine); //First line of the file, denoting attributes' types

	/* This is important */
	//I created an array of Sale pointers instead of Sale objects
	//So that when swapping, I swap the pointers
	//This is more efficient because the size of the object can be big (it contains strings)
	Sale** sales = new Sale*[TOTAL];
	for (int i = 0; i < TOTAL; i++){
		sales[i] = new Sale;
		salesFile >> *sales[i];
	}
	salesFile.close();
	Clock::time_point start = Clock::now(); //Starting time of quickSort
	quickSort(sales, 0, TOTAL - 1); //Initial interval (whole interval) is [0, total - 1]
	Clock::time_point end = Clock::now();	//Ending time of quickSort
	chrono::duration<double, milli> timeElapsed = end - start;
	cout << fixed << setprecision(5) << "Time elapsed (ms): " << timeElapsed.count() << "\n";
	for(int i = 0; i < TOTAL - 1; i++){
		/* Check if quickSort function worked properly (sales are sorted) */
		/* If there is an error, program is going to be terminated */
		assert(*sales[i] <= *sales[i+1]);
	}
	ofstream sortedFile("sorted.txt");
	if (!sortedFile.is_open()){
		cerr << "Output file couldn't be opened, sorry!\n";
		exit(1);
	}
	sortedFile << fixed << setprecision(2); //Write only two digits after decimal point
	sortedFile << firstLine << "\n";
	for(int i = 0; i < TOTAL; i++){
		sortedFile << *sales[i]; //Have overloaded the operator <<
	}
	sortedFile.close();
	for (int i = 0; i < TOTAL; i++){
		delete sales[i];
	}
	delete[] sales;
	return 0;
}