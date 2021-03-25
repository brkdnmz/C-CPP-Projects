/*
    Berke Dönmez
    150170045
*/

#include <iostream>
#include <fstream>  //FILE I/O
#include <string>   //std::string
#include <cstdlib>  //atoi, exit()
#include <cassert>  //Used assert to ensure tree is valid
using namespace std;
//defining the name of colors as boolean variables
#define BLACK 0 
#define RED 1
const string colorName[] = {"BLACK", "RED"}; //stores the name of colors

class Player{ //Node class
private:
    string name;    //player's full name
    int rebound;    //total number of rebounds
    int assist;     //total number of assists
    int point;      //total points
    Player* left;   //left "child"
    Player* right;  //right "child"
    Player* parent; //"parent"
    bool color; //0 -> BLACK, 1 -> RED
public:
    friend class Tree;  //let Tree class reach privates of Player class
    Player(){
        left = NULL;
        right = NULL;
        parent = NULL;
        color = BLACK; //paint it black, initially (will be painted to red before insertion)
    }
    Player(const string name, const int rebound=0, const int assist=0, const int point=0) 
        : name(name), rebound(rebound), assist(assist), point(point) {
        left = NULL;
        right = NULL;
        parent = NULL;
        color = BLACK;
    }
    Player(const Player& other) 
        : name(other.name), rebound(other.rebound), assist(other.assist), point(other.point) { //only copies the name and stats
        left = NULL;
        right = NULL;
        parent = NULL;
        color = BLACK;
    }

    void update(const int point, const int assist, const int rebound){
        //updates the player (adds new stats to old stats)
        this->point += point;
        this->assist += assist;
        this->rebound += rebound;
    }

    void paint(const bool color){
        //paint to desired color
        this->color = color;
    }

    //operator overloads
    bool operator<(const Player& other) const{
        return name < other.name;
    }
    bool operator==(const Player& other) const{
        return name == other.name;
    }
    bool operator>(const Player& other) const{
        return name > other.name;
    }
    bool operator<=(const Player& other) const{
        return !(*this > other);
    }
    bool operator>=(const Player& other) const{
        return !(*this < other);
    }
    //no need for destructor overriding
};

class Tree{ //RB Tree class
private:
    Player* root;           //always stores the root's address
    string pointLeader;     //point leader's name
    string assistLeader;    //assist leader's name
    string rebLeader;       //rebound leader's name
    int maxPoint;           //point of the point leader
    int maxAssist;          //num. of assists of the assist leader
    int maxReb;             //num. of rebounds of the rebound leader
public:
    //default constructor
    //max's are initialized with -1 in order to make sure
    //leader's are set even if their stats are 0
    Tree() : root(NULL), maxPoint(-1), maxAssist(-1), maxReb(-1){}

    //self-explanatory getters
    const Player* getRoot() const{
        return root;
    }
    const string get_pointLeader() const{
        return pointLeader;
    }
    const string get_assistLeader() const{
        return assistLeader;
    }
    const string get_rebLeader() const{
        return rebLeader;
    }
    int get_maxPoint() const{
        return maxPoint;
    }
    int get_maxAssist() const{
        return maxAssist;
    }
    int get_maxReb() const{
        return maxReb;
    }

    Player* operator[](const string targetName) const{
        //returns the address of node with name attribute equal to target[name]
        return find(root, targetName);
    }
    //returns the address of node with name attribute equal to target[name]
    Player* find(Player* current, const string targetName) const;

    void rotateRight(Player* node); //rotate "node"s subtree right
    void rotateLeft(Player* node);  //rotate "node"s subtree left
    void insert(const Player& player);  //insert a new node

    Player* getParent(Player* node) const;  //returns node's parent
    Player* getGrandParent(Player* node) const; //return node's parent's parent
    Player* getUncle(Player* node) const;   //returns parent's sibling

    void print_inOrder(const Player* current, const Player* parent) const;  //my choice to implement
    void print_preOrder(const Player* current, int depth = 0) const;        //print the whole tree pre-order
    void setLeaders(const Player* current);     //reset the leaders when new season begins
    
    int checkIfOK(const Player* current) const; //check if tree is valid (my choice to implement)
    
    void TreeEraser(Player* eraser);    //frees the memory allocated for the nodes with "new"
    ~Tree(){
        TreeEraser(root);
    }
};

