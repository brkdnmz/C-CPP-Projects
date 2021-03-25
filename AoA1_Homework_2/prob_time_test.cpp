/*****************************************************************************************
******************************************************************************************
******************************************************************************************
**********************  PLEASE COMPILE WITH AT LEAST "-std=c++11" FLAG  ******************
******************************************************************************************
******************************************************************************************
*****************************************************************************************/

#include <iostream> //std
#include <fstream>  //File I/O
#include <vector>   //Used as the main structure of heap (insertions, deletions...)
#include <cassert>  //To check if anything unexpected happens
#include <cmath>    //sqrt()
#include <string>   //Used it to read the first line of the file, not that necessary
#include <iomanip>  //for some output functions such as setprecision
#include <chrono>   //High precision clock
#include <random>   //A better random number generator
using namespace std;

typedef std::chrono::high_resolution_clock Clock; //abbreviating

/* Better rand function than classic rand() */
typedef uniform_int_distribution<int> rnd;
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int myRand(){
	return rnd(0, RAND_MAX)(rng);
}

const int INF = (int) 1e9; //represents infinity

const double hotelLongitude = 33.40819; //coordinates of hotel
const double hotelLatitude = 39.19001;

/* Function declarations */
int getParent(int);
int getLeftChild(int);
int getRightChild(int);
double distance(double, double);
double square(double);

/* Min Heap class */
class minHeap{ //dist[parent] <= dist[child]
private:
    vector<double> taxis; //distances of available taxis
    int _size; //name with an underline so that member function can be defined without underline
public:
    minHeap(){
        taxis.clear(); //just in case if anything would happen
        taxis.push_back(0); //this is a dummy just to have a 1-indexed array
        _size = 0; // dummy is not counted, count starts from index 1
    }

    int size() const{
        return _size; //returns the number of taxis
    }

    double getMin() const{
        //minimum element is the root
        return taxis[1]; //1-indexed
    }

    void heapifyUP(int startIndex){ //ascend with swaps until dist[parent] < dist[cur] or cur == 1
        int& index = startIndex; //keeps the index of the current node
        int parent = getParent(index); //index of parent
        while(parent >= 1 && taxis[index] < taxis[parent]){
            //while dist[parent] is strictly greater, swap with parent
            swap(taxis[index], taxis[parent]);
            index = parent;
            parent = getParent(index);
        }
    }

    void insert(const double newTaxi){
        taxis.push_back(newTaxi); 
        //an element inserted, increment size
        _size++;
        //newTaxi's index is "_size" in "taxis"
        //newly added taxi is a leaf now
        //Therefore, heapify is required until dist[parent] < dist[child]
        int curIndex = _size;
        heapifyUP(curIndex);
    }

    void removeMin(){
        //firstly, I implemented an O(2 * log(n)) solution but then I checked the slides.
        //there is a better one, thus, I changed the implementation into that way.
        if(_size == 0) return; //there is no available taxi, just don't do anything
        swap(taxis[1], taxis[_size]); //swap the last element with the minimum so that removing becomes super easy
        taxis.pop_back(); //remove min in O(1)
        _size--; //an element removed, decrement size
        if(_size == 0) return; //there is no available taxi, don't heapify
        //now, descend the (previously last) topmost element until it becomes smaller than or equal to its children
        int index = 1;
        while(true){ //descend until dist[cur] <= dist[children]
            int leftChild = getLeftChild(index); //get index of left child
            int rightChild = getRightChild(index); //get index of right child
            double leftChildVal = leftChild <= _size ? taxis[leftChild] : INF; //if it does not exist, its value is INF
            double rightChildVal = rightChild <= _size ? taxis[rightChild] : INF; //if it does not exist, its value is INF
            int minChild = -1; //will keep the index to be swapped with
            if(leftChildVal >= taxis[index] && rightChildVal >= taxis[index]){
                break; //children do not exist or their values are not smaller than current value
            }
            //else, the minimum one is guaranteed to be smaller than current value, must swap with it
            if(leftChildVal < rightChildVal){
                minChild = leftChild;
            }else{
                minChild = rightChild;
            }
            //just check if anything unexpected happened
            assert(minChild != -1); 
            assert(taxis[minChild] < taxis[index]);
            swap(taxis[index], taxis[minChild]); //swap with smaller child
            index = minChild;
        }
    }

