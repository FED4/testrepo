/*
	First name	YIYANG
	Last name		FEI
	Student ID	01510275
	Username		yf4418
*/

//IMPORTANT: before submitting your source code, please make sure to comment your main function (needed for automated testing)


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <cmath>
#include <ctime>
#include <chrono>

// do not use using namespace std

// do not alter the struct declaration
struct hufftreenode {
	char character;
	int frequency;
	hufftreenode* left;
	hufftreenode* right;
};

// do not alter the typedef
typedef hufftreenode* hufftreeptr;


// you can define and use any additional *structures* if you might need to

// YOUR CODE HERE FOR ADDIIONAL STRUCTS DECLARATION, IF ANY



/////////////////////////////////////////
// Functions declarations (prototypes) //
/////////////////////////////////////////
void insert_sorted(std::list<hufftreeptr>& forestlist, hufftreeptr& newnode);

void traverse_code(hufftreeptr node, std::string code, std::map<char, std::string>& hufftable);

void print_list(std::list <hufftreeptr> g);

void print_tree(hufftreeptr node, int depth);

void print_map(std::map<char,int> thismap);

void print_map_string(std::map<char,std::string> thismap);

void delete_tree(hufftreeptr t);
// do not alter these functions declarations

//utility function
bool valid_hufftree(const hufftreeptr hufftree);
 // returns true if the input hufftree is a valid Huffman tree
 // i.e. all the terminal nodes (leaves) have characters,
 // all the non-leaf nodes have two sub-trees each,
 // and the occurence frequency of a non-leaf node equals
 // the sum of the occurence frequencies of its two sub-trees.



std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable);
// encodes an input message (a string of characters) into an encoded message (string of bits) using the Huffman coding
// builds the Huffman tree and its associated encoding table (i.e. the association between the characters in the tree leaves and their encodings (string of bits))

std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree);
// decodes an encoded message (a string of bits) into the original message (a string of characters) using the Huffman tree built during the encoding



// you can define and use any additional *functions* if you might need to

// YOUR CODE HERE FOR ADDIIONAL FUNCTIONS DECLARATION (NOT THE IMPLEMENTATION), IF ANY




///////////////////
// main function //
///////////////////

//IMPORTANT (Reminder): before submitting your source code, please make sure to comment your main function (needed for automated testing)
int main() {

	// YOUR CODE HERE FOR TESTING YOUR PROGRAM

	int length = 256, unit_count=2;//2^8
	int unit = 3, longest;

  double clock_ticks_max = 0 ,current_clock_ticks = 0 ,clock_ticks_average = 0, clock_ticks_sum = 0;


				// an example of basic encoding/decoding
std::string message, encodedmessage="", decodedmessage;
hufftreeptr hufftree;
std::map<char, int> freqtable;
std::map<char, std::string> hufftable;


	for(length = 10; length < 2200000; length*=1.2)
	{

		for(unit_count = 1; unit_count < length/unit; unit_count++)
		{
			//create test message
			int increment = 0;
			std::string test_message;
			for(int i = 0; i < length; i++){
				if(i < unit_count*unit){
					if(i >= unit_count*increment){
						increment++;
					}
					test_message.push_back('!'+increment);
				}else{
						test_message.push_back('!');
				}
			}

  		//std::cout << message << std::endl;
			message = test_message;

			auto t_start = std::chrono::high_resolution_clock::now();

			encodedmessage = huffencode(message, freqtable, hufftree, hufftable);

			auto t_end = std::chrono::high_resolution_clock::now();
			double current_clock_ticks = std::chrono::duration<double,std::milli>(t_end-t_start).count();

			longest = hufftable[freqtable.begin()->first].length();//deepest leave

			clock_ticks_sum = clock_ticks_sum + current_clock_ticks;
			if(current_clock_ticks > clock_ticks_max){
				clock_ticks_max = current_clock_ticks;
			}

			decodedmessage = huffdecode(encodedmessage, hufftree);

		}

		std::cout << "unit," << unit << ",unit_count," << unit_count << ",length," << length << ",valid," << valid_hufftree(hufftree) << ",OK," << (decodedmessage == message)<<",messagelength,"<< encodedmessage.length() << ",longest," << longest << ",clock_t_max=," << clock_ticks_max << ",clock_ticks_sum," << clock_ticks_sum << std::endl;

		delete_tree(hufftree);
		hufftable.clear();
		freqtable.clear();

		clock_ticks_max = 0;
		current_clock_ticks = 0;
		clock_ticks_sum = 0;
	}

return 0;
}



/////////////////////////////////////////////
// Functions definitions (implementations) //
/////////////////////////////////////////////

