

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

    Gtk::Label m_label;
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

	m_playerX = 0; // Position initiale du joueur en X
        m_playerY = 0; // Position initiale du joueur en Y
	
        // Création d'une grille pour représenter le labyrinthe
        m_grid.set_row_spacing(5);
        m_grid.set_column_spacing(5);

	set_title("Menu");

	entry_rows.set_placeholder_text("Lignes");
        m_grid.attach(entry_rows, 0, 1, 1, 1);

        entry_columns.set_placeholder_text("Colonnes");
        m_grid.attach(entry_columns, 1, 1, 1, 1);

        // Création des boutons "Fusion" et "Aldous-Broder"
        m_button_fusion.set_label("Fusion");
        m_button_fusion.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::on_button_clicked_fusion));
        m_grid.attach(m_button_fusion, 0, 2, 2, 1);

        m_button_aldous_broder.set_label("Aldous-Broder");
        m_button_aldous_broder.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::on_button_clicked_aldous_broder));
        m_grid.attach(m_button_aldous_broder, 0, 3, 2, 1);

        m_button_quit.set_label("Quit");
        m_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::close));
        m_grid.attach(m_button_quit, 0, 4, 2, 1);
        
        m_label.set_markup("<span size='large'>\nComment jouer ? \n\n - Allé vers le haut : 'z' \n - Allé vers le bas : 's' \n - Allé vers la gauche : 'q' \n - Allé vers la droite : 'd' <span foreground='cyan'>\n\n\t\t    z\n\n\t    q\t    s\t    d</span></span>");
	m_label.set_hexpand(true);
	m_grid.attach(m_label, 0, 5, 2, 1);
        
        
	signal_key_press_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_key_press));
	

        add(m_grid);
        show_all();
    }

   

    void on_button_clicked_fusion() {

        choice = "fusion"; // Choix de l'utilisateur
	
	this->nbRows = stoi(entry_rows.get_text());
        this->nbColumns = stoi(entry_columns.get_text());
	Graph graph(this->nbColumns, this->nbRows);
	graph.fusion();
        this->walls = graph.getWalls();

        display_maze();
    }

    void on_button_clicked_aldous_broder() {

        choice = "aldous"; // Choix de l'utilisateur

        this->nbRows = stoi(entry_rows.get_text());
        this->nbColumns = stoi(entry_columns.get_text());
        Graph graph(this->nbColumns, this->nbRows);
        graph.aldousBroder();
        this->walls = graph.getWalls();


        display_maze();
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

/*
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

        m_button_quit.set_label("Quit");
        m_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::close));
        m_grid.attach(m_button_quit, 0, 3, 2, 1);

        show_all();
    }
*/

    void display_maze() {
        remove_all_widgets(m_grid);

        set_title("Labyrinthe");

        m_drawingArea.set_size_request(get_allocated_width(), get_allocated_height());
        m_grid.attach(m_drawingArea, 0, 0, nbColumns, nbRows);

        m_drawingArea.signal_draw().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_draw_grid));//dessine les murs du labyrinthe
        m_drawingArea.add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK); // Activer les événements clavier
        m_drawingArea.signal_key_press_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_key_press));

        show_all();
        grab_focus();
    }

    bool on_drawingArea_draw_grid(const Cairo::RefPtr<Cairo::Context>& cr) {
        Gtk::Allocation allocation = m_drawingArea.get_allocation();
        pair<int, int> xy;
        
        Graph graph(this->nbColumns, this->nbRows);
        double cellWidth = allocation.get_width() / this->nbColumns;
        double cellHeight = allocation.get_height() / this->nbRows;
        
        // Dessiner l'arrivée
        m_exitX = nbColumns - 1;
        m_exitY = nbRows - 1;
        double exitX = m_exitX * cellWidth;
        double exitY = m_exitY * cellHeight;
        cr->set_source_rgb(1.0, 0.0, 0.0); // Couleur rouge pour l'arrivée
        cr->rectangle(exitX, exitY, cellWidth, cellHeight);
        cr->fill();
        
        // Dessiner le joueur
        double playerX = m_playerX * cellWidth;
        double playerY = m_playerY * cellHeight;
        cr->set_source_rgb(0.0, 1.0, 0.0); // Couleur verte pour le joueur
        cr->rectangle(playerX, playerY, cellWidth, cellHeight);
        cr->fill();

        cr->set_source_rgb(1.0, 1.0, 1.0);
        cr->set_line_width(3.0);

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


        return true;
    }

    bool on_drawingArea_key_press(GdkEventKey* event) {
    	int keyval = event->keyval;
    	cout<<"key val:"<<keyval<<endl;
    	switch(keyval){
    		case 122: // up
    			if (m_playerY > 0 && withoutWalls(m_playerX, m_playerY, m_playerX, m_playerY-1)) { 
                		m_playerY--;
                		m_drawingArea.queue_draw();
                	}
                	break;
    		
		case 115: // down
		    if (m_playerY < nbRows - 1 &&  withoutWalls(m_playerX, m_playerY, m_playerX, m_playerY+1)) {
			m_playerY++;
			m_drawingArea.queue_draw();
		    }
		    break;
	
		case 113: // left
		    if (m_playerX > 0 && withoutWalls(m_playerX, m_playerY, m_playerX-1, m_playerY)) {
		        m_playerX--;
		        m_drawingArea.queue_draw();
		    }
		    break;
		
		case 100: // right 
		    if (m_playerX < nbColumns - 1 && withoutWalls(m_playerX, m_playerY, m_playerX+1, m_playerY)) {
		        m_playerX++;
		        m_drawingArea.queue_draw();
		    }
        }
        
        if(m_playerX==nbColumns-1 && m_playerY==nbRows-1){
        	cout<<"victoire"<<endl;
        	victory();
        }

        return true;
    }
    
    bool withoutWalls(int m_playerX, int m_playerY, int m_playerX_dest, int m_playerY_dest){
	int id = getId(m_playerX, m_playerY);
	int id_dest = getId(m_playerX_dest, m_playerY_dest);
	pair<int,int> xy = make_pair(id, id_dest);
	cout<<id<<id_dest<<endl;
	if (find(walls.begin(), walls.end(), xy) == walls.end()) {
		return true;
	}
	return false;	
    }
    
    int getId(int m_playerX, int m_playerY){
    	int id=0;
    	for(int i=0; i<nbRows; i++){
    		for(int j=0; j<nbColumns; j++){
    			if(m_playerX==j && m_playerY==i){
    				return id;
    			}
    			id++;
    		}
    	
    	}
    	return -1;
    }
    
    void victory(){
    
    	remove_all_widgets(m_grid);

        set_title("Victoire");
    
	m_label.set_markup("<span size='large'>\n\t\t\t\tBravo, gagné !\n\n Votre score est sauvegardé dans le fichier 'score.txt'</span>\n");
	m_label.set_hexpand(true);
	m_grid.attach(m_label, 0, 2, 2, 1);

    	m_button_aldous_broder.set_label("Continuer");
        m_button_aldous_broder.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::init_game));
        m_grid.attach(m_button_aldous_broder, 0, 3, 2, 1);

        m_button_quit.set_label("Quit");
        m_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::close));
        m_grid.attach(m_button_quit, 0, 4, 2, 1);
        
        show_all();
    
    }
    
    void init_game(){
    
	delete this; // Détruit l'instance actuelle de la fenêtre
	//MazeWindow mazeWindow;
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
        
	signal_key_press_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_key_press));
	

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

        m_button_quit.set_label("Quit");
        m_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::close));
        m_grid.attach(m_button_quit, 0, 3, 2, 1);

        show_all();
    }

    void display_maze() {
        remove_all_widgets(m_grid);

        set_title("Labyrinthe");
        this->nbRows = stoi(entry_rows.get_text());
        this->nbColumns = stoi(entry_columns.get_text());

        m_drawingArea.set_size_request(get_allocated_width(), get_allocated_height());
        m_grid.attach(m_drawingArea, 0, 0, nbColumns, nbRows);

        m_drawingArea.signal_draw().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_draw_grid));//dessine les murs du labyrinthe
        m_drawingArea.add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK); // Activer les événements clavier
        m_drawingArea.signal_key_press_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_key_press));

        show_all();
        grab_focus();
    }

    bool on_drawingArea_draw_grid(const Cairo::RefPtr<Cairo::Context>& cr) {
        Gtk::Allocation allocation = m_drawingArea.get_allocation();
        pair<int, int> xy;

        Graph graph(this->nbColumns, this->nbRows);
        if (choice == "fusion") {
            graph.fusion();
            this->walls = graph.getWalls();
        } else {
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
        m_exitX = nbColumns - 1;
        m_exitY = nbRows - 1;
        double exitX = m_exitX * cellWidth;
        double exitY = m_exitY * cellHeight;
        cr->set_source_rgb(1.0, 0.0, 0.0); // Couleur rouge pour l'arrivée
        cr->rectangle(exitX, exitY, cellWidth, cellHeight);
        cr->fill();

        return true;
    }

    bool on_drawingArea_key_press(GdkEventKey* event) {
    	int keyval = event->keyval;
    	cout<<"key val:"<<keyval<<endl;
    	switch(keyval){
    		case 122:
    			if (m_playerY > 0) {
                		m_playerY--;
                		m_drawingArea.queue_draw();
                	}
                	break;
    		
		case 115:
		    if (m_playerY < nbRows - 1) {
			m_playerY++;
			m_drawingArea.queue_draw();
		    }
		    break;
	
		case 113:
		    if (m_playerX > 0) {
		        m_playerX--;
		        m_drawingArea.queue_draw();
		    }
		    break;
		
		case 100:  
		    if (m_playerX < nbColumns - 1) {
		        m_playerX++;
		        m_drawingArea.queue_draw();
		    }
        }

        return true;
    }
};
*/

