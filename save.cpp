
// a finir
    int search_neighbors(int id, vector<vector<int>> &neighborsList, vector<Cell> &mazeCells){
        
        int rd=rand()%neighborsList[id].size();
        int id_neighbor=neighborsList[id][rd];

        while(mazeCells[id_neighbor].visited==true){

        }
        mazeCells[id_neighbor].visited=true;

        return id_neighbor;
    }

// a finir 

   void fusion_test() {
        
        int all_visited=0;

        int id_;
        vector<int> id_v;

        while(all_visited<nbCells){
            id_=rand()%nbCells;
            id_v=voisins(mazeCells[id_]); 

            for(int i=0; i<id_v.size(); i++){
                if(mazeCells[id_].id!=mazeCells[id_v[i]].id && mazeCells[id_v[i]].visited==0){  
                    IsVisited(mazeCells,id_);
                    //IsVisited(mazeCells,id_v);
                    all_visited++;
                    cout<<"all_visited"<<all_visited<<endl;
                    changeMultipleId(mazeCells, id_, id_v[i]);
                    chemin.push_back(make_pair(id_, id_v[i]));

                    cout<<"\n\nid_ :"<<id_<<"\tid_v :"<<id_v[i]<<endl;
                }
            }

            
            for(int i=0; i<nbCells; i++){

                if(i%nbRows==0){cout<<endl;}

                cout<<mazeCells[i].id<<"\t";        
        
            }  
            cout<<"\n\n"<<endl; 

            printVisited();

        }

        cout<<"\n\n"<<endl;
        for(int i=0; i<chemin.size(); i++){

            cout<<"("<<chemin[i].first<<","<<chemin[i].second<<")\t";       
        
        }  
        cout<<"\n\n"<<endl;         

    }



// ancienne fonction fusion 

    void fusion() {
        
        int all_visited=0;
        vector<pair<int,int>> chemin; //retiens le chemin à partir de l'id des cellules traité

        int id_;
        vector<int> id_v;

        while(all_visited<nbCells){
            id_=rand()%nbCells;
            id_v=voisins(mazeCells[id_]); 

            for(int i=0; i<id_v.size(); i++){
                if(mazeCells[id_].id!=mazeCells[id_v[i]].id && mazeCells[id_v[i]].visited==0){  
                    IsVisited(mazeCells,id_);
                    //IsVisited(mazeCells,id_v);
                    all_visited++;
                    cout<<"all_visited"<<all_visited<<endl;
                    changeMultipleId(mazeCells, id_, id_v[i]);
                    chemin.push_back(make_pair(id_, id_v[i]));

                    cout<<"\n\nid_ :"<<id_<<"\tid_v :"<<id_v[i]<<endl;
                }
            }

            
            for(int i=0; i<nbCells; i++){

                if(i%nbRows==0){cout<<endl;}

                cout<<mazeCells[i].id<<"\t";        
        
            }  
            cout<<"\n\n"<<endl; 

            printVisited();

        }

        cout<<"\n\n"<<endl;
        for(int i=0; i<chemin.size(); i++){

            cout<<"("<<chemin[i].first<<","<<chemin[i].second<<")\t";       
        
        }  
        cout<<"\n\n"<<endl;         

    }

