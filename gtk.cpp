#include <gtkmm.h>
#include "graph.hpp"
#include <iostream>
#include <string>
#include <utility>

using namespace std;

class MazeWindow : public Gtk::Window {
protected:
    int nbRows;
    int nbColumns;
    std::vector<Cell> mazeCells;
    std::vector<std::pair<int, int>> walls;

    int m_playerX, m_playerY;
    int m_exitX, m_exitY;
    string choice;

    Gtk::DrawingArea m_drawingArea;
    Gtk::Grid m_grid;
    Gtk::Button m_button_fusion;
    Gtk::Button m_button_aldous_broder;
    Gtk::Button m_button_quit;
    Gtk::Entry entry_rows, entry_columns;
    Gtk::Button m_button_valider;

public:
    MazeWindow() {
        set_default_size(400, 400);

        // Création d'une grille pour représenter le labyrinthe
        m_grid.set_row_spacing(5);
        m_grid.set_column_spacing(5);

        // Création des boutons "Fusion" et "Aldous-Broder"
        m_button_fusion.set_label("Fusion");
        m_button_fusion.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::on_button_clicked_fusion));
        m_grid.attach(m_button_fusion, 0, 0, 1, 1);

        m_button_aldous_broder.set_label("Aldous-Broder");
        m_button_aldous_broder.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::on_button_clicked_aldous_broder));
        m_grid.attach(m_button_aldous_broder, 0, 1, 1, 1);

        m_button_quit.set_label("Quit");
        m_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::close));
        m_grid.attach(m_button_quit, 0, 2, 1, 1);

        add(m_grid);
        show_all();
    }

    void on_button_clicked_fusion() {
        m_playerX = 0; // Position initiale du joueur en X
        m_playerY = 0; // Position initiale du joueur en Y
        choice = "fusion"; // Choix de l'utilisateur

        display_choice_rows_columns();
    }

    void on_button_clicked_aldous_broder() {
        m_playerX = 0; // Position initiale du joueur en X
        m_playerY = 0; // Position initiale du joueur en Y
        choice = "aldous"; // Choix de l'utilisateur

        display_choice_rows_columns();
    }

    void close() {
        Gtk::Window::close();
    }

    void remove_all_widgets(Gtk::Grid& grid) {
        std::vector<Gtk::Widget*> children = grid.get_children();

        for (Gtk::Widget* child : children) {
            grid.remove(*child);
        }
    }

    void display_choice_rows_columns() {
        remove_all_widgets(m_grid);

        set_title("Dimensions");
        set_size_request(400, 400);

        entry_rows.set_placeholder_text("Lignes");
        m_grid.attach(entry_rows, 0, 1, 1, 1);

        entry_columns.set_placeholder_text("Colonnes");
        m_grid.attach(entry_columns, 1, 1, 1, 1);

        m_button_valider.set_label("Valider");
        m_button_valider.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::display_maze));
        m_grid.attach(m_button_valider, 0, 2, 2, 1);

        show_all();
    }

    void display_maze() {
        remove_all_widgets(m_grid);
        set_title("Labyrinthe");
        this->nbRows = stoi(entry_rows.get_text());
        this->nbColumns = stoi(entry_columns.get_text());

        m_drawingArea.set_size_request(get_allocated_width(), get_allocated_height());
        m_grid.attach(m_drawingArea, 0, 0, nbColumns, nbRows);

        m_drawingArea.signal_draw().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_draw_grid));
        m_drawingArea.add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK); // Activer les événements clavier
        m_drawingArea.signal_key_press_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_key_press));
        m_drawingArea.signal_key_press_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_key_press), false);

        show_all();
        grab_focus();
    }

    bool on_drawingArea_draw_grid(const Cairo::RefPtr<Cairo::Context>& cr) {
        Gtk::Allocation allocation = m_drawingArea.get_allocation();
        pair<int,int> xy;

        Graph graph(this->nbColumns, this->nbRows);
        if (choice == "fusion") {
            graph.fusion();
            this->walls = graph.getWalls();
        }
        else {
            graph.aldousBroder();
            this->walls = graph.getWalls();
        }

        double cellWidth = allocation.get_width() / this->nbColumns;
        double cellHeight = allocation.get_height() / this->nbRows;

        cr->set_source_rgb(1.0, 1.0, 1.0);
        cr->set_line_width(1.0);

        for (const auto& wall : this->walls) {
            if (wall.second == wall.first + 1) {
                xy = graph.getXY(wall.first, wall.second, nbRows, nbColumns);
                cr->move_to(xy.first, xy.second);
                cr->line_to(xy.first, xy.second + cellHeight);
            }

            if (wall.second == wall.first + nbColumns) {
                xy = graph.getXY(wall.first, wall.second, nbRows, nbColumns);
                cr->move_to(xy.first, xy.second);
                cr->line_to(xy.first + cellWidth, xy.second);
            }

            cr->stroke();
        }

        // Dessiner le joueur
        double playerX = m_playerX * cellWidth;
        double playerY = m_playerY * cellHeight;
        cr->set_source_rgb(0.0, 1.0, 0.0); // Couleur verte pour le joueur
        cr->rectangle(playerX, playerY, cellWidth, cellHeight);
        cr->fill();

        // Dessiner l'arrivée
        m_exitX = nbColumns-1;
        m_exitY = nbRows-1;
        double exitX = m_exitX * cellWidth;
        double exitY = m_exitY * cellHeight;
        cr->set_source_rgb(1.0, 0.0, 0.0); // Couleur rouge pour l'arrivée
        cr->rectangle(exitX, exitY, cellWidth, cellHeight);
        cr->fill();
        
        return true;
    }
    

    bool on_drawingArea_key_press(GdkEventKey* event) {
        // Récupérer la touche pressée
        cout<<"key pressed"<<endl;

        int keyVal = event->keyval;
        bool moved = false;

        
        // Déplacer le joueur en fonction de la touche pressée
        switch (keyVal) {
            case GDK_KEY_Up:
                if (m_playerY > 0 && !isWall(m_playerX, m_playerY - 1)) {
                    m_playerY -= 1;
                    moved = true;
                }
                break;
            case GDK_KEY_Down:
                if (m_playerY < nbRows - 1 && !isWall(m_playerX, m_playerY + 1)) {
                    m_playerY += 1;
                    moved = true;
                }
                break;
            case GDK_KEY_Left:
                if (m_playerX > 0 && !isWall(m_playerX - 1, m_playerY)) {
                    m_playerX -= 1;
                    moved = true;
                }
                break;
            case GDK_KEY_Right:
                if (m_playerX < nbColumns - 1 && !isWall(m_playerX + 1, m_playerY)) {
                    m_playerX += 1;
                    moved = true;
                }
                break;
        }
        

        // Redessiner le labyrinthe
        if (moved) {
            m_drawingArea.queue_draw();
        }

        // Vérifier si le joueur a atteint l'arrivée
        if (m_playerX == m_exitX && m_playerY == m_exitY) {
            Gtk::MessageDialog dialog(*this, "Victoire !", false, Gtk::MESSAGE_INFO);
            dialog.set_secondary_text("Vous avez atteint l'arrivée.");
            dialog.run();

            display_choice_rows_columns();
        }

        return true;
    }

    bool isWall(int x, int y) {
        for (const auto& wall : this->walls) {
            if ((wall.first == x && wall.second == x + 1) || (wall.second == x && wall.first == x + 1)) {
                if ((wall.first / nbColumns == y && wall.first % nbColumns == x) || (wall.second / nbColumns == y && wall.second % nbColumns == x)) {
                    return true;
                }
            }
            else if ((wall.first == x && wall.second == x + nbColumns) || (wall.second == x && wall.first == x + nbColumns)) {
                if ((wall.first / nbColumns == y && wall.first % nbColumns == x) || (wall.second / nbColumns == y && wall.second % nbColumns == x)) {
                    return true;
                }
            }
        }
        return false;
    }
};