void endOfSeason(Tree& tree, const int seasonStart){    //prints the leaders when a season ends
    int seasonEnd = seasonStart + 1;
    cout << "End of the " << seasonStart << "-" << seasonEnd << " Season\n";
    tree.setLeaders(tree.getRoot());
    cout << "Max Points: " << tree.get_maxPoint() << " - " << "Player Name: " << tree.get_pointLeader() << "\n";
    cout << "Max Assists: " << tree.get_maxAssist() << " - " << "Player Name: " << tree.get_assistLeader() << "\n";
    cout << "Max Rebs: " << tree.get_maxReb() << " - " << "Player Name: " << tree.get_rebLeader() << "\n";
}
int main(int argCount, char** args){
    if(argCount != 2){
        cerr << "Please give me the file to be read ^^\n";
        exit(EXIT_FAILURE);
    }
    Tree tree;  //construct the tree object
    ifstream input(args[1]);    //input file stream
    string foo;
    input >> foo;   //read the first useless line
    string buffer;  //for keeping the read strings
    int prevSeason = -1;    //keeps the previous season's starting year
    int firstSeason = -1;   //keeps the first season's starting year
    for(int i = 0; !input.eof(); i++){
        getline(input, buffer, '-'); //read starting year of the season
        int curSeason = atoi(buffer.c_str());   //convert the year to int
        if(i == 0){ //reading has started, "initialize" these variables
            firstSeason = curSeason;
            prevSeason = curSeason;
        }
        bool newSeason = curSeason != prevSeason; //checks whether a season has just ended
        if(newSeason){  //new season has begun
            endOfSeason(tree, prevSeason);  //print the leaders
            if(firstSeason == prevSeason){  //print the tree only if the first season has ended
                tree.print_preOrder(tree.getRoot());
            }
        }
        prevSeason = curSeason; //update previous season
        getline(input, buffer, ','); //read ending year of the season, unnecessary
        getline(input, buffer, ','); //read name
        string name = buffer;        //name is a string
        getline(input, buffer, ','); //read team, unnecessary
        getline(input, buffer, ','); //read rebound
        int rebound = atoi(buffer.c_str()); //convert rebound to int
        getline(input, buffer, ',');        //read assist
        int assist = atoi(buffer.c_str());  //convert assist to int
        getline(input, buffer, '\n');       //read point
        int point = atoi(buffer.c_str());   //convert point to int

        Player* finder = tree[name];
        if(finder != NULL){ //this player is not new
            finder->update(point, assist, rebound);
        }else{              //this player is new
            tree.insert(Player(name, rebound, assist, point));
        }
        tree.checkIfOK(tree.getRoot());
    }
    //do not forget to consider the last season, it was not considered in loop
    endOfSeason(tree, prevSeason);
    if(firstSeason == prevSeason){
        tree.print_preOrder(tree.getRoot());
    }
    input.close();
    return 0;
}
/***********************************    MEMBER FUNCTIONS OF Tree    ******************************/
Player* Tree::find(Player* current, const string targetName) const{
    //returns the address of player named targetName
    //if not exists, return NULL
    if(!current){
        return NULL;
    }
    if(targetName < current->name){          //targetName is smaller (lexicographically) than current name
        return find(current->left, targetName);
    }else if(targetName > current->name){    //targetName is bigger than current name
        return find(current->right, targetName);
    }else{  //targetName is equal to current name
        return current;
    }
}

void Tree::rotateRight(Player* node){
    //rotates the subtree rooted at node right
    //please notice that addresses of nodes will not change,
    //only connections change.
    
    //function is called only when b exists.
    /*
             a           b
            / \        /  \
           b  e  -->  c    a
          / \             / \
         c  d            d   e
    */
    Player* a = node;
    Player* b = a->left;
    Player* d = b->right;
    //reconfigure the child connections
    b->right = a;
    a->left = d;
    if(a->parent != NULL){ //do not forget a's parent
        if(a->parent->left == a){
            a->parent->left = b;
        }else{
            a->parent->right = b;
        }
    }
    /* special case: a == root */
    if(a == root){
        root = b;
    }
    //now, configure parents (parents have not been changed yet)
    b->parent = a->parent;
    a->parent = b;
    if(d != NULL) d->parent = a;
}

void Tree::rotateLeft(Player* node){
    //rotates the subtree rooted at node left
    //please notice that addresses of nodes will not change,
    //only connections change.
    //this function is the inverse of rotateRight.

    //it is called only when a exists.
    /*
            b                 a 
        /  \               / \
        c    a    -->      b  e
            / \           / \  
            d   e         c  d  
    */
    Player* b = node;
    Player* a = b->right;
    Player* d = a->left;
    //reconfigure the child connections
    a->left = b;
    b->right = d;
    if(b->parent != NULL){ //do not forget b's parent
        if(b->parent->left == b){
            b->parent->left = a;
        }else{
            b->parent->right = a;
        }
    }
    /* special case: b == root */
    if(b == root){
        root = a;
    }
    //now, configure parents (parents have not been changed yet)
    a->parent = b->parent;
    b->parent = a;
    if(d != NULL) d->parent = b;
}