// deux fonctions voisins 



    //La fonction voisin renvoit une liste de voisins de la cellule actuelle qui n'a pas été visité
    vector<int> voisins(Cell cell){

        int saveY = cell.y_coord, saveX = cell.x_coord;
        vector<int> voisins;

        // Coordonnées des déplacements possibles (haut, bas, gauche, droite)
        int deplacementsX[] = { -1, 1, 0, 0 };
        int deplacementsY[] = { 0, 0, -1, 1 };  

        for (int i = 0; i < 4; i++) {
            cell.y_coord = saveY;
            cell.x_coord = saveX;
            cell.x_coord = cell.x_coord + deplacementsX[i];
            cell.y_coord = cell.y_coord + deplacementsY[i];
            if(estValide(cell.y_coord, cell.x_coord)){
                if(mazeCells[getId(cell.y_coord, cell.x_coord)].visited==0){
                    voisins.push_back(getId(cell.y_coord, cell.x_coord));
                }   
            }
        }
        return voisins;
    }


    //La fonction voisin renvoit un voisin de la cellule actuelle qui n'a pas été visité
    int voisinFusion(Cell cell){
        int saveY = cell.y_coord, saveX = cell.x_coord;


        // Coordonnées des déplacements possibles (haut, bas, gauche, droite)
        int deplacementsY[] = { 0, 0, -1, 1 }; 
        int deplacementsX[] = { -1, 1, 0, 0 };

        for (int i=0; i<4; i++){
            cell.y_coord = saveY;
            cell.x_coord = saveX;
            cell.y_coord = cell.y_coord + deplacementsY[i];
            cell.x_coord = cell.x_coord + deplacementsX[i];
            if(estValide(cell.y_coord, cell.x_coord) && mazeCells[getId(cell.y_coord, cell.x_coord)].visited==0){
                if(getId(cell.y_coord, cell.x_coord)!=getId(saveY, saveX)){
                    return getId(cell.y_coord, cell.x_coord);
                }
            }
        }
        cout<<"pas de voisin valide"<<endl;
        
    }












// affichage fusion correct mais nombre de mur manquants

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Cell {
    int x_coord;
    int y_coord;
    int id;
    bool visited;
};

class Graph {
private:
    int nbColumns;
    int nbRows;
    int nbCells;
    vector<Cell> mazeCells;
    vector<vector<int>> neighborsList;
    vector<vector<bool>> walls;

public:
    Graph(int rows, int columns) : nbColumns(columns), nbRows(rows), nbCells(rows * columns) {
        neighborsList.resize(nbCells);
        walls.resize(nbColumns, vector<bool>(nbRows, true));

        for (int i = 0; i < nbColumns; i++) {
            for (int j = 0; j < nbRows; j++) {
                Cell cell;
                cell.x_coord = j;
                cell.y_coord = i;
                cell.id = i * nbRows + j;
                cell.visited = false;
                mazeCells.push_back(cell);
            }
        }

        init_neighborsList();
        generateMaze();
        printMaze();
    }

    int getId(int y, int x) {
        return y * nbRows + x;
    }

    void changeMultipleId(int id_, int id_v) {
        for (int i = 0; i < mazeCells.size(); i++) {
            if (mazeCells[i].id == id_v) {
                mazeCells[i].id = id_;
            }
        }
    }

    void IsVisited(int id) {
        mazeCells[id].visited = true;
    }

    bool isValid(int y, int x) {
        return (y >= 0 && y < nbColumns && x >= 0 && x < nbRows);
    }

    void init_neighborsList() {
        int deplacementsX[] = { -1, 1, 0, 0 };
        int deplacementsY[] = { 0, 0, -1, 1 };
        int saveY, saveX, Y, X;

        for (int i = 0; i < nbCells; i++) {
            saveY = mazeCells[i].y_coord;
            saveX = mazeCells[i].x_coord;

            for (int d = 0; d < 4; d++) {
                Y = saveY + deplacementsY[d];
                X = saveX + deplacementsX[d];
                if (isValid(Y, X)) {
                    neighborsList[i].push_back(getId(Y, X));
                }
            }
        }
    }

