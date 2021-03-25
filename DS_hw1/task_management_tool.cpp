/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

/* @Author
Student Name: Berke Dönmez
Student ID : 150170045
Date: 13.11.2019 */
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <stdio.h>

#include "task_management_tool.h"

using namespace std; 


void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot =new Task;
		Task *compeer =new Task;
			
		pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV=compeer->previous!=NULL?compeer->previous->name:"NULL";
				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}


void WorkPlan::create()
{		
	head = NULL;
}

void WorkPlan::close()
{
	if(head == NULL){//Exit without adding any task
		return;
	}
	
	Task *travel = head;
	Task *travel_in_day = travel;
	
	do{													//First do, then check. It is for deleting the head
		travel_in_day = travel;
		travel = travel->next;
		do{												//And this do-while is for deleting the first task
			Task *deleted = travel_in_day;				//The task which is going to be deleted
			travel_in_day = travel_in_day->counterpart;	//Going down in the day
			delete[] deleted->name;						//We must first delete the name we created with "new"
			delete deleted;
		}while(travel_in_day != NULL);					//When it comes to the last counterpart's counterpart, it breaks the loop
	}while(travel != head);
	//head = NULL;
	//When it comes to head, it breaks the loop.
	//(When I deleted head, it didn't point NULL so the condition is not "travel != NULL". I think "delete" just blocks changing the value of the address)
	//head = NULL;
}

void WorkPlan::add(Task *task)
{
	//// Creating a task whose values are the same as the argument's values.(Not same addresses) ////
	task->next = NULL;
	task->previous = NULL;
	task->counterpart = NULL;
	Task *task2be_added = new Task();
	*task2be_added = *task;		//Day, time and priority are same now.
	task2be_added->name = new char[strlen(task->name)+1];
	strcpy(task2be_added->name, task->name);	//If I just made addresses same, I could not access the address after the add function finishes, I could not understand why.
	
	//// If we add our first task ////
	if(head == NULL){
		
		head = new Task();
		*head = *task;
		head->name = new char[strlen(task->name)+1];
		strcpy(head->name, task->name);
		head->next = head;
		head->previous = head;
		head->counterpart = NULL;
		return;
		
	}
	//// If there is only one non-empty day before adding the task ////
	else if(head->next == head){
		
		if(task2be_added->day < head->day){	//If we add a task whose day comes before head's day
			
			task2be_added->next = head;
			task2be_added->previous = head;
			head->previous = task2be_added;
			head->next = task2be_added;
			task2be_added->counterpart = NULL;
			head = task2be_added;
			
		}
		else if(task2be_added->day > head->day){ 	//If we add a task whose day comes after head's day
			
			head->next = task2be_added;
			head->previous = task2be_added;
			task2be_added->previous = head;
			task2be_added->next = head;
			task2be_added->counterpart = NULL;
			
		}
		else{	//If we add another task into head (Task's day == head's day)
		
			if(task2be_added->time < head->time){	//If task is earlier than head's first task
				task2be_added->previous = task2be_added;
				task2be_added->next = task2be_added;
				task2be_added->counterpart = head;
				head->previous = NULL;
				head->next = NULL;
				head = task2be_added;
				
			}
			else if(task2be_added->time == head->time){	// If task coincides head
				
				if(task2be_added->priority < head->priority){	//If task's priority is smaller than head's priority
					checkAvailableNextTimesFor(task2be_added);
					task2be_added->day = getUsableDay();
					task2be_added->time = getUsableTime();
				}
				else{	//Else, we should change the head
					// Swapping the priorities //
					int old_priority = head->priority;
					head->priority = task2be_added->priority;
					task2be_added->priority = old_priority;
					// Swapping the names (below) //
					char a[strlen(head->name)+1];
					strcpy(a, head->name);
					strcpy(head->name, task2be_added->name);
					strcpy(task2be_added->name, a);
				}
					add(task2be_added);
			}
			else{									//If task is later than head's first task
				
				Task *travel = head;
				Task *tail = travel;
				
				while(travel->counterpart != NULL && travel->time < task2be_added->time){
					
					tail = travel;
					travel = travel->counterpart;
					
				}	//Now task2be_added is between tail and travel or it is later than travel
				
				if(travel->time < task2be_added->time){	//This can only happen if task2be_added is later than the last task
					
					travel->counterpart = task2be_added;
					//task2be_added->counterpart was NULL already
				}
				else{	//If task2be_added is between tail and travel, we're inserting it between them
					
					tail->counterpart = task2be_added;
					task2be_added->counterpart = travel;
					
				}
			}
		}
		return;
	}
	////	IF THERE ARE MORE THAN 1 DAY (BELOW)	////
	if(task2be_added->day < head->day){	//If task2be_added's day comes before the first day of the list, we must change the head
		
		task2be_added->next = head;
		task2be_added->previous = head->previous;
		head->previous->next = task2be_added;
		head->previous = task2be_added;
		head = task2be_added;
		return;
		
	}else if(task2be_added->day == head->day && task2be_added->time < head->time){	//If task2be_added's day and head's day are same but task2be_added is earlier than the first task, we must change the head
		
		task2be_added->next = head->next;
		task2be_added->previous = head->previous;
		head->next->previous = task2be_added;
		head->previous->next = task2be_added;
		head->next = NULL;
		head->previous = NULL;
		task2be_added->counterpart = head;
		head = task2be_added;
		return;
	}
	
	Task *temp = getTask(task2be_added->day, task2be_added->time);	//Getting the task from the list and assign the address of it to a temporary pointer
	
	if(temp != NULL){	//If there is a task which has the same day and time as task2be_added's in the list
		
		if(task2be_added->priority < temp->priority){	//If task2be_added's priority is smaller than temp's priority
			
			checkAvailableNextTimesFor(task2be_added);	//Check the available day and hour
			task2be_added->day = getUsableDay();		//And assign them to task2be_added(We are changing task2be_added)
			task2be_added->time = getUsableTime();
		
		}
		else{	//If task2be_added's priority is bigger than temp's priority
			// Swapping the priorities //
			int old_priority = temp->priority;
			temp->priority = task2be_added->priority;
			task2be_added->priority = old_priority;
			// Swapping the names (below) //
			char a[strlen(temp->name)+1];
			strcpy(a, temp->name);
			strcpy(temp->name, task2be_added->name);
			strcpy(task2be_added->name, a);
		}
		
		add(task2be_added); //After doing the operations, we add our changed task2be_added
	
	}else{	//If there isn't any task which has the same day and time as task2be_added's in the list(Day can be same but time cannot), it is easier
		
		Task *travel = head;
		
		while(travel->next != head && travel->day < task2be_added->day){
			travel = travel->next;
		}//Going to the day which comes after task2be_added or same as task2be_added's day or it is the last day
		
		if(travel->day < task2be_added->day){	//Which means that travel->next is head, travel is the last day, we are changing the last day
			
			travel->next = task2be_added;
			task2be_added->next = head;
			head->previous = task2be_added;
			task2be_added->previous = travel;
		
		}
		else if(travel->day > task2be_added->day){	//Which means travel comes after task2be_added and travel->previous comes before task2be_added
			
			travel->previous->next = task2be_added;
			task2be_added->next = travel;
			task2be_added->previous = travel->previous;
			travel->previous = task2be_added;
		
		}
		else{	//If task2be_added's day and travel's day are same
			
			if(task2be_added->time < travel->time){	//If task2be_added is earlier than the first task. First task of the day changes
				task2be_added->next = travel->next;
				task2be_added->previous = travel->previous;
				travel->next->previous = task2be_added;
				travel->previous->next = task2be_added;
				travel->next = NULL;	//Because travel is not the
				travel->previous = NULL;//first task anymore
				task2be_added->counterpart = travel;
				return;
			}
			
			Task *tail = travel;	//Defining a tail, which will later be one task behind the travel (Travel may not change so tail may be travel, below)
			
			while(travel->counterpart != NULL && travel->time < task2be_added->time){
				tail = travel;
				travel = travel->counterpart;
			}	//Travel now comes after task2be_added and tail comes before task2be_added or travel is the last task and task2be_added comes after travel
			
			if(travel->time < task2be_added->time){	//If travel is the last task and task2be_added comes after travel
				travel->counterpart = task2be_added;
			}
			else{	//If task2be_added is between tail and travel
				
				tail->counterpart = task2be_added;
				task2be_added->counterpart = travel;
			
			}
		}
	}
	
} 

