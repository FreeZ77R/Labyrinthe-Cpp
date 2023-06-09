#pragma once

#include <vector>
#include <utility>


using namespace std;

// Chaque case du labyrinthe est une cellule dont on connait la position(x,y), l'id et si elle est visité 
struct Cell {
	int x_coord;
	int y_coord;
	int id;
	bool visited;
};

class Graph {
protected:
	int nbColumns; // Nombre de ligne dans le labyrinthe
	int nbRows; // Nombre de colonnes dans le labyrinthe
	int nbCells; // Nombre de cellules au total dans me labyrinthe
	vector<Cell> mazeCells; // Stocke toutes les cellules du labyrinthe et leurs attributs
	vector<vector<int>> neighborsList; // Liste des voisins de chaque cellules à partir de l'id
	vector<pair<int, int>> walls; // Liste des murs présent dans le labyrinthe à partir de l'id

public:

	//Constructeur
	Graph(int rows, int columns);
	
	/////////////////////////////////  GETTERS  ////////////////////////////////////
	int getNbRows() const;
	int getNbColumns() const;
	int getId(int y, int x) const;
	pair<int,int> getXY(int id1, int id2, int nbRows, int nbColumns, int window_w, int window_h);
	vector<Cell> getCells() const;
	vector<pair<int, int>> getWalls() const;
	vector<int> getSolution() const;
	
	/////////////////////  METHODES UTILITAIRE ALGO    /////////////////////////////
	bool withoutWalls(const int current_id, const int next_id) const;
	void changeMultipleId(const int id, const int id_neighbor);
	bool estValide(int y, int x);
	void init_neighborsList();
	void init_walls();
	bool allSameId();
	void removeWall(int id, int id_neighbor);
	int RandNeighbor(int id);
	void fusion();
	void aldousBroder();
	
	/////////////////// FONCTIONS D'AFFICHAGE UTILE POUR TESTER ////////////////////
	void printCells();
	void printNeighborsList();
	void printWallsId();
	void printWalls();
	void printVisited();
	void printId();
};