    void generateMaze() {
        srand(time(nullptr));

        int startCellId = 0;
        int endCellId = nbCells - 1;

        changeMultipleId(startCellId, -1);
        changeMultipleId(endCellId, -2);

        vector<int> stack;
        stack.push_back(startCellId);

        while (!stack.empty()) {
            int currentCellId = stack.back();
            stack.pop_back();
            IsVisited(currentCellId);

            vector<int> unvisitedNeighbors;

            for (int neighborId : neighborsList[currentCellId]) {
                if (!mazeCells[neighborId].visited) {
                    unvisitedNeighbors.push_back(neighborId);
                }
            }

            if (!unvisitedNeighbors.empty()) {
                int randomNeighborId = unvisitedNeighbors[rand() % unvisitedNeighbors.size()];
                stack.push_back(currentCellId);
                stack.push_back(randomNeighborId);

                int wallX = (mazeCells[currentCellId].x_coord + mazeCells[randomNeighborId].x_coord) / 2;
                int wallY = (mazeCells[currentCellId].y_coord + mazeCells[randomNeighborId].y_coord) / 2;

                if (mazeCells[currentCellId].id != mazeCells[randomNeighborId].id) {
                    walls[wallY][wallX] = false;
                    changeMultipleId(mazeCells[currentCellId].id, mazeCells[randomNeighborId].id);
                }
            }
        }
    }

    void printMaze() {
        // Affichage des murs supérieurs du labyrinthe
        for (int i = 0; i < nbRows; i++) {
            cout << "+---";
        }
        cout << "+" << endl;

        for (int i = 0; i < nbColumns; i++) {
            // Affichage des murs latéraux et des espaces entre les cellules
            for (int j = 0; j < nbRows; j++) {
                // Vérifier si le mur de gauche doit être affiché
                if (j == 0 || walls[i][j - 1]) {
                    cout << "|   ";
                } else {
                    cout << "    ";
                }
            }

            cout << "|" << endl;

            // Affichage des murs inférieurs du labyrinthe
            for (int j = 0; j < nbRows; j++) {
                cout << "+";
                if (i == nbColumns - 1 || walls[i][j]) {
                    cout << "---";
                } else {
                    cout << "   ";
                }
            }
            cout << "+" << endl;
        }
    }
};

int main() {
    Graph graph(5, 5); // Exemple : Labyrinthe de taille 10x10
    return 0;
}



// les deux methodes fonctionne A PEU PRET 
///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////


#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <algorithm>

using namespace std;

//Le type Cell représente une cellule du labyrinthe
struct Cell {
    int x_coord;
    int y_coord;
    int id;
    bool visited;
}cell;


class Graph {
private:
    int nbColumns; //y_max
    int nbRows;    //x_max
    int nbCells=nbColumns*nbRows;

    vector<Cell> mazeCells;

    vector<vector<int>> neighborsList; 

    vector<pair<int,int>> walls; //exemple : walls[i][j] = true; Mur entre deux id differents

public:

    //Constructeur qui initialise les cellules du labyrinthe (y,x)
    Graph(int rows, int columns) : nbColumns(columns), nbRows(rows) {
        neighborsList.resize(nbCells);
        int id_=0;
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
        init_neighborsList(mazeCells); 
        init_walls(neighborsList);

    }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////  METHODES UTILITAIRE ALGO FUSION   ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int getId(int y,int x){
        for(int i=0; i<nbCells; i++){
            if(mazeCells[i].y_coord==y && mazeCells[i].x_coord==x){return mazeCells[i].id;}
        }
        cout<<"Cellule inexistante"<<endl;
    }

    void changeMultipleId(const int id, const int id_neighbor){
        for(int i=0; i<this->mazeCells.size(); i++){
            if(this->mazeCells[i].id==id_neighbor){
                this->mazeCells[i].id=id;
            }
        }
        mazeCells[id_neighbor].id=id;
    }

    void IsVisited(vector<Cell> &mazeCells, const int id){
        mazeCells[id].visited=true;
    }

    bool estValide(int y, int x){
        return (y>=0 && y<nbColumns && x>=0 && x<nbRows);
    }


    void init_neighborsList(const vector<Cell> &mazeCells){

        // Coordonnées des déplacements possibles (haut, bas, gauche, droite)
        int deplacementsX[] = { -1, 1, 0, 0 };
        int deplacementsY[] = { 0, 0, -1, 1 };  
        int saveY,saveX,Y,X;

        for(int i=0; i<nbCells; i++){ 

            saveY=mazeCells[i].y_coord;
            saveX=mazeCells[i].x_coord;

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

    void init_walls(const vector<vector<int>> &neighborsList){
        for(int i=0; i<nbCells; i++){
            for(int j=0; j<neighborsList[i].size(); j++){
                this->walls.push_back(make_pair(i, neighborsList[i][j]));               
            }
        }
    }

    // renvoie un voisin aleatoire de la cellule courante
    int RandNeighbor(int id){

        int rd = rand()%neighborsList[id].size();
        int id_v = neighborsList[id][rd];
        return id_v;
    }

    bool alreadyVisited(int id, vector<int> &neighborsVisited){
        for(int i=0; i<neighborsVisited.size(); i++){
            if(neighborsVisited[i]==id){
                return false;
            }
        }
        return true;
    }

    void removeWall(int id, int id_neighbor){


        walls.erase(
            remove_if(
                walls.begin(), walls.end(),
                [id, id_neighbor](const pair<int, int>& wall) {
                    return (wall.first == id && wall.second == id_neighbor) || (wall.first == id_neighbor && wall.second == id);
                }
            ),
            walls.end()
        );
    

        /*
        for(int i=0; i<walls.size(); i++){
            if((this->walls[i].first==id && this->walls[i].second==id_neighbor)||(this->walls[i].first==id_neighbor && this->walls[i].second==id)){
                walls.erase(walls.begin()+i);
            }
        }*/
    }

    void fusion() {
        vector<int> neighborsVisited;
        int id, id_neighbor;

        while (neighborsVisited.size() < nbCells) {
            id = rand() % nbCells;

            if (alreadyVisited(id, neighborsVisited)) {
                id_neighbor = RandNeighbor(id);

                if (mazeCells[id].id != mazeCells[id_neighbor].id) {
                    removeWall(id, id_neighbor);
                    changeMultipleId(mazeCells[id].id, mazeCells[id_neighbor].id);
                }

                neighborsVisited.push_back(id);
            }
        }
    }



    void aldousBroder() {
        int nbVisited = 1;
        int id = rand() % nbCells;
        mazeCells[id].visited = true;

        while (nbVisited < nbCells) {
            int id_neighbor = RandNeighbor(id);

            if (!mazeCells[id_neighbor].visited) {
                removeWall(id, id_neighbor);
                mazeCells[id_neighbor].visited = true;
                nbVisited++;
            }

            id = id_neighbor;
        }

    }

//};



 



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Affichage des cellules du labyrinthe 
    void printCells() {
        for(int i=0; i<nbCells; i++){

            if(i%nbRows==0){cout<<endl;}

            cout<<mazeCells[i].y_coord<<","<<mazeCells[i].x_coord<<"\t";        
        
        }
        cout<<endl;
    }


    // Affichage des voisins de chaque cellules 
    void printNeighborsList() {
        for(int i=0; i<nbCells; i++){
            for(int x : neighborsList[i]){
                cout<<x<<",";
 
            } 
            cout<<"\t";       
        
        }
        cout<<"\n"<<endl;
    }

    // Affichage entre qu'elles cellules il y a des murs 
    void printWallsId() {
        for(int i=0; i<walls.size(); i++){
            cout<<"walls "<<i<<" : "<<walls[i].first<<","<<walls[i].second<<"\t";
        }
        cout<<"\n"<<endl;
    }






    void printWalls() {
        vector<vector<bool>> wallPositions(this->nbColumns * 2 + 1, vector<bool>(this->nbRows * 2 + 1, false));

        // Parcours de la variable walls pour stocker les positions des murs
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

        // Affichage des murs dans le terminal
        for (int i = 0; i < wallPositions.size(); i++) {
            for (int j = 0; j < wallPositions[i].size(); j++) {
                if (wallPositions[i][j]) {
                    cout << "+";
                } else {
                    cout << " ";
                }
            }
            cout << endl;
        }
    }



    void printVisited() {
        for(int i=0; i<nbCells; i++){

            if(i%nbRows==0){cout<<endl;}

            cout<<mazeCells[i].visited<<"\t";       
        
        }
        cout<<endl;
    }

    void printId(){

        for(int j=0; j<nbCells; j++){
            if(j%nbColumns==0){
                cout<<endl;
            }

            cout<<mazeCells[j].id<<"\t";
        
        } 
        cout<<endl;
    }

};






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
    graph.fusion();
    //graph.aldousBroder();
    graph.printWallsId();
    //graph.printWalls();
    //graph.printId();




    return EXIT_SUCCESS;
}


    








