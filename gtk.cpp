#include <ctime>
#include <chrono>
#include <fstream>
#include <gtkmm.h>
#include "graph.hpp"
#include "gtk.hpp"
#include <iostream>
#include <string>
#include <utility>

using namespace std;


MazeWindow::MazeWindow() {
	set_default_size(400, 400);

	solution_statement=false;
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

	m_label.set_markup("<span size='large'>\nComment jouer ? \n\n - Allé vers le haut : 'z' \n - Allé vers le bas : 's' \n - Allé vers la gauche : 'q' \n - Allé vers la droite : 'd' \n\n Code triche (solution) : 't'</span>");
	m_label.set_hexpand(true);
	m_grid.attach(m_label, 0, 5, 2, 1);


	signal_key_press_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_key_press));


	add(m_grid);
	show_all();
}

   

void MazeWindow::on_button_clicked_fusion() {

        choice = "fusion"; // Choix de l'utilisateur
	
	this->nbRows = stoi(entry_rows.get_text());
        this->nbColumns = stoi(entry_columns.get_text());
	Graph graph(this->nbColumns, this->nbRows);
	graph.fusion();
        this->walls = graph.getWalls();
	this->solution = graph.getSolution();

        display_maze();
}

void MazeWindow::on_button_clicked_aldous_broder() {

        choice = "aldous"; // Choix de l'utilisateur

        this->nbRows = stoi(entry_rows.get_text());
        this->nbColumns = stoi(entry_columns.get_text());
        Graph graph(this->nbColumns, this->nbRows);
        graph.aldousBroder();
        this->walls = graph.getWalls();
	this->solution = graph.getSolution();

        display_maze();
}

void MazeWindow::close() {
        Gtk::Window::close();
}

void MazeWindow::remove_all_widgets(Gtk::Grid& grid) {
        vector<Gtk::Widget*> children = grid.get_children();

        for (Gtk::Widget* child : children) {
            grid.remove(*child);
        }
}


void MazeWindow::display_maze() {
        remove_all_widgets(m_grid);

        set_title("Labyrinthe");
        
        //Démarre le timer pour calculer le score de l'utilisateur
        start_time = std::chrono::steady_clock::now();

        m_drawingArea.set_size_request(400, 400);
        m_grid.attach(m_drawingArea, 0, 0, nbColumns, nbRows);

        m_drawingArea.signal_draw().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_draw_grid));//dessine les murs du labyrinthe
        m_drawingArea.add_events(Gdk::KEY_PRESS_MASK); // Activer les événements clavier
        m_drawingArea.signal_key_press_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_key_press));
        


        show_all();
}

bool MazeWindow::on_drawingArea_draw_grid(const Cairo::RefPtr<Cairo::Context>& cr) {
        pair<int, int> xy;
        
        Graph graph(this->nbColumns, this->nbRows);
        double cellWidth = 400 / this->nbColumns;
        double cellHeight = 400 / this->nbRows;
        

        cr->set_line_width(3.0);
        
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

        cr->set_source_rgb(1.0, 1.0, 1.0); // Couleur blanche pour la grille 
        
        //Dessiner les coutours 
        cr->move_to(0, 0);
        cr->line_to(400, 0);
        cr->move_to(400, 0);
        cr->line_to(400, 400);
        cr->move_to(400, 400);
        cr->line_to(0, 400);
        cr->move_to(0, 400);
        cr->line_to(0, 0);

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

            
        }

	// Déblocage de l'affichage de la solution
	if(solution_statement==true){
		cout<<"tricheur"<<endl;
		cr->set_source_rgb(0.0, 1.0, 0.0); // Couleur verte pour solution 
		double pos_X = 0.5*cellWidth;
		double pos_Y = 0.5*cellHeight;
		for(uint32_t i=0; i<solution.size()-1; i++){
			if(solution[i+1]==solution[i]+1){ // Trait vers la gauche
	    			cr->move_to(pos_X, pos_Y);
	    			pos_X += cellWidth;
		        	cr->line_to(pos_X, pos_Y);
		        	
	    		}
	    		else if(solution[i+1]==solution[i]-1){ // Trait vers la droite
	    			cr->move_to(pos_X, pos_Y);
	    			pos_X -= cellWidth;
		        	cr->line_to(pos_X, pos_Y);
	    		
	    		}
	    		else if(solution[i+1]==solution[i]-nbColumns){ // Trait vers le haut
	    			cr->move_to(pos_X, pos_Y);
	    			pos_Y -= cellHeight;
		        	cr->line_to(pos_X, pos_Y);
	    		}
	    		else{	// Trait vers le bas
	    			cr->move_to(pos_X, pos_Y);
	    			pos_Y += cellHeight;
		        	cr->line_to(pos_X, pos_Y);
	    		}	
		}	
	}
	
	cr->stroke();
        return true;
}