/*
#include <gtkmm.h>
#include "graph.hpp"
#include <iostream>
#include <string>
#include <utility>

using namespace std;

class MazeWindow : public Gtk::Window {
protected:
    int nbRows;
    int nbColumns;
    std::vector<Cell> mazeCells;
    std::vector<std::pair<int, int>> walls;

    int m_playerX, m_playerY;
    string choice;

    Gtk::DrawingArea m_drawingArea;
    Gtk::Grid m_grid;
    Gtk::Button m_button_fusion;
    Gtk::Button m_button_aldous_broder;
    Gtk::Button m_button_quit;
    Gtk::Entry entry_rows, entry_columns;
    Gtk::Button m_button_valider;

public:
    MazeWindow() {
        set_default_size(400, 400);

        // Création d'une grille pour représenter le labyrinthe
        m_grid.set_row_spacing(5);
        m_grid.set_column_spacing(5);

        // Création des boutons "Fusion" et "Aldous-Broder"
        m_button_fusion.set_label("Fusion");
        m_button_fusion.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::on_button_clicked_fusion));
        m_grid.attach(m_button_fusion, 0, 0, 1, 1);

        m_button_aldous_broder.set_label("Aldous-Broder");
        m_button_aldous_broder.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::on_button_clicked_aldous_broder));
        m_grid.attach(m_button_aldous_broder, 0, 1, 1, 1);
        
        m_button_quit.set_label("Quit");
        m_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::close));
        m_grid.attach(m_button_quit, 0, 2, 1, 1);

        add(m_grid);
        show_all();
    }

    void on_button_clicked_fusion() {
        m_playerX = 0; // Position initiale du joueur en X
        m_playerY = 0; // Position initiale du joueur en Y
        choice = "fusion"; // Choix de l'utilisateur
        // Logique pour le bouton "Fusion"
        std::cout << "Fusion button clicked" << std::endl;
        display_choice_rows_columns();
        //display_maze();
        //graph.fusion();
    }

    void on_button_clicked_aldous_broder() {
        m_playerX = 0; // Position initiale du joueur en X
        m_playerY = 0; // Position initiale du joueur en Y
        choice = "aldous"; // Choix de l'utilisateur
        // Logique pour le bouton "Aldous-Broder"
        std::cout << "Aldous-Broder button clicked" << std::endl;
        display_choice_rows_columns();

        // Appeler la fonction correspondante de la classe Graph
        //graph.aldousBroder();
    }
    
    void close() {
        Gtk::Window::close();
    }
    
    void remove_all_widgets(Gtk::Grid& grid) {        
	// Obtenez tous les enfants de la grille
	std::vector<Gtk::Widget*> children = grid.get_children();

	// Parcourez tous les enfants et détruisez-les
	for (Gtk::Widget* child : children) {
	// Supprimez l'enfant de la grille
		grid.remove(*child);

	}        
    }

    // Choix du nombre de lignes et de colonnes pour l'utilisateur
    void display_choice_rows_columns() {
        // Supprimer tous les widgets existants de la grille
	remove_all_widgets(m_grid);
	
	set_title("Dimensions");
	set_size_request(400, 400);



	entry_rows.set_placeholder_text("Lignes");
	m_grid.attach(entry_rows, 0, 1, 1, 1);

	entry_columns.set_placeholder_text("Colonnes");
	m_grid.attach(entry_columns, 1, 1, 1, 1);

	m_button_valider.set_label("Valider");
	m_button_valider.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::display_maze));
	m_grid.attach(m_button_valider, 0, 2, 2, 1);

	show_all();
	
    }    

  
    
    void display_maze() {
        // Supprimer tous les widgets existants de la grille
	    remove_all_widgets(m_grid);
	    set_title("Labyrinthe");
      	    this->nbRows=stoi(entry_rows.get_text());
	    this->nbColumns=stoi(entry_columns.get_text());

	    // Créer une nouvelle DrawingArea et l'attacher à la grille
	    m_drawingArea.set_size_request(get_allocated_width(), get_allocated_height());
	    m_grid.attach(m_drawingArea, 0, 0, nbColumns, nbRows);

	    // Définir les événements pour la DrawingArea

	    m_drawingArea.signal_draw().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_draw_grid));
	    //m_drawingArea.signal_draw().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_draw_walls));

	    // Mettre à jour l'affichage de la fenêtre
	    show_all();
    }



    //Dessine les murs du labyrinthe
    bool on_drawingArea_draw_grid(const Cairo::RefPtr<Cairo::Context>& cr) {
	Gtk::Allocation allocation = m_drawingArea.get_allocation();
	pair<int,int> xy;
	

	Graph graph(this->nbColumns, this->nbRows);
	if(choice=="fusion"){
		graph.fusion();
		this->walls=graph.getWalls();
	}
	else{
		graph.aldousBroder();
		this->walls=graph.getWalls();
	}

	// Calculer la taille des cases du labyrinthe en fonction de la taille allouée à la DrawingArea
	double cellWidth = allocation.get_width() / this->nbColumns;
	double cellHeight = allocation.get_height() / this->nbRows;

	// Dessiner les cases du labyrinthe
	cr->set_source_rgb(1.0, 1.0, 1.0); // Couleur blanche pour les lignes de la grille 
	cr->set_line_width(1.0); // Largeur du trait pour les cases


	// Parcourir tous les murs
	for (const auto& wall : this->walls) {
		// Trait verticaux
		if (wall.second == wall.first + 1) {
		    xy = graph.getXY(wall.first, wall.second, nbRows, nbColumns);
		    cout<<"id1 :"<<wall.first<<" id2 :"<<wall.second<<endl;
		    cout<<"x :"<<xy.first<<" y :"<<xy.second<<endl;
		    cr->move_to(xy.first, xy.second);
		    cr->line_to(xy.first, xy.second + cellHeight);
		}

		// Trait horizontaux
		if (wall.second == wall.first + nbColumns) {
		    xy = graph.getXY(wall.first, wall.second, nbRows, nbColumns);
		    cout<<"id1 :"<<wall.first<<" id2 :"<<wall.second<<endl;
		    cout<<"x :"<<xy.first<<" y :"<<xy.second<<endl;
		    cr->move_to(xy.first, xy.second);
		    cr->line_to(xy.first + cellWidth, xy.second);
		}

	cr->stroke();
	}
	cout<<"fin affichage grille"<<endl;


	return true;
    }



};

*/

