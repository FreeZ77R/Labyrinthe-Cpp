#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdint>
#include "graph.hpp"

using namespace std;



//Constructeur qui initialise les cellules du labyrinthe (y,x)
Graph::Graph(int rows, int columns) : nbColumns(columns), nbRows(rows) {
	neighborsList.resize(nbCells);
	int id_=0;
	Cell cell;
	for(int i=0; i<nbColumns; i++){
		for(int j=0; j<nbRows; j++){

		cell.x_coord = j;
		cell.y_coord = i;
		cell.id = id_++;
		cell.visited = 0;
		cout<<"y_coord :"<<cell.y_coord<<"\tx_coord :"<<cell.x_coord<<"\tid :"<<cell.id<<"\tvisited :"<<cell.visited<<endl;
		mazeCells.push_back(cell);

		}
	}	

	//initialise la liste de voisins de chaque cellules
	Graph::init_neighborsList(); 
	Graph::init_walls();

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////  METHODES UTILITAIRE ALGO FUSION   ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Graph::getId(int y,int x){
	for(int i=0; i<nbCells; i++){
	    if(mazeCells[i].y_coord==y && mazeCells[i].x_coord==x){return mazeCells[i].id;}
	}
	return -1;
}

void Graph::changeMultipleId(const int id, const int id_neighbor){
	for(uint32_t i=0; i<this->mazeCells.size(); i++){
		if(this->mazeCells[i].id==id_neighbor){
		this->mazeCells[i].id=id;
		}
	}
}


bool Graph::estValide(int y, int x){
	return (y>=0 && y<nbColumns && x>=0 && x<nbRows);
}


void Graph::init_neighborsList(){

	// Coordonnées des déplacements possibles (haut, bas, gauche, droite)
	int deplacementsX[] = { -1, 1, 0, 0 };
	int deplacementsY[] = { 0, 0, -1, 1 };  
	int saveY,saveX,Y,X;

	for(int i=0; i<nbCells; i++){ 

	    saveY=this->mazeCells[i].y_coord;
	    saveX=this->mazeCells[i].x_coord;

		for(int d = 0; d < 4; d++) {
			Y=saveY;
			X=saveX;
			Y = Y + deplacementsY[d];
			X = X + deplacementsX[d];
			if(estValide(Y, X)){
			    this->neighborsList[i].push_back(getId(Y, X));
			}
		}
	} 
}

void Graph::init_walls(){
	for(int i=0; i<nbCells; i++){
		for(uint32_t j=0; j<this->neighborsList[i].size(); j++){
			this->walls.push_back(make_pair(i, this->neighborsList[i][j]));               
		}
	}
}


//fonction qui verifie si les identifiants de chaque cellules sont identiques
bool Graph::allSameId() {

	int firstId = mazeCells[0].id;
	for (uint32_t i = 1; i < mazeCells.size(); i++) {
		if (mazeCells[i].id != firstId) {
			return false;  // Au moins un élément est différent, donc ils ne sont pas tous identiques
		}
	}

	return true;  // Tous les éléments sont identiques
}

int Graph::getUnvisitedID(const vector<int>& neighborsVisited) {
	int id;
	bool end_while=true;
	while(end_while){
		id = rand() % nbCells;
		if (find(neighborsVisited.begin(), neighborsVisited.end(), id) == neighborsVisited.end()) {
			end_while=false;
		}
	}
	return id;

}

/*
bool alreadyVisited(int id, vector<int> &neighborsVisited){
for(int i=0; i<neighborsVisited.size(); i++){
    if(neighborsVisited[i]==id){
        return true;
    }
}
return false;
}
*/

void Graph::removeWall(int id, int id_neighbor){

	this->walls.erase(
	    remove_if(
		walls.begin(), walls.end(),
		[id, id_neighbor](const pair<int, int>& wall) {
		    return (wall.first == id && wall.second == id_neighbor) || (wall.first == id_neighbor && wall.second == id);
		}
	    ),
	    walls.end()
	);
}


// renvoie un voisin aleatoire de la cellule courante
int Graph::RandNeighbor(int id){

	int rd = rand()%neighborsList[id].size();
	int id_v = neighborsList[id][rd];
	//cout<<"id : "<<id<<"\trd : "<<rd<<"\tnb voisins : "<< neighborsList[id].size()<<"\tid _v : "<<id_v<<endl;

	int firstIdNeighbor = this->mazeCells[neighborsList[id][0]].id;
	uint32_t cpt=0;
	//cout<<firstIdNeighbor<<endl;
	for (uint32_t i = 0; i < this->neighborsList[id].size(); i++) {
		//cout<<"salut"<<endl;
		if (this->mazeCells[this->neighborsList[id][i]].id == firstIdNeighbor) {
			//cout<<"cpt :"<<cpt<<endl;
			cpt++;
			if(cpt==this->neighborsList[id].size()){return id_v;} 
		}
	}

	while(this->mazeCells[id].id == this->mazeCells[id_v].id){
		rd = rand()%this->neighborsList[id].size();
		id_v = this->neighborsList[id][rd];
	}

	return id_v;
}


