/*
	Berke Dönmez
	150170045
*/

#include <iostream>
#include <cmath>	//exp function, e^x
#include <fstream>	//File handling
#include "classes.h"
using namespace std;

Neuron::Neuron(){
	value = 0;
	activatedValue = 0;
}

Neuron::Neuron(const double& value){
	this->value = value;
	activatedValue = value;//This line is just for the first layer, but every neuron's activatedValue is initialized with this. (continued below)
	//It causes no mistake since all activated values of neurons in other layers are going to be calculated one by one.
}

void Neuron::activateSigmoid(){
	activatedValue = 1.0 / (1 + exp(-value));
}

void Neuron::activateRelu(){
	activatedValue = (0 > value) ? 0 : value;	//max(0, value)
	/*
	Same with this:
	if(0 > value) activatedValue = 0;
	else activatedValue = value;
	*/
}

void Neuron::activateLRelu(){
	double otherValue = 0.1 * value;
	activatedValue = (otherValue > value) ? otherValue : value;	//max(0.1 * value, value)
}

void Neuron::setValue(const double& value){
	this->value = value;
}

double Neuron::getValue(){
	return value;
}

double Neuron::getActivatedValue(){
	return activatedValue;
}

SigmoidNeuron::SigmoidNeuron() : Neuron(){} //Calling default Neuron constructor is unnecessary but who cares
SigmoidNeuron::SigmoidNeuron(const double& value) : Neuron(value){}

void SigmoidNeuron::activate(){
	//Since I cannot access activatedValue directly
	activateSigmoid();	
}

ReluNeuron::ReluNeuron() : Neuron(){}		//Calling default Neuron constructor is unnecessary but who cares
ReluNeuron::ReluNeuron(const double& value) : Neuron(value){}

void ReluNeuron::activate(){
	//Since I cannot access activatedValue directly
	activateRelu();
}

LReluNeuron::LReluNeuron() : Neuron(){}		//Calling default Neuron constructor is unnecessary but who cares
LReluNeuron::LReluNeuron(const double& value) : Neuron(value){}

void LReluNeuron::activate(){
	//Since I cannot access activatedValue directly
	activateLRelu();
}

Layer::Layer(){
	neurons = NULL;
	neuronCount = 0;
}

//I thought I would use it but i did not :D
/*Layer::Layer(Neuron** const neuronArr, const int& size){
	neurons = neuronArr;
	neuronCount = size;
}*/

void Layer::initializeNeurons(const int& neuronCount){
	neurons = new Neuron*[neuronCount];
	this->neuronCount = neuronCount;
}

void Layer::setNeuron(const int& index, Neuron* const neuron){
	neurons[index] = neuron;	//Address assignment
}

void Layer::setValue(const int& index, const double& value){
	neurons[index]->setValue(value);	//Indirect access
}

void Layer::setActivatedValue(const int& type, const int& index){
	if (type == 0){
		neurons[index]->activateSigmoid();
	}
	else if (type == 1){
		neurons[index]->activateLRelu();
	}
	else if (type == 2){
		neurons[index]->activateRelu();
	}
}

double Layer::getActivatedValue(const int& index){
	return neurons[index]->getActivatedValue();	//Indirect access
}

Layer::~Layer(){
	//Since "neurons" is a 2D dynamic array, I should delete like this
	for (int i = 0; i < neuronCount; i++){
		delete neurons[i];
	}
	delete[] neurons;
}

Network::Network(){
	layers = NULL;
	layerCount = 0;
}

//I did not use it
/*Network::Network(Layer** const layerArr, const int& size){
	layers = layerArr;
	layerCount = size;
}*/

void Network::initializeLayers(const int& layerCount){
	layers = new Layer*[layerCount];
	this->layerCount = layerCount;
}

void Network::setLayer(const int& index, Layer* const layer){
	layers[index] = layer;	//Address assignment
}

void Network::setActivatedValue(const int& index, const int& type, const int& neuronIndex){
	layers[index]->setActivatedValue(type, neuronIndex);	//Indirect access
}

double Network::getActivatedValue(const int& layerIndex, const int& neuronIndex){
	return layers[layerIndex]->getActivatedValue(neuronIndex);	//Indirect access
}

void Network::matrixMultiplication(const int& leftLayer, const int& leftCount, const int& rightCount, double** const W_Matrix, double* const B_Vector){
	//An example
	/*
			|w00 w01 w02|	|x0|	|b0|	|z0|
			|w10 w11 w12| x |x1| +	|b1| =	|z1|
			|w20 w21 w22|	|x2|	|b2|	|z2|
	*/
	//z0 == 0th row of W . x + 0th row of b	(b0)
	//
	//zi == ith row of W . x + ith row of b	(bi)
	//Where i is "currentNeuron"
	int index = leftLayer;
	int nextIndex = index + 1;
	for (int currentNeuron = 0; currentNeuron < rightCount; currentNeuron++){
		double result = 0;
		for (int col = 0; col < leftCount; col++){				//
			double w = W_Matrix[currentNeuron][col];			//	W(i, 0) . x0 + W(i, 1) . x1... + W(i, n) . xn
			double currentValue = getActivatedValue(index, col);//	One by one addition
			result += w * currentValue;							//
		}
		result += B_Vector[currentNeuron];						//+ ith row of b (bi)
		layers[nextIndex]->setValue(currentNeuron, result);		//Indirectly setting zi
	}
}