bool MazeWindow::on_drawingArea_key_press(GdkEventKey* event) {
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
		    break;
		case 116: //triche (solution)
		    solution_statement=true;
		    m_drawingArea.queue_draw();
		    break;
		    
        }
        
        if(m_playerX==nbColumns-1 && m_playerY==nbRows-1){
        	cout<<"victoire"<<endl;
        	victory();
        }

        return true;
}
    
bool MazeWindow::withoutWalls(int m_playerX, int m_playerY, int m_playerX_dest, int m_playerY_dest){
	int id = getId(m_playerX, m_playerY);
	int id_dest = getId(m_playerX_dest, m_playerY_dest);
	pair<int,int> xy = make_pair(id, id_dest);
	cout<<id<<id_dest<<endl;
	if (find(walls.begin(), walls.end(), xy) == walls.end()) {
		return true;
	}
	return false;	
 }
    
int MazeWindow::getId(int m_playerX, int m_playerY){
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
    
void MazeWindow::victory(){
    
    	//Recupere le score 
	int score = getScore();

	// Obtenir la date et l'heure actuelles
	time_t current_time = time(nullptr);
	tm* time_info = localtime(&current_time);

	// Formater la date et l'heure
	char time_buffer[80];
	strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", time_info);


        // Sauvegarder le score dans un fichier texte
        ofstream score_file("score.txt", ios::app);
        if (score_file.is_open()) {
            if(solution_statement==false){score_file << time_buffer << " - " << score << "\n";}
            score_file << time_buffer << " - " << score << " avec solution" << "\n";
            score_file.close();
        } else {
            std::cerr << "Erreur : impossible d'ouvrir le fichier score.txt" << std::endl;
        }
    
    	// Retire les widgets présent sur la grille
    	remove_all_widgets(m_grid);

        set_title("Victoire");
    
	m_label.set_markup("<span size='large'>\n\t\t\t\tBravo, gagné !\n\n\t\t\t\tVotre score : "+ to_string(score) + "\n\nVotre score est sauvegardé dans le fichier 'score.txt'</span>\n");
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
    
void MazeWindow::init_game(){
    
	remove_all_widgets(m_grid);

	solution_statement=false;
	m_playerX = 0; // Position initiale du joueur en X
        m_playerY = 0; // Position initiale du joueur en Y
	
        // Création d'une grille pour représenter le labyrinthe

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
        
        m_label.set_markup("<span size='large'>\nComment jouer ? \n\n - Allé vers le haut : 'z' \n - Allé vers le bas : 's' \n - Allé vers la gauche : 'q' \n - Allé vers la droite : 'd' \n\n Code triche (solution) : 't'</span>");
	m_label.set_hexpand(true);
	m_grid.attach(m_label, 0, 5, 2, 1);
        
        
	signal_key_press_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_key_press));
	
        show_all();
}
    
int MazeWindow::getScore(){
        //Gestion du score 
    	chrono::steady_clock::time_point end_time = chrono::steady_clock::now(); // Enregistrer le temps de fin

        // Calcul du score
        chrono::seconds duration = chrono::duration_cast<chrono::seconds>(end_time - start_time);
        int duration_in_seconds = duration.count();
        int max_score = nbRows * nbColumns; // Définir le score maximum possible
        int score = max_score - duration_in_seconds;
        return score;
}
    