void Graph::fusion() {

	vector<int> neighborsVisited;
	int id, id_neighbor;

	while (!allSameId()) {
		id = getUnvisitedID(neighborsVisited);


		/// faire des verifications supplémentaire ici 
		//cout<<"bloqué"<<endl;
		id_neighbor = RandNeighbor(id);


		removeWall(id, id_neighbor);
		changeMultipleId(this->mazeCells[id].id, this->mazeCells[id_neighbor].id);

		this->mazeCells[id].visited=true;
		neighborsVisited.push_back(id);
		//cout<<"id : "  <<id <<"\t id voisin : "<<id_neighbor;

		printId();
	}

	cout<<endl;
	for(uint32_t i=0; i<neighborsVisited.size(); i++){
	    cout<<neighborsVisited[i]<<", ";
	}

}



void Graph::aldousBroder() {
	int nbVisited = 1;
	int id = rand() % nbCells;
	this->mazeCells[id].visited = true;

	while (nbVisited < this->nbCells) {
		int id_neighbor = RandNeighbor(id);

		if (!mazeCells[id_neighbor].visited) {
			removeWall(id, id_neighbor);
			this->mazeCells[id_neighbor].visited = true;
			nbVisited++;
	    	}

	    	id = id_neighbor;
	}
}




 



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Affichage des cellules du labyrinthe 
void Graph::printCells() {
	for(int i=0; i<this->nbCells; i++){

		if(i%nbRows==0){cout<<endl;}

		cout<<this->mazeCells[i].y_coord<<","<<this->mazeCells[i].x_coord<<"\t";        

	}
	cout<<endl;
}


// Affichage des voisins de chaque cellules 
void Graph::printNeighborsList() {
	for(int i=0; i<this->nbCells; i++){
		for(int x : this->neighborsList[i]){
			cout<<x<<",";

		} 
		cout<<"\t";       

	}
	cout<<"\n"<<endl;
}

// Affichage entre qu'elles cellules il y a des murs 
void Graph::printWallsId() {
	for(uint32_t i=0; i<this->walls.size(); i++){
		cout<<"walls "<<i<<" : "<<this->walls[i].first<<","<<this->walls[i].second<<"\t";
	}
	cout<<"\n"<<endl;
}






void Graph::printWalls() {
	vector<vector<bool>> wallPositions(this->nbColumns * 2 + 1, vector<bool>(this->nbRows * 2 + 1, false));

	// Mark boundary walls
	for (uint32_t i = 0; i < wallPositions.size(); i++) {
	    wallPositions[i][0] = true;                       // Top boundary
	    wallPositions[i][wallPositions[i].size() - 1] = true;  // Bottom boundary
	}

	for (uint32_t j = 0; j < wallPositions[0].size(); j++) {
	    wallPositions[0][j] = true;                          // Left boundary
	    wallPositions[wallPositions.size() - 1][j] = true;   // Right boundary
	}

	// Mark internal walls
	for (const auto& wall : this->walls) {
	    int id1 = wall.first;
	    int id2 = wall.second;

	    int x1 = id1 / nbRows;
	    int y1 = id1 % nbRows;
	    int x2 = id2 / nbRows;
	    int y2 = id2 % nbRows;

	    int wallX = x1 + x2 + 1;
	    int wallY = y1 + y2 + 1;

	    wallPositions[wallX][wallY] = true;
	}

	// Display the walls in the terminal
	for (uint32_t i = 0; i < wallPositions.size(); i++) {
	    for (uint32_t j = 0; j < wallPositions[i].size(); j++) {
		if (wallPositions[i][j]) {
		    cout << "+";
		} else {
		    cout << " ";
		}
	    }
	    cout << endl;
	}
}




void Graph::printVisited() {
	for(int i=0; i<this->nbCells; i++){

		if(i%this->nbRows==0){cout<<endl;}

		cout<<this->mazeCells[i].visited<<"\t";       

	}
	cout<<endl;
	}

void Graph::printId(){

	for(int j=0; j<this->nbCells; j++){
		if(j%this->nbColumns==0){
			cout<<endl;
		}

	    	cout<<this->mazeCells[j].id<<"\t";

	} 
	cout<<endl;
}








///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
int main() {

    srand (time(NULL));

    int numRows, numCols;
    std::cout << "Entrez le nombre de lignes du labyrinthe : ";
    std::cin >> numRows;
    std::cout << "Entrez le nombre de colonnes du labyrinthe : ";
    std::cin >> numCols;

    Graph graph(numRows, numCols);
    graph.printNeighborsList();
    graph.printWallsId();
    //graph.printWalls();
    graph.printId();
    graph.printCells();
    //graph.fusion();
    graph.aldousBroder();
    graph.printWallsId();
    graph.printWalls();
    graph.printId();
    graph.printVisited();




    return EXIT_SUCCESS;
}
*/

    


