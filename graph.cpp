#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdint>
#include "graph.hpp"
#include <queue>

using namespace std;



//Constructeur qui initialise les cellules du labyrinthe (y,x)
Graph::Graph(int rows, int columns) : nbColumns(columns), nbRows(rows) {
	this->nbCells=nbRows*nbColumns;
	neighborsList.resize(this->nbCells);
	int id_=0;
	Cell cell;
	for(int i=0; i<nbColumns; i++){
		for(int j=0; j<nbRows; j++){

		cell.x_coord = j;
		cell.y_coord = i;
		cell.id = id_++;
		cell.visited = 0;
		mazeCells.push_back(cell);

		}
	}	

	//initialise la liste de voisins de chaque cellules
	Graph::init_neighborsList(); 
	Graph::init_walls();

}


//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////  GETTERS  ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

//Renvoie le nombre de ligne 
int Graph::getNbRows() const{
	return this->nbRows;
}

//Renvoie le nombre de colonne
int Graph::getNbColumns() const{
	return this->nbColumns;
}

//Renvoie l'id d'une cellule à partir de ses coordonnées
int Graph::getId(int y,int x) const{
	for(int i=0; i<nbCells; i++){
	    if(this->mazeCells[i].y_coord==y && this->mazeCells[i].x_coord==x){return this->mazeCells[i].id;}
	}
	return -1;
}

//Renvoie les coordonées xy d'affichage en deux cellules (id)
pair<int,int> Graph::getXY(int id1, int id2, int nbRows, int nbColumns) {
	pair<int,int> xy = make_pair(-1, -1);;
	
	for (int i = 0; i < nbRows; i++) {
		for (int j = 0; j < nbColumns; j++) {
		    if (id2 == id1 + 1) { // Adjacent horizontalement
			if (id2 == i * nbColumns + j) {
			    xy = std::make_pair((400 / nbColumns) * j, (400 / nbRows) * i);
			    return xy;
			}
		    } else if (id2 == id1 + nbColumns) { // Adjacent verticalement
			if (id2 == i * nbColumns + j) {
			    xy = std::make_pair((400 / nbColumns) * j, (400 / nbRows) * i);
			    return xy;
			}
		    }
		}
	}
	// Retourner une valeur par défaut si les cellules ne sont pas adjacentes
	return xy;	
}

//Renvoie les cellules du labyrinthe
vector<Cell> Graph::getCells() const{
	return this->mazeCells;
}

//Renvoie les murs du labyrinthe
vector<pair<int, int>> Graph::getWalls() const{
	return this->walls;
}