// les deux methodes fonctionne A PEU PRET 
///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////



#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <algorithm>

using namespace std;

//Le type Cell représente une cellule du labyrinthe
struct Cell {
    int x_coord;
    int y_coord;
    int id;
    bool visited;
}cell;


class Graph {
private:
    int nbColumns; 
    int nbRows;    
    int nbCells=nbColumns*nbRows;

    vector<Cell> mazeCells;

    vector<vector<int>> neighborsList; 

    vector<pair<int,int>> walls; 

public:

    //Constructeur qui initialise les cellules du labyrinthe (y,x)
    Graph(int rows, int columns) : nbColumns(columns), nbRows(rows) {
        neighborsList.resize(nbCells);
        int id_=0;
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
        init_neighborsList(mazeCells); 
        init_walls(neighborsList);

    }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////  METHODES UTILITAIRE ALGO FUSION   ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int getId(int y,int x){
        for(int i=0; i<nbCells; i++){
            if(mazeCells[i].y_coord==y && mazeCells[i].x_coord==x){return mazeCells[i].id;}
        }
        cout<<"Cellule inexistante"<<endl;
    }

    void changeMultipleId(const int id, const int id_neighbor){
        for(int i=0; i<this->mazeCells.size(); i++){
            if(this->mazeCells[i].id==id_neighbor){
                this->mazeCells[i].id=id;
            }
        }
    }

    void IsVisited(vector<Cell> &mazeCells, const int id){
        mazeCells[id].visited=true;
    }

    bool estValide(int y, int x){
        return (y>=0 && y<nbColumns && x>=0 && x<nbRows);
    }


    void init_neighborsList(const vector<Cell> &mazeCells){

        // Coordonnées des déplacements possibles (haut, bas, gauche, droite)
        int deplacementsX[] = { -1, 1, 0, 0 };
        int deplacementsY[] = { 0, 0, -1, 1 };  
        int saveY,saveX,Y,X;

        for(int i=0; i<nbCells; i++){ 

            saveY=mazeCells[i].y_coord;
            saveX=mazeCells[i].x_coord;

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

    void init_walls(const vector<vector<int>> &neighborsList){
        for(int i=0; i<nbCells; i++){
            for(int j=0; j<neighborsList[i].size(); j++){
                this->walls.push_back(make_pair(i, neighborsList[i][j]));               
            }
        }
    }

    // renvoie un voisin aleatoire de la cellule courante
    int RandNeighbor(int id){

        int rd = rand()%neighborsList[id].size();
        int id_v = neighborsList[id][rd];

        while(mazeCells[id].id == mazeCells[id_v].id){
            rd = rand()%neighborsList[id].size();
            id_v = neighborsList[id][rd];
        }

        return id_v;
    }


    bool alreadyVisited(int id, vector<int> &neighborsVisited){
        for(int i=0; i<neighborsVisited.size(); i++){
            if(neighborsVisited[i]==id){
                return true;
            }
        }
        return false;
    }

    void removeWall(int id, int id_neighbor){


        walls.erase(
            remove_if(
                walls.begin(), walls.end(),
                [id, id_neighbor](const pair<int, int>& wall) {
                    return (wall.first == id && wall.second == id_neighbor) || (wall.first == id_neighbor && wall.second == id);
                }
            ),
            walls.end()
        );
    }

    //fonction qui verifie si les identifiants de chaque cellules sont identiques
    bool allSameId() {

        int firstId = mazeCells[0].id;
        for (int i = 1; i < mazeCells.size(); i++) {
            if (mazeCells[i].id != firstId) {
                return false;  // Au moins un élément est différent, donc ils ne sont pas tous identiques
            }
        }

        return true;  // Tous les éléments sont identiques
    }

    int getUnvisitedID(const vector<int>& neighborsVisited) {
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

    void fusion() {

        int nbVisited=0;
        vector<int> neighborsVisited;
        int id, id_neighbor;

        while (!allSameId()) {
            id = getUnvisitedID(neighborsVisited);

            if (!alreadyVisited(id, neighborsVisited)) {

                /// faire des verifications supplémentaire ici 
                id_neighbor = RandNeighbor(id);

                /*if(!allSameId){
                    break;
                }*/

                if (mazeCells[id].id != mazeCells[id_neighbor].id) {
                    removeWall(id, id_neighbor);
                    changeMultipleId(mazeCells[id].id, mazeCells[id_neighbor].id);
                }


                nbVisited++;
                mazeCells[id].visited=true;
                neighborsVisited.push_back(id);
                cout<<"id : "  <<id <<"\t id voisin : "<<id_neighbor;
            }
            printId();
        }


        cout<<endl;
        for(int i=0; i<neighborsVisited.size(); i++){
            cout<<neighborsVisited[i]<<", ";
        }

    }



    void aldousBroder() {
        int nbVisited = 1;
        int id = rand() % nbCells;
        mazeCells[id].visited = true;

        while (nbVisited < nbCells) {
            int id_neighbor = RandNeighbor(id);

            if (!mazeCells[id_neighbor].visited) {
                removeWall(id, id_neighbor);
                mazeCells[id_neighbor].visited = true;
                nbVisited++;
            }

            id = id_neighbor;
        }

    }

//};



 



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Affichage des cellules du labyrinthe 
    void printCells() {
        for(int i=0; i<nbCells; i++){

            if(i%nbRows==0){cout<<endl;}

            cout<<mazeCells[i].y_coord<<","<<mazeCells[i].x_coord<<"\t";        
        
        }
        cout<<endl;
    }


    // Affichage des voisins de chaque cellules 
    void printNeighborsList() {
        for(int i=0; i<nbCells; i++){
            for(int x : neighborsList[i]){
                cout<<x<<",";
 
            } 
            cout<<"\t";       
        
        }
        cout<<"\n"<<endl;
    }

    // Affichage entre qu'elles cellules il y a des murs 
    void printWallsId() {
        for(int i=0; i<walls.size(); i++){
            cout<<"walls "<<i<<" : "<<walls[i].first<<","<<walls[i].second<<"\t";
        }
        cout<<"\n"<<endl;
    }






    void printWalls() {
        vector<vector<bool>> wallPositions(this->nbColumns * 2 + 1, vector<bool>(this->nbRows * 2 + 1, false));

        // Mark boundary walls
        for (int i = 0; i < wallPositions.size(); i++) {
            wallPositions[i][0] = true;                       // Top boundary
            wallPositions[i][wallPositions[i].size() - 1] = true;  // Bottom boundary
        }

        for (int j = 0; j < wallPositions[0].size(); j++) {
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
        for (int i = 0; i < wallPositions.size(); i++) {
            for (int j = 0; j < wallPositions[i].size(); j++) {
                if (wallPositions[i][j]) {
                    cout << "+";
                } else {
                    cout << " ";
                }
            }
            cout << endl;
        }
    }




    void printVisited() {
        for(int i=0; i<nbCells; i++){

            if(i%nbRows==0){cout<<endl;}

            cout<<mazeCells[i].visited<<"\t";       
        
        }
        cout<<endl;
    }

    void printId(){

        for(int j=0; j<nbCells; j++){
            if(j%nbColumns==0){
                cout<<endl;
            }

            cout<<mazeCells[j].id<<"\t";
        
        } 
        cout<<endl;
    }

};






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
    graph.fusion();
    //graph.printId();
    //graph.aldousBroder();
    graph.printWallsId();
    graph.printWalls();
    //graph.printId();
    graph.printVisited();




    return EXIT_SUCCESS;
}


    





