#include <gtkmm.h>
#include "graph.hpp"
#include <iostream>
#include <string>

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

        // Création d'une instance de la classe Graph pour générer le labyrinthe
        Graph graph(10, 10); // Remplacez les dimensions du labyrinthe selon vos besoins

        // Récupération des informations du labyrinthe
        this->nbRows = graph.getNbRows();
        this->nbColumns = graph.getNbColumns();
        this->mazeCells = graph.getCells();
        this->walls = graph.getWalls();

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

	    nbRows=stoi(entry_rows.get_text());
	    nbColumns=stoi(entry_columns.get_text());
	    // Créer une nouvelle DrawingArea et l'attacher à la grille
	    m_drawingArea.set_size_request(get_allocated_width(), get_allocated_height());
	    m_grid.attach(m_drawingArea, 0, 0, nbColumns, nbRows);

	    // Définir les événements pour la DrawingArea

	    m_drawingArea.signal_draw().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_draw));

	    // Mettre à jour l'affichage de la fenêtre
	    show_all();
    }


    //Dessine un grille 
    bool on_drawingArea_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	Gtk::Allocation allocation = m_drawingArea.get_allocation();

	// Calculer la taille des cases du labyrinthe en fonction de la taille allouée à la DrawingArea
	double cellWidth = allocation.get_width() / nbColumns;
	double cellHeight = allocation.get_height() / nbRows;

	// Dessiner les cases du labyrinthe
	cr->set_source_rgb(1.0, 1.0, 1.0); // Couleur blanche pour les cases
	cr->set_line_width(1.0); // Largeur du trait pour les cases


	// Dessiner les lignes horizontales
	for (int row = 0; row <= nbRows; ++row) {
		double y = row * cellHeight;
		cr->move_to(0, y);
		cr->line_to(m_drawingArea.get_allocated_width(), y);
		cr->stroke();
	}
	

	// Dessiner les lignes verticales
	for (int col = 0; col <= nbColumns; ++col) {
		double x = col * cellWidth;
		cr->move_to(x, 0);
		cr->line_to(x, m_drawingArea.get_allocated_height());
		cr->stroke();
	}
	
	// Dessiner le caractère "*" à la position (0, 0) de la grille
	cr->set_source_rgb(100.0, 100.0, 100.0); // Couleur blanche pour les cases

	
	cr->set_font_size(12.0);
	cr->move_to(19, 19);
	cr->show_text("@");
	cr->stroke();


	return true;
    }



};