Task * WorkPlan::getTask(int day, int time)
{
	Task *travel = head;
	while(travel->next != head && travel->day < day){
		travel = travel->next;
	}//Travel's day == day now if "day" exists
	if(travel->day != day){	//If "day" does not exist
		return NULL;	//There is no task in the day, so return NULL pointer	
	}
	while(travel->counterpart != NULL && travel->time < time){	//If day exists
		travel = travel->counterpart;
	}	//Travel's time == time now if "time" of that "day" exists
	if(travel->time != time){	//If "time" does not exist
		return NULL;	//There is no task in the time, so return NULL pointer
	}
	return travel;	//After operations if task exists, return the task
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed) //I used it for the existing day and time (Delaying operations)
{
	Task *travel = head;
	while(travel->next != head && travel->day < delayed->day){
		travel = travel->next;
	}//travel->day == delayed->day now
	
	do{	//do-while is used to also check the head
		
		int check = 0;	//A checker checks whether the available hour is found or not
		Task *travel_in_day = travel;	//This is the pointer that travels in a day
		
		if(travel->day > delayed->day){	//After checking the day of "delayed", if available hour is not found on that day, function continues checking with this "if"
			
			if(travel->time > 8){	//If eighth hour is free, it is available hour
				
				usable_day = travel->day;
				usable_time = 8;
				check = 1;	//We found the first available hour
				
			}
			else{	//If eighth hour is not free
				
				while(travel_in_day->counterpart != NULL){	//We should check the counterparts
					
					if(travel_in_day->counterpart->time - travel_in_day->time > 1){	//If the time time difference between counterparts is more than one, it is what we are searching for
						
						usable_day = travel_in_day->day;
						usable_time = travel_in_day->time + 1;	//Then the first available is one hour later
						check = 1;	//We found the first available hour
						break;
						
					}
					
					travel_in_day = travel_in_day->counterpart;	//We are going to the later tasks in the day
				}
			}
		}
		else{	//Checking the task's day we want to delay
			
			while(travel_in_day->counterpart != NULL && travel_in_day->time <= delayed->time){	
				
				if((travel_in_day->counterpart->time > delayed->time) && (travel_in_day->counterpart->time - travel_in_day->time > 1)){	//If the task to be delayed can be between two tasks
					
					usable_day = travel_in_day->day;
					usable_time = travel_in_day->time + 1; //Then the first available is one hour later
					check = 1;	//We found the first available hour
					break;
				
				}
				travel_in_day = travel_in_day->counterpart;	//This is "else", actually. We are going to the later tasks in the day
			
			}
		}
		
		if(check){	//If we found an available hour
			break;
		}
		travel = travel->next;	//Traveling the first tasks (days)
	
	}while(travel != head);	//Do these until come to beginning
}

