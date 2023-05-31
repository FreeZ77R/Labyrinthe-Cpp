#pragma once

#include <vector>
#include <utility>


using namespace std;

struct Cell {
	int x_coord;
	int y_coord;
	int id;
	bool visited;
};

class Graph {
protected:
	int nbColumns;
	int nbRows;
	int nbCells;
	vector<Cell> mazeCells;
	vector<vector<int>> neighborsList;
	vector<pair<int, int>> walls;

public:

	Graph(int rows, int columns);
	
	int getNbRows() const;
	int getNbColumns() const;
	int getId(int y, int x) const;
	vector<Cell> getCells() const;
	vector<pair<int, int>> getWalls() const;
	void changeMultipleId(const int id, const int id_neighbor);
	bool estValide(int y, int x);
	void init_neighborsList();
	void init_walls();
	bool allSameId();
	int getUnvisitedID(const vector<int>& neighborsVisited);
	void removeWall(int id, int id_neighbor);
	int RandNeighbor(int id);
	void fusion();
	void aldousBroder();
	void printCells();
	void printNeighborsList();
	void printWallsId();
	void printWalls();
	void printVisited();
	void printId();
};



