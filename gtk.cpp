#include <gtkmm.h>
#include "graph.hpp"

class MazeWindow : public Gtk::Window {
public:
  MazeWindow() {
    set_default_size(400, 400);

    // Création d'une instance de la classe Graph pour générer le labyrinthe
    Graph graph(10, 10); // Remplacez les dimensions du labyrinthe selon vos besoins

    // Récupération des informations du labyrinthe
    int nbRows = graph.getNbRows();
    int nbColumns = graph.getNbColumns();
    std::vector<Cell> mazeCells = graph.getMazeCells();
    std::vector<std::pair<int, int>> walls = graph.getWalls();

    // Création d'une grille pour représenter le labyrinthe
    Gtk::Grid grid;
    grid.set_row_homogeneous(true);
    grid.set_column_homogeneous(true);

    // Ajout des murs du labyrinthe à la grille
    for (const auto& wall : walls) {
      int row = wall.first / nbColumns;
      int col = wall.first % nbColumns;
      int neighborRow = wall.second / nbColumns;
      int neighborCol = wall.second % nbColumns;

      // Dessiner le mur (par exemple, en utilisant des boutons désactivés)
      Gtk::Button* wallButton = Gtk::manage(new Gtk::Button);
      wallButton->set_sensitive(false);
      grid.attach(*wallButton, col, row, 1, 1);

      // Dessiner les murs adjacents
      if (col < neighborCol) {
        Gtk::Button* neighborButton = Gtk::manage(new Gtk::Button);
        neighborButton->set_sensitive(false);
        grid.attach(*neighborButton, col + 1, row, 1, 1);
      }
      else if (col > neighborCol) {
        Gtk::Button* neighborButton = Gtk::manage(new Gtk::Button);
        neighborButton->set_sensitive(false);
        grid.attach(*neighborButton, col - 1, row, 1, 1);
      }
      else if (row < neighborRow) {
        Gtk::Button* neighborButton = Gtk::manage(new Gtk::Button);
        neighborButton->set_sensitive(false);
        grid.attach(*neighborButton, col, row + 1, 1, 1);
      }
      else if (row > neighborRow) {
        Gtk::Button* neighborButton = Gtk::manage(new Gtk::Button);
        neighborButton->set_sensitive(false);
        grid.attach(*neighborButton, col, row - 1, 1, 1);
      }
    }

    add(grid);
    show_all();
  }
};

int main(int argc, char** argv) {
  auto app = Gtk::Application::create(argc, argv);
  MazeWindow mazeWindow;
  return app->run(mazeWindow);
}

