#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <cassert>
using namespace std;

typedef std::chrono::high_resolution_clock Clock;

int str_to_int(string s){
	int n = s.size();
	int ret = 0;
	for (int i = 0; i < n; i++){
		ret = ret * 10 + s[i] - '0';
	}
	return ret;
}

class Sale{
private:
	string country, item_type, order_ID;
	int units_sold;
	double total_profit;
public:
	Sale(){}
	Sale(string& country, string& item_type, string& order_ID, int& units_sold, double& total_profit){
		this->country = country;
		this->item_type = item_type;
		this->order_ID = order_ID;
		this->units_sold = units_sold;
		this->total_profit = total_profit;
	}
	Sale(const Sale& otherSale){
		country = otherSale.get_country();
		item_type = otherSale.get_item_type();
		order_ID = otherSale.get_order_ID();
		units_sold = otherSale.get_units_sold();
		total_profit = otherSale.get_total_profit();
	}

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
	bool operator=(const Sale& otherSale){
		country = otherSale.get_country();
		item_type = otherSale.get_item_type();
		order_ID = otherSale.get_order_ID();
		units_sold = otherSale.get_units_sold();
		total_profit = otherSale.get_total_profit();
	}
	bool operator<(const Sale& otherSale) const{
		if (country < otherSale.country) return true;
		if (country > otherSale.country) return false;
		//country == otherSale.country
		return total_profit > otherSale.total_profit; //Descending order of total profits
	}

	friend ifstream& operator>>(ifstream& in, Sale& sale);
	friend ostream& operator<<(ostream& out, Sale& sale);
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
ostream& operator<<(ostream& out, Sale& sale){
	out << "Country: " << sale.country << "\n";
	out << "Item Type: " << sale.item_type << "\n";
	out << "Order ID: " << sale.order_ID << "\n";
	out << "Units Sold: " << sale.units_sold << "\n";
	out << "Total Profit: " << sale.total_profit << "\n";
	return out;
}
int getRand(int left, int right){
	return left + rand() % (right - left + 1);
}
void quickSort(Sale** sales, int L, int R){
	if (L >= R) return;
	int pivotIndex = R;//getRand(L, R);
	Sale* pivot = sales[pivotIndex];
	//swap(sales[pivotIndex], sales[R]);
	int leftBar = L, rightBar = L;
	while (rightBar < R){
		if (*sales[rightBar] < *pivot){
			swap(sales[leftBar], sales[rightBar]);
			leftBar++;
		}
		rightBar++;
	}
	swap(sales[leftBar], sales[R]);
	quickSort(sales, L, leftBar - 1);
	quickSort(sales, leftBar + 1, R);
}
int main(int arg_count, char** arguments){
	cout << fixed;
	srand(time(NULL));
	Clock::time_point start = Clock::now();
	int total = 0;
	if (arg_count >= 2) total = str_to_int(arguments[1]); //don't forget exceptions
	ifstream salesFile("sales.txt");
	if (!salesFile.is_open()){
		cerr << "File couldn't be opened, sorry!\n";
		exit(1);
	}
	string useless; //Used to read useless stuff
	getline(salesFile, useless); //First line of the file, denoting attributes' types

	Sale** sales = new Sale*[total];
	for (int i = 0; i < total; i++){
		sales[i] = new Sale;
		salesFile >> *sales[i];
	}
	quickSort(sales, 0, total - 1);
	for (int i = 0; i < total; i++){
		delete sales[i];
	}
	delete[] sales;
	Clock::time_point end = Clock::now();
	chrono::duration<double> timeElapsed = end - start;
	cout << fixed << setprecision(10) << "Time elapsed (s): " << timeElapsed.count() << "\n";
}