// YOUR CODE HERE FOR THE FUNCTIONS IMPLEMENTATIONS
std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable){
//std::cout << "enter" << std::endl;//test
//(1)Construct freqtable by counting
	for(int char_i = 0; char_i < message.length(); char_i++){
		freqtable[message[char_i]]++;
	}
	 //std::cout << "freqtable" << std::endl;//test
	 //print_map(freqtable);//test

	//sort? compare with tree to insert change sorting method
	//construct new node for each character
	std::map<char, int>::iterator it_node;
	std::list <hufftreeptr> forestlist;
	for(it_node = freqtable.begin(); it_node != freqtable.end(); it_node++)
	{//each char in freqtable -> newnode
		 hufftreeptr newnode;
		 newnode = new hufftreenode;
		 newnode->character = it_node->first;
		 newnode->frequency = it_node->second;
		 newnode->left = NULL;
		 newnode->right = NULL;
		 //insert into list keep sorted
		 insert_sorted(forestlist, newnode);
	}
	  //std::cout << "list" << std::endl;//test
	//print_list(forestlist);//test

//(2)Construct hufftree from nodes
	while(forestlist.size()>1)
	{//tree not complete
		//copy and pop child nodes in the front
		hufftreeptr left=forestlist.front();
		forestlist.pop_front();
		hufftreeptr right=forestlist.front();
		forestlist.pop_front();
		//create new parent node
		hufftreeptr newnode;
		newnode = new hufftreenode;
		newnode->character='<';//test
		newnode->frequency = left->frequency + right->frequency;
		newnode->left = left;
		newnode->right = right;
		//insert the subtree to the right position in forestlist
		insert_sorted(forestlist, newnode);
	}
	hufftree = forestlist.front();
	//print_tree(hufftree,0);//test

//(3)Construct hufftable for encoding from trees
	std::string code="";
	traverse_code(hufftree,code,hufftable);
  //print_map_string(hufftable);//test

//(4)encoded message using hufftable
	std::string encodedmessage="";
	for(int char_j = 0; char_j < message.size(); char_j++){
		encodedmessage+=hufftable[message[char_j]];
	}

	return encodedmessage;
}
// encodes an input message (a string of characters) into an encoded message (string of bits) using the Huffman coding
// builds the Huffman tree and its associated encoding table (i.e. the association between the characters in the tree leaves and their encodings (string of bits))

void insert_sorted(std::list<hufftreeptr>& forestlist, hufftreeptr& thisnode)
{//min -> max
	//std::cout << "enter insert sorted" << std::endl;
	//std::cout << thisnode->character << thisnode->frequency << std::endl;
	int freq = thisnode->frequency;
	std::list<hufftreeptr>::iterator it_end = forestlist.end();
	it_end--;
	//insert to before the 1st elfreq>freq
		if(forestlist.size() == 0){
			forestlist.push_back(thisnode);
		}else if((*it_end)->frequency <= freq){
			forestlist.push_back(thisnode);
		}else{
			std::list<hufftreeptr>::iterator it = forestlist.begin();
			//std::cout << (*it)->frequency << ">=" << freq << std::endl;//test
			while((*it)->frequency < freq){
				it++;
			}
			forestlist.insert(it,thisnode);
		}

	return;
}

void traverse_code(hufftreeptr node, std::string code, std::map<char, std::string>& hufftable)
{
	if(node->left == NULL){//and right == NULL for leave as BASE CASE
		if(code == ""){code = '1';}//Specialcase for 1 el
		hufftable[node->character] = code;
	}else{//not leave
		traverse_code(node->left,code+"0",hufftable);
		traverse_code(node->right,code+"1",hufftable);
	}
}

bool valid_hufftree(const hufftreeptr hufftree)
{
	bool isThisnodeValid;
//(1)traverse all nodes
	if((hufftree->left == NULL) && (hufftree->right == NULL)){//leaves char=""
		return (hufftree->character != '\0');//empty char for leave
	}else if((hufftree->left != NULL) && (hufftree->right != NULL)){//parent
		isThisnodeValid =  (hufftree->frequency == (hufftree->left->frequency + hufftree->right->frequency));
			// and the occurence frequency of a non-leaf node equals
		  // the sum of the occurence frequencies of its two sub-trees.
	}else{//only one subtree
		return false;
	}
	if(!isThisnodeValid){
		return false;
	}
	return (valid_hufftree(hufftree->left) && valid_hufftree(hufftree->right));
}

std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree)
{
	//std::cout << "enter decode" << encodedmessage.length() << std::endl;//test
	std::string recoveredmessage;
	hufftreeptr tmpptr = hufftree;
	for(int string_index = 0; string_index < encodedmessage.length(); string_index++){
			if(encodedmessage[string_index] == '0' && tmpptr->left != NULL){
				tmpptr=tmpptr->left;
				//std::cout << "left" << std::endl;//test
			}else if(tmpptr->left != NULL){//in case single node tree
				tmpptr=tmpptr->right;
				//std::cout << "right" << std::endl;//test
			}

			if(tmpptr->left == NULL){//prefix
				recoveredmessage.push_back(tmpptr->character);
				//std::cout << tmpptr->character << std::endl;//test
				tmpptr = hufftree;//back to root
			}
	}
	return recoveredmessage;
}

void print_list(std::list<hufftreeptr> g)
{
    std::list <hufftreeptr> :: iterator it;
    for(it = g.begin(); it != g.end(); ++it)
        std::cout << '\t' << (*it)->character << " " << (*it)->frequency << std::endl;
}

void print_map(std::map<char,int> thismap)
{
	std::map<char,int>::iterator it;
	for (it = thismap.begin(); it != thismap.end(); it++) {
		std::cout << "(" << it->first << ", " << it->second << ")" << std::endl;
	}
}

void print_map_string(std::map<char,std::string> thismap)
{
	std::map<char,std::string>::iterator it;
	for (it = thismap.begin(); it != thismap.end(); it++){
		std::cout << "(" << it->first << ", " << it->second << ")" << std::endl;
	}
}

void print_tree(hufftreeptr node, int depth)
{
	if(node == NULL){
		return;
	}
	print_tree(node->left, depth+1);
	for(int i = 0; i <= depth; i++){
		std::cout << "     ";
	}
	std::cout << "(" << node->frequency << ")" << node->character<< std::endl;
	print_tree(node->right, depth+1);
}

void delete_tree(hufftreeptr t){
  if(t != NULL){
    delete_tree(t->left);
    delete_tree(t->right);
    delete t;
  }
}
