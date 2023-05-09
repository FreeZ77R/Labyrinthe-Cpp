#include "graph.hpp"
#include "iostream"
#include <vector>


using namespace std;

vector<int> Graph::fusion(int x_size, int y_size){

	cout<<"x :"<<x_size<<"   y :"<<y_size<<endl;
	int grid_size=x_size*y_size;

	cout<<"grid :"<<grid_size<<endl;

	vector<int> grid;

	for(int i=0; i<grid_size; i++){
		grid.push_back(i);
	}


	for(int i=0; i<y_size; i++){
		for(int j=0; j<x_size; j++){
			
			cout<<grid[i*x_size+j]<<"\t";

		}
		cout<<endl;
	}

	return grid;
}


