
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
    vector<vector<vector<bool>>> walls;

public:
    Graph(int rows, int columns) : nbColumns(columns), nbRows(rows), nbCells(rows * columns) {
        mazeCells.resize(nbCells);
        neighborsList.resize(nbCells);

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




/*
    void generateMaze() {
        srand(time(nullptr));

        int startCellId = 0;
        int endCellId = nbCells - 1;

        //changeMultipleId(startCellId, -1);
        //changeMultipleId(endCellId, -2);

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
        for (int i = 0; i < nbColumns; i++) {
            for (int j = 0; j < nbRows; j++) {
                cout << "walls[" << i << "][" << j << "] = " << walls[i][j] << endl;
            }
        }

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
*/

};


int main() {
    Graph graph(5, 5); // Exemple : Labyrinthe de taille 10x10
    //graph.generateMaze();
    //graph.printMaze();
    return 0;
}
