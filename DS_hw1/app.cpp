/*
PLEASE DO NOT CHANGE THIS FILE 
*/

#include <iostream>	
#include <cstring>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <fstream>
#include <ctype.h>
#include <bits/stdc++.h> 
#include <chrono>
#include <thread>
#include <regex>
#define strnicmp strncasecmp 

#include "task_management_tool.h"

using namespace std;

typedef WorkPlan Datastructure;

Datastructure workplan;

void print_menu();
bool perform_operation(char);
void add_from_file(string filepath);
void display_schedule();
void remove_task();
void delay_a_task(); 
void delay_all_tasks_of_a_day();

Task * get_addresses(Task *,bool);
bool check_closing(Task *);
bool check_indentations(bool);

string filepath="appointments";
bool testing=false;

int main(int argc, char* argv[]){
	if (argc>1)
	{
		if(strnicmp(argv[1],"f",1) == 0) 
		{
			filepath=argv[2];
			ifstream in(filepath);
			testing=true;
			if(!in.is_open())
			{
				cout<<"ERROR: File could not been open"<<endl;
				return 1;
			}
			else
				cout<<"You have changed default file path as '"<<argv[2]<<"'"<<endl;
		}
		else
		{
			cout << "Error: You have entered an invalid choice" << endl; 
			cout << "Please try again"<<endl;
		}
	}		
	workplan.create();	
	bool end = false; 
	char choice; 	
	while (!end) { 
		print_menu(); 
		cin >> choice;
		end = perform_operation(choice); 
		}		
	workplan.close();
	this_thread::sleep_for(chrono::seconds(1));// to see the closing outputs
	system("clear");// make this line as comment if you are compiling on Windows
	//system("cls"); // make this line as comment if you are compiling on Linux or Mac
	return EXIT_SUCCESS;
}

void print_menu(){
	//system("clear");// make this line as comment if you are compiling on Windows
	//system("cls"); // make this line as comment if you are compiling on Linux or Mac
	cout << endl << endl;
	cout << "DR CELEBI APPOINTMENT APPLICATION" << endl;
	cout << "Choose an operation" << endl;
	cout << "A: Add From File" << endl;	
	cout << "D: Display Schedule" << endl;
	cout << "R: Remove Task" << endl;	
	cout << "G: Delay A Task" << endl;
	cout << "S: Delay All Tasks Of A Day Respecting Their Order and Priority" << endl;
	cout << "E: Exit" << endl;	
	cout << endl;
	cout << "Enter a choice {A,D,R,G,S,E}: ";
}

bool perform_operation(char choice){
	bool terminate=false;
	switch (choice) {
		case 'A': case 'a': 
			add_from_file(filepath);
			break; 
		case 'D': case 'd': 
			display_schedule();
			break;		 
		case 'R': case 'r': 
			remove_task();
			break;		
		case 'G': case 'g': 
			delay_a_task();
			break; 
		case 'S': case 's': 
			delay_all_tasks_of_a_day();
			break;
		case 'X':case 'x':
			{
			get_addresses(workplan.head,true);
			check_indentations(false);
			break;
			}
		case 'M':case 'm':
			{
			Task *adresses=get_addresses(workplan.head,false);	
			workplan.close();
			check_closing(adresses);
			break;
			}  
		case 'E': case 'e': 
			cout << "Are you sure you want to exit the program? (Y/N):";
			cin >> choice;
			if(choice=='Y' || choice=='y')
				terminate=true; 
			break; 
		default: 
			cout << "Error: You have entered an invalid choice" << endl; 
			cout << "Please try again {A, D, R, G, S, E}  :" ;
			cin >> choice;
			terminate = perform_operation(choice);
			break; 
	}
	return terminate;
}

void add_from_file(string filepath)
{	
	ifstream in(filepath);
	if(!in.is_open())
		cout<<"ERROR: File could not been open"<<endl;
	Task task;
	while(in.peek() != EOF )
	{		
		string name, day, time, priority;
		getline(in,name,',');
		getline(in,day,',');
		getline(in,time,',');
		getline(in,priority,'\n');		

		char cstr[name.size() + 1];
		name.copy(cstr, name.size() + 1);
		cstr[name.size()] = '\0';
		task.name=cstr;
		task.day=stoi(day);
		task.time=stoi(time);
		task.priority=stoi(priority);	
		cout<<task.name<<" is read from file"<<endl;
		workplan.add(&task);
	}
	in.close();
}

void remove_task(){
		int day=0,time=0;
	cout<<"Insert the day number and time of task that you want to remove"<<endl;
	cout << "Day number :";
	while (!(cin >> day))
	{
	    cin.clear(); // clear the error flags
	    cin.ignore(INT_MAX, '\n'); // discard the row
	    cout << "Invalid input! Try again\nDay number :";
	}

	cout << "Time (Just enter hour without minutes) :";
	while (!(cin >> time))
	{
	    cin.clear(); // clear the error flags
	    cin.ignore(INT_MAX, '\n'); // discard the row
	    cout << "Invalid input! Try again\nTime (Just enter hour without minutes) :";
	}
	//get the task that you want to remove
	Task *target=workplan.getTask(day, time);

	//remove task
	if (target!=NULL)
		workplan.remove(target);
}

void display_schedule()
{
	workplan.display(false,testing);
}