    void update(int index){
        if(_size == 0) return; //there is no available taxi, just don't do anything
        assert(index <= _size); //to make sure index is not bigger than size
        taxis[index] -= 0.01; //decrease the distance
        if(taxis[index] < 0) taxis[index] = 0; //ensure that distance is not negative
        //if it is smaller than parent, heapify up
        heapifyUP(index);
        //there is no need to mind the subtree since dist[cur] was already smaller than dist[children]
    }

    void print() const{ //not related to the homework
        for(int i = 1; i <= _size; i++){
            cout << taxis[i] << "\n";
        }
    }

    void checkIfOK(){ //heap validator
        //check if nodes have a greater value than their parents
        for(int i = 2; i <= _size; i++){
            assert(taxis[i] >= taxis[getParent(i)]);
        }
    }
};

int getParent(int i){
    //      p
    //2*p       2*p+1 -> any of them = i
    //then p = i/2 (integer division)
    //and also when i = 1, it returns 0 (safe)
    return i/2;
}

int getLeftChild(int i){
    //      i
    //2*i       2*i+1
    return 2*i;
}

int getRightChild(int i){
    //      i
    //2*i       2*i+1
    return 2*i + 1;
}

double square(double x){ //x^2
    return x*x;
}

double distance(double longitute, double latitude){ //euclidean distance
    return sqrt(square(longitute - hotelLongitude) + square(latitude - hotelLatitude));
}

int main(int argCount, char** arguments){
    assert(argCount == 3);
	cout<<"Total runtime of operations in milliseconds (average of 25),\n";
	for(double p = 0.1; p <= 0.9; p += 0.1){
		chrono::duration<double> avg(0);
		for(int _ = 0; _ < 25; _++){
			ifstream file("locations.txt");
			if(!file.is_open()){
				cerr << "File cannot be opened, sorry :( But you can try to open next time ^^\n";
				exit(EXIT_FAILURE);
			}
			string foo;
			getline(file, foo, '\n'); //read the first useless line
			int numOfOps = stoi(arguments[1]);      //m
			double updateProb = p; //p
			minHeap heap;   //construct heap
			vector<double> calledTaxis; //stores the called taxis
			int addition = 0;   //number of successful additions
			int update = 0;     //number of successful updates
			for(int i = 1; i <= numOfOps; i++){
				//every operation is counted even if some of them could not be done
				if(i % 100 == 0){   //if it is the 100k-th operation
					if(heap.size() == 0) continue;  //if there is no available taxi
    				Clock::time_point start = Clock::now();
					calledTaxis.push_back(heap.getMin());   //call the nearest taxi
					heap.removeMin();   //remove the nearest taxi
					avg += Clock::now() - start;
					continue;
				}
				//a random real number between [0, 1]
				double which = (double) myRand() / RAND_MAX;
				if(which <= updateProb){ //update random taxi
					int size = heap.size();
					if(size == 0) continue; //if there is no available taxi
					Clock::time_point start = Clock::now();
					int randomTaxi = myRand() % size + 1; //+1 is because heap is 1-indexed
					heap.update(randomTaxi);
					avg += Clock::now() - start;
					update++;
				}else{ //insert from file
					double longitude, latitude;
					file >> longitude >> latitude;
					Clock::time_point start = Clock::now();
					double dist = distance(longitude, latitude);
					heap.insert(dist);  //insert new taxi's distance to heap
					avg += Clock::now() - start;
					addition++;
				}
			}
			file.close();
		}
		double tot = avg.count();
		tot /= 25;
		tot *= 1000;
		cout<<"for p = "<<setprecision(1)<<p<<": ";
		cout<<fixed<<setprecision(6)<<tot<<"\n";
	}
    return 0;
}