//Renvoie la solution du labyrinthe sous forme d'id contenus dans un vecteur
vector<int> Graph::getSolution() const {
    //Position de départ et d'arrivé du labyrinthe
    int startId = 0;
    int endId = nbCells - 1;

    // File pour stocker les cellules à visiter
    queue<int> q;
    // Tableau pour stocker les cellules visitées
    vector<bool> visited(nbCells, false);
    // Tableau pour stocker les prédécesseurs de chaque cellule
    vector<int> predecessors(nbCells, -1);

    // Marquer la cellule de départ comme visitée
    visited[startId] = true;
    // Ajouter la cellule de départ à la file
    q.push(startId);

    // Parcours BFS
    while (!q.empty()) {
        int currentId = q.front();
        q.pop();

        // Vérifier si la cellule actuelle est la cellule de destination
        if (currentId == endId) {
            break;  // Sortir de la boucle si la cellule de destination est atteinte
        }

        // Parcourir les voisins de la cellule actuelle
        for (int neighborId : neighborsList[currentId]) {
            // Vérifier si le voisin n'est pas visité et s'il n'y a pas de mur entre les deux cellules
            if (!visited[neighborId] && withoutWalls(currentId, neighborId)) {
                visited[neighborId] = true;  // Marquer le voisin comme visité
                predecessors[neighborId] = currentId;  // Stocker le prédécesseur du voisin
                q.push(neighborId);  // Ajouter le voisin à la file pour le visiter plus tard
            }
        }
    }

    // Reconstituer le chemin à partir des prédécesseurs
    vector<int> path;
    int currentId = endId;
    while (currentId != -1) {
        path.push_back(currentId);
        currentId = predecessors[currentId];
    }
    reverse(path.begin(), path.end());  // Inverser le chemin pour obtenir l'ordre correct

    return path;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////  METHODES UTILITAIRE ALGO FUSION   ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Indique s'il y a un mur présent entre deux id
bool Graph::withoutWalls(const int current_id, const int next_id) const{

	pair<int,int> xy = make_pair(current_id, next_id);

	if (find(walls.begin(), walls.end(), xy) == walls.end()) {
		return true;
	}
	return false;	
}


//Permet de changer plusieurs id des cellules
void Graph::changeMultipleId(const int id, const int id_neighbor){
	for (auto& cell : mazeCells) {
		if (cell.id == id_neighbor) {
		    cell.id = id;
		}
	}
}

//Indique si les coordonnées sont valables
bool Graph::estValide(int y, int x){
	return (y>=0 && y<nbColumns && x>=0 && x<nbRows);
}

//Initialise la liste des voisins de chaque cellule
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

//Initialise murs à partir de la liste des voisins
void Graph::init_walls(){
	for(int i=0; i<nbCells; i++){
		for(uint32_t j=0; j<this->neighborsList[i].size(); j++){
			this->walls.push_back(make_pair(i, this->neighborsList[i][j]));               
		}
	}
}


//Fonction qui verifie si les identifiants de chaque cellules sont identiques
bool Graph::allSameId() {

	int firstId = mazeCells[0].id;
	for (uint32_t i = 1; i < mazeCells.size(); i++) {
		if (mazeCells[i].id != firstId) {
			return false;  // Au moins un élément est différent, donc ils ne sont pas tous identiques
		}
	}

	return true;  // Tous les éléments sont identiques
}

//Retire un mur de la liste de murs à partir de deux id 
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


//Renvoie un voisin aleatoire de la cellule courante
int Graph::RandNeighbor(int id) {

    int rd = rand() % neighborsList[id].size();
    int id_v = neighborsList[id][rd];

    if (mazeCells[id].id != mazeCells[id_v].id) {
        return id_v; // Retourne l'ID du voisin si les identifiants sont différents
    }
	
    
    // Recherche d'un voisin avec un identifiant différent
    for (uint32_t i = 0; i < neighborsList[id].size(); i++) {
        int neighborId = neighborsList[id][i];
        if (mazeCells[id].id != mazeCells[neighborId].id) {
            return neighborId; // Retourne le premier voisin trouvé avec un identifiant différent
        }
    }

    return -1; // Aucun voisin avec un identifiant différent trouvé, retourne l'ID actuel
}




//Algorithme Fusion : création de labyrinthe suivant le cahier des charges suivant :
//On part d’un labyrinthe dont tous les murs sont fermés,
//On associe une valeur unique (un identifiant) à chaque cellule,
//À chaque itération, on choisit un mur à ouvrir de manière aléatoire,
//Lorsqu’un mur est ouvert entre deux cellules adjacentes, les deux cellules sont liées entre elles et forment un « chemin ».
//À chaque fois que l’on tente d’ouvrir un passage entre deux cellules, on vérifie que ces deux cellules ont des identifiants différents.
//Si les identifiants sont identiques, c’est que les deux cellules sont déjà reliées et appartiennent donc au même chemin. On ne peut donc pas ouvrir le passage.
//Si les identifiants sont différents, le passage est ouvert, et l’identifiant de la première cellule est affecté à toutes les cellules du second chemin.
void Graph::fusion() {

	vector<int> neighborsVisited;
	int id, id_neighbor;

	while (!allSameId()) {
		id = rand()%nbCells;
		id_neighbor = RandNeighbor(id);
		
		if(id_neighbor!=-1){
			removeWall(id, id_neighbor);
			changeMultipleId(this->mazeCells[id].id, this->mazeCells[id_neighbor].id);

			this->mazeCells[id].visited=true;
			neighborsVisited.push_back(id);
		}
	}

}


//Algorithme Aldous-Broder : création de labyrinthe suivant le cahier des charges suivant :
//On choisit une cellule au hasard comme « cellule courante » et on la marque comme « visitée ».
//Tant qu’il existe des cellules non visitées :
//On choisit un voisin au hasard,
//Si le voisin choisi n’a pas encore été visité :
//On ouvre un passage entre la « cellule courante » et le voisin choisi,
//On marque le voisin choisi comme étant visité,
//On fait du voisin choisi la « cellule courante ».
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
/////////////////////////////////////// FONCTIONS D'AFFICHAGE UTILE POUR TESTER ///////////////////////////////////////
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

	// Marquage des limites
	for (uint32_t i = 0; i < wallPositions.size(); i++) {
	    wallPositions[i][0] = true;                       // Limite en haut
	    wallPositions[i][wallPositions[i].size() - 1] = true;  // Limite en bas
	}

	for (uint32_t j = 0; j < wallPositions[0].size(); j++) {
	    wallPositions[0][j] = true;                          // Limite de gauche 
	    wallPositions[wallPositions.size() - 1][j] = true;   // Limite de droite
	}

	// Marquage des murs
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

	// Affiche les murs dans le terminal
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





    