void Tree::insert(const Player& player){
    if(root == NULL){ //tree is empty
        //PLayer constructor sets the initial color to black
        root = new Player(player);
        return;
    }

    Player* finder = root; //will find the right place for the newcomer
    //a regular BST search algorithm (a bit customized)
    while(true){ //Ending conditions are decided inside
        if(player < *finder){   //player's name is less than finder's name
            if(finder->left == NULL){   //destination has been reached
                finder->left = new Player(player);  //create the new node
                finder->left->parent = finder;      //assign new node's parent
                finder = finder->left;              //go to new node
                break;
            }
            else{
                finder = finder->left;
            }
        }else{  //player's name is greater than finder's name
            if(finder->right == NULL){  //destination has been reached
                finder->right = new Player(player); //create the new node
                finder->right->parent = finder;     //assign new node's parent
                finder = finder->right;             //go to new node
                break;
            }
            else{
                finder = finder->right;
            }
        }
    }
    //now, finder points to the inserted player
    finder->paint(RED); //paint the new node red
    Player* current = finder;   //this will be the violation tracker node
    //fix the violations now
    while(current->parent != NULL){
        Player* parent = getParent(current);
        if(current->color == BLACK || parent->color == BLACK){
            //if there is no violation, continue
            current = current->parent;
            continue;
        }
        //now, both current and parent are red
        //grandparent is guaranteed to exist (parent is red)
        Player* uncle = getUncle(current);
        Player* grandParent = getGrandParent(current);
        if(uncle != NULL && uncle->color == RED){
            //if uncle's color is red,
            //paint parent and uncle black
            //paint grandparent red
            parent->color = BLACK;
            uncle->color = BLACK;
            grandParent->color = RED;
            if(root->color == RED){ //grandparent might be root
                root->color = BLACK;
            }
        }else{  //only current and parent are red (uncle is black or leaf (also black))
            //There are four cases. They are visualized.
            if(parent == grandParent->left){
                if(current == parent->left){
                    /*
                            GP
                           / \
                          P   U
                         /
                        C   
                    */
                    rotateRight(grandParent);
                    /*
                         P
                        / \
                       C  GP
                            \
                            U
                    */
                    //now, recolor P and GP
                    //after rotation, pointers still point to same objects
                    parent->color = BLACK;
                    grandParent->color = RED;
                }else{
                    /*
                            GP
                           / \
                          P   U
                          \
                           C   
                    */
                    rotateLeft(parent);
                    //now, it is the same as the previous case (but P and C are swapped)
                    rotateRight(grandParent);
                    current->color = BLACK;
                    grandParent->color = RED;
                }
            }else{
                if(current == parent->right){
                    /*
                         GP
                        / \
                       U   P
                            \
                            C
                    */
                    rotateLeft(grandParent);
                    /*
                            P
                           / \
                         GP  C
                        /
                       U   
                    */
                    //now, recolor P and GP
                    parent->color = BLACK;
                    grandParent->color = RED;
                }else{
                    /*
                          GP
                         / \
                        U   P
                           /
                          C
                    */
                    rotateRight(parent);
                    //now, it is the same as the previous case (but P and C are swapped)
                    rotateLeft(grandParent);
                    current->color = BLACK;
                    grandParent->color = RED;
                }
            }
            //there cannot be any violations left, just break
            break;
        }
        current = current->parent; //last code of the loop, go upper
    }
}

Player* Tree::getParent(Player* node) const{
    return node->parent;
}
Player* Tree::getGrandParent(Player* node) const{
    return getParent(getParent(node));
}
Player* Tree::getUncle(Player* node) const{ //get parent's sibling
    Player* parent = node->parent;
    Player* grandParent = parent->parent;
    if(grandParent->left == parent) return grandParent->right;
    else return grandParent->left;
}

void Tree::print_inOrder(const Player* current, const Player* parent) const{ //can be ignored
    if(!current) return;
    print_inOrder(current->left, current);
    cout<<current->name<<" "<<(parent ? parent->name : "-")<<" "<<current->color<<"\n";
    print_inOrder(current->right, current);
}
void Tree::print_preOrder(const Player* current, int depth) const{
    if(!current) return;
    for(int i = 0; i < depth; i++){
        cout<<"-";
    }
    cout<<"("<<colorName[current->color]<<") "<<current->name<<"\n";
    print_preOrder(current->left, depth+1);
    print_preOrder(current->right, depth+1);
}
void Tree::setLeaders(const Player* current){ //finds the leaders (all-in-one function)
    if(!current) return;

    if(current->point > maxPoint){
        maxPoint = current->point;
        pointLeader = current->name;
    }
    if(current->assist > maxAssist){
        maxAssist = current->assist;
        assistLeader = current->name;
    }
    if(current->rebound > maxReb){
        maxReb = current->rebound;
        rebLeader = current->name;
    }
    setLeaders(current->left);
    setLeaders(current->right);
}
int Tree::checkIfOK(const Player* current) const{ //validates the properties of RB Tree
    if(!current) return 0;
    int left = checkIfOK(current->left) + (current->left == NULL || current->left->color == BLACK);
    int right = checkIfOK(current->right) + (current->right == NULL || current->right->color == BLACK);
    assert(left == right);
    if(current == root) assert(current->color == BLACK);
    if(current->color == RED) assert(current->parent->color == BLACK);
    return left;
}
void Tree::TreeEraser(Player* eraser){
    if(!eraser) return;
    TreeEraser(eraser->left);
    TreeEraser(eraser->right);
    delete eraser;
}