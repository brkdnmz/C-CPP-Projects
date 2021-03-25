#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

const int SIZE = 3;
class Node; //predeclaration for Key class

class Key{
private:
	int keyIndex;
	int* attr;
	Node* child; //preceding child
public:
	Key();
	Key(const int&, const int*);
	Key(const Key&);
	int getKey() const;
	void operator=(const Key&);
	int operator[](const int) const;
	bool operator<(const Key&) const;
	bool operator<=(const Key&) const;
	bool operator==(const Key&) const;
	bool operator>(const Key&) const;
	~Key();
	friend class BTree;
	friend class Node;
};
Key::Key(){
	child = NULL;
	keyIndex = 0;
	attr = new int[SIZE];
}
Key::Key(const int& keyIndex, const int attr[]){
	child = NULL;
	this->attr = new int[SIZE];
	this->keyIndex = keyIndex;
	for(int i = 0; i < SIZE; i++){
		this->attr[i] = attr[i];
	}
}
Key::Key(const Key& otherKey){
	this->attr = new int[SIZE];
	keyIndex = otherKey.keyIndex;
	for(int i = 0; i < SIZE; i++){
		this->attr[i] = otherKey.attr[i];
	}
	child = otherKey.child;
}
int Key::getKey() const{
	return attr[keyIndex];
}
void Key::operator=(const Key& otherKey){
	keyIndex = otherKey.keyIndex;
	for(int i = 0; i < SIZE; i++){
		this->attr[i] = otherKey.attr[i];
	}
	child = otherKey.child;
}
int Key::operator[](const int index) const{
	assert(index >= 0 && index < 3);
	return attr[index];
}
bool Key::operator<(const Key& other) const{
	return attr[keyIndex] < other.attr[keyIndex];
}
bool Key::operator<=(const Key& other) const{
	return attr[keyIndex] <= other.attr[keyIndex];
}
bool Key::operator==(const Key& other) const{
	//keys are going to be the same
	for(int i = 0; i < SIZE; i++){
		if(this->attr[i] != other.attr[i]){
			return false;
		}
	}
	return true;
}
bool Key::operator>(const Key& other) const{
	return attr[keyIndex] > other.attr[keyIndex];
}

Key::~Key(){
	delete[] attr;
}


class Node{
private:
	bool leaf;
	Node* lastChild; 	//child connected to place after last key
	Node* parent;
    vector<Key> keys;
public:
	Node();
	Node(const Node&);
	const vector<Key>& getKeys() const;
	Key firstKey() const;
	Key lastKey() const;
	bool empty() const;
	int size() const;
	bool exists(const Key&) const;
	int indexOf(const Key&) const;
	Node*& preceding_child(const Key&);
	Node*& following_child(const Key&);
	void insert(const Key&);
	void erase(const Key&);
	bool isFull(const int&) const;
	bool isLeaf() const;
	Key operator[](const int) const;
	~Node();
	friend class BTree;
};
Node::Node() : leaf(true), lastChild(NULL), parent(NULL){
	keys.clear();
}
Node::Node(const Node& other){
	leaf = other.leaf;
	lastChild = other.lastChild;
	parent = other.parent;
	keys = other.keys;
}

