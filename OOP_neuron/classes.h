/*
	Berke Dönmez
	150170045
*/

class Neuron{
private:
	double value;
	double activatedValue;
public:
	Neuron(); //It's necessary to dynamically allocate memory for an array
	Neuron(const double&);	//Construct with the given value
	virtual void activate() = 0;
	void activateSigmoid();	//
	void activateRelu();	//Different activators for three different types of neurons
	void activateLRelu();	//
	void setValue(const double&);	//
	double getValue();				//Indrirect access
	double getActivatedValue();		//
	//virtual ~Neuron(); It is not necessary since the class does not contain any dynamically allocated memory
};

class SigmoidNeuron : public Neuron{
public:
	SigmoidNeuron();	//It's necessary to dynamically allocate memory for an array
	SigmoidNeuron(const double&);	//Construct with the given value
	void activate();
	//~SigmoidNeuron(); Same reason with "Neuron" class
};

class ReluNeuron : public Neuron{
public:
	ReluNeuron();		//It's necessary to dynamically allocate memory for an array
	ReluNeuron(const double&);
	void activate();
	//~ReluNeuron();
};

class LReluNeuron : public Neuron{
public:
	LReluNeuron();		//It's necessary to dynamically allocate memory for an array
	LReluNeuron(const double&);
	void activate();
	//~LReluNeuron();
};

class Layer{
private:
	Neuron **neurons;	//Dynamic array which contains pointers of "Neuron" class
	int neuronCount;	//I used it in destructor
public:
	Layer();			//It's necessary to dynamically allocate memory for an array
	//Layer(Neuron** const, const int&);	//(I DID NOT USE IT) If this is called, "neurons" pointer points to the same place where given pointer points to
	void initializeNeurons(const int&);		//Allocating memory for "neurons", with the given size
	void setNeuron(const int&, Neuron* const);		//Assigning the given address to the target neuron (index is given)
	void setValue(const int&, const double&);		//
	void setActivatedValue(const int&, const int&);	//Indirect access
	double getActivatedValue(const int&);			//
	~Layer();			//Deletes "neurons"
};

class Network{
private:
	Layer **layers;		//Dynamic array which contains pointers of "Layer" class
	int layerCount;		//I used it in destructor
public:
	Network();			//It is obliged to process "layers = NULL;"
	//Network(Layer** const, const int&); //I DID NOT USE IT
	void initializeLayers(const int&);	//Allocating memory for "layers", with the given size
	void setLayer(const int&, Layer* const);						//
	void setActivatedValue(const int&, const int&, const int&);		//Indirect access
	double getActivatedValue(const int&, const int&);				//
	void matrixMultiplication(const int&, const int&, const int&, double** const, double* const);	//Calculates the matrix equation
	~Network();			//Deletes "layers"
};