void WorkPlan::delayAllTasksOfDay(int day)
{
	Task *travel = head;
	
	while(travel->next != head && travel->day < day){
		travel = travel->next;
	}	//Travel->day == "day", we found our day to be delayed
	
	Task *last_task = travel;	//Defining a pointer, which will later point to the last task
	
	while(last_task->counterpart != NULL){
		last_task = last_task->counterpart;
	}	//last_task now points to the last_task of "day"
	
	while(travel != NULL){	//We are delaying all the tasks ("travel" points to these tasks)
		
		checkAvailableNextTimesFor(last_task);	//Always check available time for the last_task, that's why we created this pointer.
		/**************************************///Because this function should delay all the tasks to following days
		/**************************************///										
		//// Copying the data to a new pointer, because I will delete the existing pointer ////										
		Task *delayed = new Task();
		*delayed = *travel;
		//// Copying the name, separately ////
		delayed->name = new char[strlen(travel->name)+1];
		strcpy(delayed->name, travel->name);
		delayed->priority = travel->priority;	//This line may not be necessary
		Task *temp = travel;					//Defining a temporary pointer
		travel = travel->counterpart;			//Going to the next task
		remove(temp);							//If we deleted "travel" here, we actually deleted the counterpart, not itself.	Then we couldn't go to the later tasks
		delayed->day = getUsableDay();			//Changing our task to delay it
		delayed->time = getUsableTime();
		add(delayed);							//We actually delayed it by removing and re-adding it, now
	}
}

void WorkPlan::remove(Task *target)
{
	if(target == head){	//If we want to remove the head
		if(head->counterpart != NULL){	//If there is more one task in head, head->counterpart must be the new head
			
			head->previous->next = target->counterpart;	//Changing the linkings
			target->counterpart->previous = head->previous;
			target->counterpart->next = head->next;
			head->next->previous = target->counterpart;
			head = head->counterpart;	//Changing the head
		
		}
		else{	//If there is just one task, which is the first task, head->next must be the new head
			
			head->next->previous = head->previous;
			head->previous->next = head->next;
			head = head->next;
		
		}
		//delete[] target->name;	//When I delete the name, because of how delay_a_task was written in app.cpp (address equalizing instead of data equalizing with strcpy), it causes error
		delete target;	//Removing the task
		return;	
	}
	if(target->next != NULL){ //If it's not in head but it is the first task of the day
		if(target->counterpart != NULL){	//If there are more than one task in the day
			if(target->next != target){		//If there are more than one day
				target->previous->next = target->counterpart;
				target->counterpart->next = target->next;
				target->counterpart->previous = target->previous;
				target->next->previous = target->counterpart;
			}else{							// If there is only one day (The head itself), the counterpart must be the new head and it must be linked to itself (Circular) 
				target->counterpart->next = target->counterpart;
				target->counterpart->previous = target->counterpart;
			}
		}else{								//If there is only one task, these linkin changes must be done
			target->previous->next = target->next;
			target->next->previous = target->previous;
		}
	}else{	//Target is not the first task of the day
		Task *travel = head;
		while(travel->day < target->day){
			travel = travel->next;
		}	//travel->day == target->day at the moment
		Task *tail = travel;
		while(travel->time < target->time){
			tail = travel;
			travel = travel->counterpart;
		}	//travel->time == target->time , travel points the target, tail points one task behind the target
		tail->counterpart = travel->counterpart;
	}
	//delete[] target->name;		//When I delete the name, because of how delay_a_task was written in app.cpp (address equalizing instead of data equalizing with strcpy), it causes error
	delete target;
}

bool WorkPlan::checkCycledList()
{				
	Task *pivot=new Task();
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