const vector<Key>& Node::getKeys() const{
	return keys;
}
Key Node::firstKey() const{
	assert(!keys.empty());
	return *keys.begin();
}
Key Node::lastKey() const{
	assert(!keys.empty());
	return keys.back();
}
bool Node::empty() const{
	return keys.empty();
}
int Node::size() const{
	return keys.size();
}
bool Node::exists(const Key& target) const{
	int index = indexOf(target);
	if(index == size()) return false;
	return keys[index].getKey() == target.getKey();
}
int Node::indexOf(const Key& target) const{
	//finds the first <= key
	//binary search
	int len = size();
	int l = 0, r =  len;
	while(l < r){
		int mid = (l+r)/2;
		if(target > keys[mid]){
			l = mid+1;
		}else{
			r = mid;
		}
	}
	return l; //if not found, returns keys.size() (out of bound)
}
Node*& Node::preceding_child(const Key& target){
	return keys[indexOf(target)].child;
}
Node*& Node::following_child(const Key& target){
	int KeyIndex = indexOf(target);
	KeyIndex++;
	assert(KeyIndex <= size());
	if(KeyIndex < size()){
		return keys[KeyIndex].child;
	}else if(KeyIndex == size()){
		return lastChild;
	}
	return lastChild; //this line cannot be reached
}
void Node::insert(const Key& newKey){
	//insertion does not violate sorted form
	int followingIndex = indexOf(newKey); //keys[followingIndex] >= newKey or followingIndex == size()
	//"followingIndex == size()" case is also actually handled
	keys.push_back(Key()); //dummy key
	for(int i = size() - 1; i > followingIndex; i--){
		keys[i] = keys[i-1];
	}
	keys[followingIndex] = newKey;
}
void Node::erase(const Key& target){
	int targetIndex = indexOf(target);
	for(int i = targetIndex; i < size() - 1; i++){
		keys[i] = keys[i+1];
	}
	keys.pop_back();
}
bool Node::isFull(const int& t) const{
	return size() == 2*t - 1;
}
bool Node::isLeaf() const{
	return leaf;
}
Key Node::operator[](const int index) const{
	return keys[index];
}
Node::~Node(){
	keys.clear();
}
class BTree{
private:
	int t;
	Node* root;
public:
	BTree();
	Node* getRoot() const;
	void set_t(int);
	void insert_into_tree(const Key);
	void insert_into_node(Node*, Key);
	Node* split(Node*);
	const Node* findNodeWithKey(const Node*, Key) const;
	void erase(Key);
	void eraseHelper(Node*, Key);
	void borrowFromLeftSibling(Node*, Key);
	void borrowFromRightSibling(Node*, Key);
	Node* mergeWithChildren(Node*, Key);
	void printPreOrder() const;
	void printHelper(const Node*) const;
	void TreeEraser(Node*);
	~BTree();
};
BTree::BTree() : root(NULL){
	//nothing to write
}

Node* BTree::getRoot() const{
	return root;
}
void BTree::set_t(int t){
	this->t = t;
}