void delay_all_tasks_of_a_day()
{
	int day=0;
	cout<<"Insert the day number you want to delay"<<endl;
	cout << "Day number :";
	while (!(cin >> day))
	{
	    cin.clear(); // clear the error flags
	    cin.ignore(INT_MAX, '\n'); // discard the row
	    cout << "Invalid input! Try again\nDay number :";
	}
	workplan.delayAllTasksOfDay(day);
}

void delay_a_task()
{
	int day=0,time=0;
	cout<<"Insert the day number and time of the task that you want to delay"<<endl;
	cout << "Day number :";
	while (!(cin >> day))
	{
	    cin.clear(); // clear the error flags
	    cin.ignore(INT_MAX, '\n'); // discard the row
	    cout << "Invalid input! Try again\nDay number :";
	}

	cout << "Time (Just enter hour without minutes) :";
	while (!(cin >> time))
	{
	    cin.clear(); // clear the error flags
	    cin.ignore(INT_MAX, '\n'); // discard the row
	    cout << "Invalid input! Try again\nTime (Just enter hour without minutes) :";
	}
	
	if (day>0 and time>0)
	{ 
		//get the task that you want to suspend
		Task *oldone=workplan.getTask(day, time);
		Task *newone=new Task();
		if (oldone!=NULL)
		{
			newone->name=new char [strlen(oldone->name)];
			strcpy(newone->name,oldone->name);
			newone->priority=oldone->priority;
			workplan.checkAvailableNextTimesFor(oldone);
			cout<<workplan.getUsableDay()<<".day and "<<std::setw(2)<<workplan.getUsableTime()<<":00 is the first available day and hour for delaying the task "<<oldone->name<<" that is on "<<oldone->day<<".day at "<<std::setw(2)<< oldone->time <<":00"<<endl;
			newone->day=workplan.getUsableDay();
			newone->time=workplan.getUsableTime();
			workplan.remove(oldone);	
			workplan.add(newone);
		}		
		delete [] newone->name;
		delete newone;
			
	}
	else
		cout<<"Invalid input!"<<endl;
}

Task *get_addresses(Task *head,bool trick)
{
	Task *addresses=new Task[40];
	int i=0;
	string mainline="";
	if (head!=NULL)
	{
		Task *compeer;			
		Task *pivot=head;
		do
		{
			compeer=pivot;
			while(compeer!=NULL)
			{
				addresses[i]=*compeer;
				i++;	
				compeer=compeer->counterpart;
			}
			mainline+=pivot->name;
			pivot=pivot->next;
		}while(pivot!=head);
		if(trick)
			cout<<"MAINLINE: "<<mainline<<endl;
		return addresses;
	}
	else
		return addresses;		
}
bool check_closing(Task *addresses)
{
	cout<<"CLOSING FUNCTION IS ";
	string a = "GFK"; 
  
    	// Here b is an object of regex (regular expression) 
    	regex b("[a-zA-Z]+"); // Geeks followed by any character 
  	
	try
	{
		for(int i=0;i<20;i++)
		{
			a=(&addresses[i])->name;
			// regex_match function matches string a against regex b 
    			if (regex_match(a, b) && a.length()<=3)
			{ 
				cout << "NOT WELL"<<endl; 
				return false;
			}			
		}
		cout<<"OK"<<endl;
		return true;
	}
	catch(int e)
	{
		cout<<"OK"<<endl;
		return true;
	}	
}
bool check_indentations(bool verbose)
{
	cout<<"INDENTATION IS ";
	string line;
	string pre;
	ifstream file;
	file.open("task_management_tool.cpp");
	int numberofP=0;
	int numberofT=0;
	int patient=10;//You can tolerate it 'patient' times
	string T="\t";	
	bool setT=false;
	if(file.is_open()) 
	{
	    	while(getline(file, line))
		{
			line.erase(line.find_last_not_of(" \n\r\t")+1);
			if(line.length()==0)
				continue;
			if (count(line.begin(),line.end(),'{')>0 ||count(line.begin(),line.end(),'}')>0||line.length()==0)
			{			
				if (numberofP==1&&count(line.begin(),line.end(),'{')>0)
					setT=true;
				else if(count(line.begin(),line.end(),'}')>0)
					setT=false;
				if(numberofP==0||count(line.begin(),line.end(),'}')>0||line.length()==0) 
					continue;
			}
			
			numberofP+=count(line.begin(),line.end(),'{');
			numberofP-=count(line.begin(),line.end(),'}');			
			if (setT)// set indentation characters
			{
				T=line.substr(0,line.find_first_not_of(" \t"));
				setT=false;
				if (T.length()==0)
				{
					T="\t";
					setT=true;
				}
			}
			pre=line.substr(0,line.find_first_not_of(" \t"));
			numberofT=count(pre.begin(),pre.end(),T[0])/T.length();
			if (numberofT<numberofP)
			{
				patient-=1;				
			}
			if (patient<0)
			{
				cout<<"NOT WELL"<<endl;
				return false;
			}
			if(verbose)
				cout<<"L:"<<line<<"(NUMBER OF P:"<<numberofP<<")(NUMBER OF T:"<<numberofT<<")(PATIENT:"<<patient<<")'"<<endl;			
			
	    	}
		file.close();
		cout<<"OK"<<endl;
		return true;
	}
	cout<<"ERROR"<<endl;
	return false;
}