Network::~Network(){
	//2D dynamic array deletion
	for (int i = 0; i < layerCount; i++){
		delete layers[i];
	}
	delete[] layers;
}

int main(int argc, char** argv){
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	ifstream file;
	//file.open("C:\\Users\\wwwbe\\Desktop\\OOP Ödevler\\odev3\\input.txt");
	file.open(argv[1]);
	if (!file.is_open()){
		cout << "File could not be opened, sorry...\n";
		return 0;
	}
	try{
		Network network;
		int layerCount = 0;
		file >> layerCount;
		//I created dynamic arrays in order to store the input
		int *neuronCount = new int[layerCount];
		for (int i = 0; i < layerCount; i++){
			file >> neuronCount[i];
		}
		int *neuronType = new int[layerCount];
		for (int i = 0; i < layerCount; i++){
			file >> neuronType[i];	//I checked validity of the input after scanning the input, before processing them
		}
		int *inValue = new int[layerCount];
		for (int i = 0; i < neuronCount[0]; i++){
			if (file.eof()){	//If there is less number of inputs than the size of the first layer
				delete[] neuronCount;
				delete[] neuronType;
				delete[] inValue;
				throw "Input shape does not match!";
			}
			file >> inValue[i];
		}
		if (!file.eof()){		//If there is more number of inputs than the size of the first layer
			throw "Input shape does not match!";
		}
		else{
			for (int i = 0; i < layerCount; i++){
				if (!(0 <= neuronType[i] && neuronType[i] <= 2)){	//If the given type is not 0, 1 or 2
					delete[] neuronCount;
					delete[] neuronType;
					delete[] inValue;
					throw "Unidentified activation function!";
				}
			}

			network.initializeLayers(layerCount);

			for (int i = 0; i < layerCount; i++){	//I traversed the layers one by one
				//I created the neurons one by one, then used them in the member function "setNeuron"
				int count = neuronCount[i];
				int type = neuronType[i];
				Layer *newLayer = new Layer;
				newLayer->initializeNeurons(count);	//Initializing "neurons" array
				Neuron *creator = NULL;
				for (int j = 0; j < count; j++){
					double value = ((i == 0) ? inValue[j] : 0);	
					//If it is the first layer, both value and activatedValue is the given value.
					//else, both value and activatedValue is "0".
					if (type == 0){
						creator = new SigmoidNeuron(value);
					}
					else if (type == 1){
						creator = new LReluNeuron(value);
					}
					else if (type == 2){
						creator = new ReluNeuron(value);
					}
					newLayer->setNeuron(j, creator);//Sending creator's address to the "neurons" array
				}
				network.setLayer(i, newLayer);	//Sending newLayer's address to the "layers" array
			}
			for (int i = 0; i < layerCount - 1; i++){//Use ith layer's activated values to calculate (i+1)th layer's unactivated values
				int currentCount = neuronCount[i];
				int nextCount = neuronCount[i + 1];
				double** matrix = new double*[nextCount];	//
				for (int r = 0; r < nextCount; r++){		//
					matrix[r] = new double[currentCount];	//
					for (int c = 0; c < currentCount; c++){	//Creating a dynamic 2D array consists of 0.1's
						matrix[r][c] = 0.1;					//
					}										//
				}											//
				double *b_vector = new double[nextCount];//B vector
				for (int r = 0; r < nextCount; r++){
					b_vector[r] = 0.1;
				}
				network.matrixMultiplication(i, currentCount, nextCount, matrix, b_vector);	//Calculating W.X + B = Z
				for (int r = 0; r < nextCount; r++){
					network.setActivatedValue(i + 1, neuronType[i + 1], r);	//Calculating activated values one by one
				}
				for (int r = 0; r < nextCount; r++){	//Deletions
					delete[] matrix[r];
				}
				delete[] matrix;
				delete[] b_vector;
			}
		}
		for (int i = 0; i < layerCount; i++){
			cout << "Layer " << i << ":" << endl;
			for (int j = 0; j < neuronCount[i]; j++){
				cout << network.getActivatedValue(i, j) << endl;	//Indirectly getting the activated values and printing them
			}
		}
		delete[] neuronCount;	//Deleting the input keepers
		delete[] neuronType;
		delete[] inValue;
	}
	catch(const char* error){
		//If an error occured, just print the sent message
		cout << error << endl;
	}
	//system("PAUSE");
	file.close();
	return 0;
}