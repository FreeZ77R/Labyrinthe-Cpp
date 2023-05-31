#include <gtkmm.h>
#include "graph.hpp"
#include <iostream>

class MazeWindow : public Gtk::Window {
protected:
    int nbRows;
    int nbColumns;
    std::vector<Cell> mazeCells;
    std::vector<std::pair<int, int>> walls;
    
    int m_playerX, m_playerY;

    Gtk::DrawingArea m_drawingArea;
    Gtk::Grid m_grid;
    Gtk::Button m_button_fusion;
    Gtk::Button m_button_aldous_broder;
    Gtk::Button m_button_quit;

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
        // Logique pour le bouton "Fusion"
        std::cout << "Fusion button clicked" << std::endl;
        display_maze();
        // graph.fusion();
    }

    void on_button_clicked_aldous_broder() {
        m_playerX = 0; // Position initiale du joueur en X
        m_playerY = 0; // Position initiale du joueur en Y
        // Logique pour le bouton "Aldous-Broder"
        std::cout << "Aldous-Broder button clicked" << std::endl;
        display_cells();
        // Appeler la fonction correspondante de la classe Graph
        // graph.aldousBroder();
    }
    
    void close() {
        Gtk::Window::close();
    }
    
    void remove_all_widgets(Gtk::Grid& grid) {
        Gtk::Widget* child = nullptr;
        int row = 0, column = 0;
        while ((child = grid.get_child_at(column, row))) {
            grid.remove(*child);
            column++;
            if (column >= grid.get_column_homogeneous()) {
                column = 0;
                row++;
            }
        }
    }


    
    void display_cells() {
        // Supprimer tous les widgets existants de la grille
	remove_all_widgets(m_grid);

        // Créer de nouveaux boutons et les attacher à la grille
        for (int row = 0; row < nbRows; ++row) {
            for (int col = 0; col < nbColumns; ++col) {
                Gtk::Button* button = Gtk::manage(new Gtk::Button());
                button->set_label("Cell " + std::to_string(row) + "-" + std::to_string(col));
                m_grid.attach(*button, col, row, 1, 1);
            }
        }

        // Mettre à jour l'affichage de la fenêtre
        show_all();
    }
    
    void display_maze() {
        // Supprimer tous les widgets existants de la grille
	    remove_all_widgets(m_grid);


	    // Créer une nouvelle DrawingArea et l'attacher à la grille
	    m_drawingArea.set_size_request(get_allocated_width(), get_allocated_height());
	    m_grid.attach(m_drawingArea, 0, 0, nbColumns, nbRows);

	    // Définir les événements pour la DrawingArea
	    m_drawingArea.add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK);
	    m_drawingArea.signal_draw().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_draw));
	    m_drawingArea.signal_size_allocate().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_size_allocate));
	    m_drawingArea.signal_button_press_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_button_press_event));
	    m_drawingArea.signal_button_release_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_button_release_event));
	    m_drawingArea.signal_motion_notify_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_motion_notify_event));

	    // Mettre à jour l'affichage de la fenêtre
	    show_all();
    }

    bool on_drawingArea_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	Gtk::Allocation allocation = m_drawingArea.get_allocation();

	// Calculer la taille des cases du labyrinthe en fonction de la taille allouée à la DrawingArea
	double cellWidth = allocation.get_width() / nbColumns;
	double cellHeight = allocation.get_height() / nbRows;

	// Dessiner les cases du labyrinthe
	cr->set_source_rgb(1.0, 1.0, 1.0); // Couleur blanche pour les cases
	cr->set_line_width(1.0); // Largeur du trait pour les cases

	for (int row = 0; row < nbRows; ++row) {
		for (int col = 0; col < nbColumns; ++col) {
		    double x = col * cellWidth;
		    double y = row * cellHeight;

		    cr->rectangle(x, y, cellWidth, cellHeight);
		    cr->fill();
		    cr->stroke();
		}
	}

	// Dessiner les lignes de la grille
	cr->set_source_rgb(0.0, 0.0, 0.0); // Couleur noire pour les lignes
	cr->set_line_width(0.5); // Largeur du trait pour les lignes

	// Dessiner les lignes horizontales
	for (int row = 0; row < nbRows; ++row) {
		double y = row * cellHeight;
		cr->move_to(0, y);
		cr->line_to(m_drawingArea.get_allocated_width(), y);
		cr->stroke();
	}
	

	// Dessiner les lignes verticales
	for (int col = 0; col < nbColumns; ++col) {
		double x = col * cellWidth;
		cr->move_to(x, 0);
		cr->line_to(x, m_drawingArea.get_allocated_height());
		cr->stroke();
	}

	return true;
    }



    void on_drawingArea_size_allocate(Gtk::Allocation& allocation) {
        // Mettre à jour la taille des cases du labyrinthe en fonction de la taille allouée à la DrawingArea
        double cellWidth = allocation.get_width() / nbColumns;
        double cellHeight = allocation.get_height() / nbRows;

        // Mettre à jour la taille des cases pour la fonction on_drawingArea_draw

        // Mettre à jour l'affichage de la fenêtre
        m_drawingArea.queue_draw();
    }



    bool on_drawingArea_button_press_event(GdkEventButton* event) {
        // Logique pour gérer l'événement de clic de souris

        // Par exemple, mettez à jour la position du personnage en fonction du clic de souris
        m_playerX = event->x;
        m_playerY = event->y;

        m_drawingArea.queue_draw(); // Demande un redessin de la zone de dessin

        return true;
    }

    bool on_drawingArea_button_release_event(GdkEventButton* event) {
        // Logique pour gérer l'événement de relâchement de souris

        return true;
    }

    bool on_drawingArea_motion_notify_event(GdkEventMotion* event) {
        // Logique pour gérer l'événement de mouvement de souris

        return true;
    }
};