void BTree::insert_into_tree(const Key newKey){
	if(root == NULL){
		root = new Node;
		root->insert(newKey);
		return;
	}
	Node* finder = root;
	while(true){ //applies splits when needed, finds the appropriate leaf for newKey and finally, inserts it
		if(finder->isFull(t)){
			finder = split(finder);
		}
		if(finder->isLeaf()){
			finder->insert(newKey);
			return;
		}
		int place = finder->indexOf(newKey);
		if(place == finder->size()){
			finder = finder->lastChild;
		}else{
			finder = (*finder)[place].child;
		}
	}
}
void BTree::insert_into_node(Node* target, Key key){
	if(key.child){
		key.child->parent = target;
	}
	target->insert(key);
}
Node* BTree::split(Node* target){ //splits the node when it is full and returns the parent
	//t-1 elements --- median --- t-1 elements
	Node *left = new Node, *right = new Node;
	left->leaf = target->leaf;
	right->leaf = target->leaf;
	for(int i = 0; i < t-1; i++){
		insert_into_node(left, (*target)[i]);
	}
	Key median = (*target)[t-1];
	left->lastChild = median.child;
	if(left->lastChild){
		left->lastChild->parent = left;
	}
	for(int i = t; i < 2*t-1; i++){
		insert_into_node(right, (*target)[i]);
	}
	right->lastChild = target->lastChild;
	if(right->lastChild){
		right->lastChild->parent = right;
	}
	Node* targetParent = target->parent;
	if(targetParent == NULL){ //target was root
		targetParent = new Node;
		targetParent->leaf = false;
		root = targetParent;
	}
	delete target;
	left->parent = targetParent;
	right->parent = targetParent;
	targetParent->insert(median);
	targetParent->preceding_child(median) = left;
	targetParent->following_child(median) = right;
	return targetParent;
}
const Node* BTree::findNodeWithKey(const Node* current, Key targetKey) const{
	if(current == NULL){
		return NULL;
	}
	int place = current->indexOf(targetKey);
	if(place != current->size() && (*current)[place].getKey() == targetKey.getKey()){
		return current;
	}
	const Node* next = (place == current->size() ? current->lastChild : (*current)[place].child);
	return findNodeWithKey(next, targetKey);
}
void BTree::erase(Key targetKey){
	eraseHelper(root, targetKey);
}
void BTree::eraseHelper(Node* current, Key targetKey){
	//if target does not exist, return without doing anything
	if(current == root){
		const Node* targetNode = findNodeWithKey(root, targetKey);
		if(targetNode == NULL){
			return;
		}
	}
	assert(current);
	assert(!current->empty());
	//DO NOT FORGET ROOT
	/*
		BE CAREFUL DOING STUFF WITH TARGET
		SENT TARGET MAY HAVE ONLY THE KEY, NOT THE CHILD ETC.
	*/
	int targetPlace = current->indexOf(targetKey);
	if(current->exists(targetKey)){ //key is found
		targetKey = (*current)[targetPlace]; //make sure to get other attributes
		if(current->isLeaf()){
			//1-st case
			/*
				leaf is guaranteed to have at least t keys... (except root when root is the only node) 
				...thanks to the 3-rd case 
			*/
			current->erase(targetKey);
			if(current->empty()){
				if(root == current){
					root = NULL;
				}
				delete current;
			}
		}
		else{ //internal node
			//2-nd case
			Node* leftChild = current->preceding_child(targetKey);
			Node* rightChild = current->following_child(targetKey);
			assert(leftChild);
			assert(rightChild);
			if(leftChild->size() >= t){
				/*		2.a		*/
				current->erase(targetKey);
				Key borrowed = leftChild->lastKey();
				eraseHelper(leftChild, leftChild->lastKey());
				borrowed.child = leftChild;
				current->insert(borrowed);
			}
			else if(rightChild->size() >= t){
				/*		2.b		*/
				current->erase(targetKey);
				Key borrowed = rightChild->firstKey();
				eraseHelper(rightChild, rightChild->firstKey());
				borrowed.child = targetKey.child;
				current->insert(borrowed);
			}
			else{
				/*		2.c		*/
				Node* mergedNode = mergeWithChildren(current, targetKey);
				current = mergedNode;
				eraseHelper(current, targetKey); //call function again with the new node
			}
		}
		return;
	}
	else{
		//3-rd case
		assert(!current->isLeaf());
		if(targetPlace == current->size()){
			//target is in the subtree rooted at current->lastChild
			targetPlace--;
			Key insertToSubroot = (*current)[targetPlace];
			Node* subroot = current->lastChild;
			assert(subroot);
			if(subroot->size() >= t){
				eraseHelper(subroot, targetKey);
				return;
			}
			Node* leftSibling = insertToSubroot.child;
			assert(leftSibling);
			if(leftSibling->size() >= t){
				/*		3.a		*/
				current->erase(insertToSubroot);
				insertToSubroot.child = leftSibling->lastChild;
				subroot->insert(insertToSubroot);
				Key siblingLast = leftSibling->lastKey();
				leftSibling->lastChild = siblingLast.child;
				leftSibling->erase(siblingLast);
				siblingLast.child = leftSibling;
				current->insert(siblingLast);
				//lastChild of current node did not change (safe)
			}
			else{
				/*		3.b		*/
				current = mergeWithChildren(current, insertToSubroot);
			}
			eraseHelper(current, targetKey);
			return;
		}
		else{
			Key insertToSubroot = (*current)[targetPlace];
			Node* subroot = insertToSubroot.child;
			assert(subroot);
			if(subroot->size() >= t){
				eraseHelper(subroot, targetKey);
				return;
			}
			//target is in the subtree rooted at ancestorKey_it->child
			//right sibling is guaranteed to exist
			//but left sibling might not exist (if subroot is the first child)
			int nextKeyPlace = targetPlace + 1;
			Node* rightSibling;
			if(nextKeyPlace == current->size()){
				rightSibling = current->lastChild;
			}else{
				rightSibling = (*current)[nextKeyPlace].child;
			}
			assert(rightSibling);
			if(rightSibling->size() >= t){
				borrowFromRightSibling(current, insertToSubroot);
			}
			else if(subroot != current->firstKey().child){
				int prevKeyPlace = targetPlace - 1;
				assert(prevKeyPlace >= 0);
				Node* leftSibling = (*current)[prevKeyPlace].child;
				if(leftSibling->size() >= t){
					borrowFromLeftSibling(current, insertToSubroot);
				}else{
					//FORGETTING TO ADD THIS LINE CAUSED STACK OVERFLOWS...
					current = mergeWithChildren(current, insertToSubroot);
				}
			}
			else{
				current = mergeWithChildren(current, insertToSubroot);
			}
			eraseHelper(current, targetKey);
			return;
		}
	}
}
void BTree::borrowFromLeftSibling(Node* parent, Key parentKey){
	Node* target = parent->preceding_child(parentKey);
	int prevKeyPlace = parent->indexOf(parentKey) - 1;
	assert(prevKeyPlace >= 0);
	Key insertToSubroot = (*parent)[prevKeyPlace];
	Node* leftSibling = insertToSubroot.child;
	parent->erase(insertToSubroot);
	insertToSubroot.child = leftSibling->lastChild;
	insert_into_node(target, insertToSubroot);
	Key leftLast = leftSibling->lastKey();
	leftSibling->lastChild = leftLast.child;
	leftSibling->erase(leftLast);
	leftLast.child = leftSibling;
	parent->insert(leftLast);
}
void BTree::borrowFromRightSibling(Node* parent, Key parentKey){
	Node* target = parent->preceding_child(parentKey);
	int nextKeyPlace = parent->indexOf(parentKey) + 1;
	Node* rightSibling;
	if(nextKeyPlace == parent->size()){
		rightSibling = parent->lastChild;
	}else{
		rightSibling = (*parent)[nextKeyPlace].child;
	}
	Key rightFirst = rightSibling->firstKey();
	parent->erase(parentKey);
	parentKey.child = target->lastChild;
	target->insert(parentKey);
	target->lastChild = rightFirst.child;
	if(target->lastChild){
		target->lastChild->parent = target;
	}
	rightSibling->erase(rightFirst);
	rightFirst.child = target;
	parent->insert(rightFirst);
}
Node* BTree::mergeWithChildren(Node* parent, Key parentKey){
	Node* precedingChild = parent->preceding_child(parentKey);
	Node* followingChild = parent->following_child(parentKey);
	assert(precedingChild->size() == t-1);
	assert(followingChild->size() == t-1);
	Node* mergedNode = new Node;
	mergedNode->leaf = precedingChild->leaf;
	for(int i = 0; i < t-1; i++){
		Key modified = (*precedingChild)[i];
		insert_into_node(mergedNode, modified);
	}
	parentKey.child = precedingChild->lastChild;
	insert_into_node(mergedNode, parentKey);
	for(int i = 0; i < t-1; i++){
		Key modified = (*followingChild)[i];
		insert_into_node(mergedNode, modified);
	}
	mergedNode->lastChild = followingChild->lastChild;
	if(mergedNode->lastChild){
		mergedNode->lastChild->parent = mergedNode;
	}
	//erase old nodes
	delete precedingChild;
	delete followingChild;
	int nextKeyPlace = parent->indexOf(parentKey) + 1;
	if(nextKeyPlace == parent->size()){
		parent->lastChild = mergedNode;
	}else{
		Key modified = (*parent)[nextKeyPlace];
		parent->erase(modified);
		modified.child = mergedNode;
		parent->insert(modified);
	}
	parent->erase(parentKey);
	if(parent->empty()){
		if(root == parent){
			root = mergedNode;
			mergedNode->parent = NULL;
		}else{
			Node* ancestor = parent->parent;
			int ancestorKeyPlace = ancestor->indexOf(parentKey);
			if(ancestorKeyPlace < ancestor->size()){
				Key modified = (*ancestor)[ancestorKeyPlace];
				ancestor->erase(modified);
				modified.child = mergedNode;
				ancestor->insert(modified);
			}else{
				ancestor->lastChild = mergedNode;
			}
			mergedNode->parent = ancestor;
		}
		delete parent;
	}else{
		mergedNode->parent = parent;
	}
	return mergedNode;
}

