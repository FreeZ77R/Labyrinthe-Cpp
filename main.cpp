#include <cstdlib>
#include <iostream>
#include "graph.hpp"
#include <vector>

using namespace std;

//How to compile :
//./graph --x 20 --y 20 --algo fusion

int main(int argc, const char * argv[]){

	//Arguments managing 
    string x(argv[2]);
    string y(argv[4]);
    string algo(argv[6]);
    
    int x_size = stoi(x); //start vertex id
    int y_size = stoi(y); //end vertex id
    vector<int> grid; //path returned by the method

    Graph graph;

    //Launching methods from the arguments (algo) passed as parameters
    //Check if the starting point and the ending point are corrects
    if((x_size != 0 && y_size != 0) && algo == "fusion") grid=graph.fusion(x_size,y_size); 
 	
 	return EXIT_SUCCESS;  
}