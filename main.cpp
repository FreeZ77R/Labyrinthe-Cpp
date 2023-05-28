#include <ctime>
#include <cstdlib>
#include <iostream>
#include "graph.hpp"

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