void BTree::printPreOrder() const{
	printHelper(root);
}
void BTree::printHelper(const Node* current) const{
	if(current == NULL){
		return;
	}
	for(int i = 0; i < current->size(); i++){
		cout<<"("<<(*current)[i].attr[0]<<","<<(*current)[i].attr[1]<<","<<(char) (*current)[i].attr[2]<<")";
	}
	cout<<"\n";
	for(int i = 0; i < current->size(); i++){
		Node* curChild = (*current)[i].child;
		if(!current->isLeaf()) assert(curChild); //checking tree validity
		printHelper(curChild);
	}
	if(!current->isLeaf()) assert(current->lastChild);
	printHelper(current->lastChild);
}
void BTree::TreeEraser(Node* current){
	if(current == NULL){
		return;
	}
	for(int i = 0; i < current->size(); i++){
		TreeEraser((*current)[i].child);
	}
	TreeEraser(current->lastChild);
	delete current;
}
BTree::~BTree(){
	TreeEraser(root);
}
int main() {
	BTree tree;
    int n, t;
    unsigned char key;
    cin>>n>>t>>key;
	tree.set_t(t);
    if(key == 'x'){
		key = 0;
	}
    else if(key == 'y'){
		key = 1;
	}
    else if(key == 'z'){
		key = 2;
	}
    //char is also an integer (1 byte)
    for(int i = 0; i < n; i++){
		int x, y;
		unsigned char z;
		cin>>x>>y>>z;
		const int data[] = {x, y, (int)z};
		tree.insert_into_tree(Key(key, data));
    }
	tree.printPreOrder();
    return 0;
}
