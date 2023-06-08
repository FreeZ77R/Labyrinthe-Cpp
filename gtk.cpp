#include <ctime>
#include <chrono>
#include <fstream>
#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/box.h>
#include "graph.hpp"
#include "gtk.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <exception>
#include <gdk/gdk.h>

using namespace std;

// Constructeur qui initialise et affiche le lancement du jeu 
MazeWindow::MazeWindow() {
	
	set_default_size(400, 400);

	m_grid.set_row_spacing(5);
	m_grid.set_column_spacing(5);

	set_title("Lancement");
	
	Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file("menu.png");
	if(pixbuf){
		int newWidth = pixbuf->get_width()*0.7;
		int newHeight = pixbuf->get_height()*0.7;
		Glib::RefPtr<Gdk::Pixbuf> resize_pixbuf = pixbuf->scale_simple(newWidth, newHeight, Gdk::INTERP_BILINEAR);
		image.set(resize_pixbuf);
	}
	m_grid.attach(image,0,1,2,1);

	m_button_continue.set_label("Jouer");
        m_button_continue.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::init_game));
        m_grid.attach(m_button_continue, 0, 3, 2, 1);
	
	m_button_quit.set_label("Quit");
        m_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::close));
        m_grid.attach(m_button_quit, 0, 4, 2, 1);
        
        
        this->add_events(Gdk::KEY_PRESS_MASK);
        // Permet les callback lié aux touches
	this->signal_key_release_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_key_press));


	add(m_grid);
	show_all();
}

   
// L'utilisateur choisit l'algorithme fusion alors on récupère toutes les informations du Graph nécéssaire à l'affichage
void MazeWindow::on_button_clicked_fusion() {

	try{
		this->nbRows = stoi(entry_rows.get_text());// Choix du nombre de ligne
        	this->nbColumns = stoi(entry_columns.get_text());// Choix du nombre de colonnes
	}catch (const exception &e){bad_insert=true;}

	if(!bad_insert){
		choice = "fusion"; // Choix de l'utilisateur
		Graph graph(this->nbColumns, this->nbRows);// Création d'un Graph
		graph.fusion(); // Appel de l'algo fusion qui intialise les murs
		this->walls = graph.getWalls();// On récupère les murs
		this->solution = graph.getSolution(); // On récupère la solution

		display_maze(); //Affichage du labyrinthe 
	}else{
		cerr<<"Nombre de lignes ou colonnes saisies invalide"<<endl;
		init_game();
	}
}

// L'utilisateur choisit l'algorithme puis on récupère toutes les informations du Graph nécéssaire à l'affichage
void MazeWindow::on_button_clicked_aldous_broder() {

        try{
		this->nbRows = stoi(entry_rows.get_text());// Choix du nombre de ligne
        	this->nbColumns = stoi(entry_columns.get_text());// Choix du nombre de colonnes
	}catch (const exception &e){bad_insert=true;}
        
        if(!bad_insert){
		choice = "aldous"; // Choix de l'utilisateur
		Graph graph(this->nbColumns, this->nbRows);// Création d'un Graph
		graph.aldousBroder();// Appel de l'algo fusion qui intialise les murs
		this->walls = graph.getWalls();// On récupère les murs
		this->solution = graph.getSolution();// On récupère la solution
		
		display_maze();//Affichage du labyrinthe 
	}else{
		cout<<"Nombre de lignes ou colonnes saisies invalide"<<endl;
		init_game();
	}      
}

// L'utilisateur choisit de quitter le programme
void MazeWindow::close() {
        Gtk::Window::close();
}

//Permet de retirer l'ensemble des widgets présent sur une grille 
void MazeWindow::remove_all_widgets(Gtk::Grid& grid) {
        vector<Gtk::Widget*> children = grid.get_children();

        for (Gtk::Widget* child : children) {
            grid.remove(*child);
        }
}

//Gestion de l'affichage du labyrinthe et des interruption clavier
void MazeWindow::display_maze() {
        remove_all_widgets(m_grid);

        set_title("Labyrinthe");
        
        //Démarre le timer pour calculer le score de l'utilisateur
        start_time = chrono::steady_clock::now();
        
	m_drawingArea.set_size_request(get_allocated_width(), get_allocated_height());
        m_grid.attach(m_drawingArea, 0, 0, nbColumns, nbRows);

        m_drawingArea.signal_draw().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_draw_grid));//Dessine les murs du labyrinthe
        m_drawingArea.add_events(Gdk::KEY_PRESS_MASK); // Activer les événements clavier
        m_drawingArea.signal_key_press_event().connect(sigc::mem_fun(*this, &MazeWindow::on_drawingArea_key_press));

        show_all();
}




//Affichage du labyrinthe (murs, joueur, arrivé et solution)
bool MazeWindow::on_drawingArea_draw_grid(const Cairo::RefPtr<Cairo::Context>& cr) {
        pair<int, int> xy;
	
	
	set_title("Jeu du labyrinthe                Score : "+to_string(getScore()));
	
	
	// Redimensionner la zone de dessin à la taille de la fenetre
	m_drawingArea.set_size_request(get_allocated_width(), get_allocated_height());

	
	
	
	
	
	
	
        // Couleur de l'arriere plan gris
	cr->set_source_rgb(0.2, 0.2, 0.2);
	cr->rectangle(0, 0, m_drawingArea.get_allocated_width(), m_drawingArea.get_allocated_height());
	cr->fill();

        
        Graph graph(this->nbColumns, this->nbRows);
        double cellWidth = get_allocated_width() / this->nbColumns;
        double cellHeight = get_allocated_height() / this->nbRows;
        
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
        double playerX = m_playerX * cellWidth; // Position graphique du joueur sur l'axe x
        double playerY = m_playerY * cellHeight; // Position graphique du joueur sur l'axe y
        cr->set_source_rgb(0.0, 1.0, 0.0); // Couleur verte pour le joueur
        cr->rectangle(playerX, playerY, cellWidth, cellHeight);
        cr->fill();

        cr->set_source_rgb(1.0, 1.0, 1.0); // Couleur blanche pour la grille 
        
        //Dessiner les coutours 
        cr->move_to(0, 0);
        cr->line_to(nbColumns*cellWidth, 0);
        cr->move_to(nbColumns*cellWidth, 0);
        cr->line_to(nbColumns*cellWidth, nbRows*cellHeight);
        cr->move_to(nbColumns*cellWidth, nbRows*cellHeight);
        cr->line_to(0, nbRows*cellHeight);
        cr->move_to(0, nbRows*cellHeight);
        cr->line_to(0, 0);

	// Dessine les murs 
        for (const auto& wall : this->walls) {
            if (wall.second == wall.first + 1) {
                xy = graph.getXY(wall.first, wall.second, nbRows, nbColumns, get_allocated_width(), get_allocated_height());
                cr->move_to(xy.first, xy.second);
                cr->line_to(xy.first, xy.second + cellHeight);
            }

            if (wall.second == wall.first + nbColumns) {
                xy = graph.getXY(wall.first, wall.second, nbRows, nbColumns, get_allocated_width(), get_allocated_height());
                cr->move_to(xy.first, xy.second);
                cr->line_to(xy.first + cellWidth, xy.second);
            }

            
        }
	cr->stroke();
	
	cr->set_source_rgb(0.0, 1.0, 0.0);
	// Déblocage de l'affichage de la solution
	if(solution_statement==true){

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

//Interprétation des interruptions clavier et détection si partie gagné
bool MazeWindow::on_drawingArea_key_press(GdkEventKey* event) {
    	guint keyval = event->keyval;
    	if(keyval==GDK_KEY_Up || keyval==122){ //up
		if (m_playerY > 0 && withoutWalls(m_playerX, m_playerY, m_playerX, m_playerY-1)) { 
			m_playerY--;
			m_drawingArea.queue_draw();
		}
        }
    		
	else if(keyval==GDK_KEY_Down || keyval==115){ // down
		if (m_playerY < nbRows - 1 &&  withoutWalls(m_playerX, m_playerY, m_playerX, m_playerY+1)) {
			m_playerY++;
			m_drawingArea.queue_draw();
		}
	}
	
	else if(keyval==GDK_KEY_Left || keyval==113){ // left
		if (m_playerX > 0 && withoutWalls(m_playerX, m_playerY, m_playerX-1, m_playerY)) {
			m_playerX--;
			m_drawingArea.queue_draw();
		}
	}
		
	else if(keyval==GDK_KEY_Right || keyval==100){ // right 
		if (m_playerX < nbColumns - 1 && withoutWalls(m_playerX, m_playerY, m_playerX+1, m_playerY)) {
			m_playerX++;
			m_drawingArea.queue_draw();
		}
	}
	
	if(keyval==116){ //triche (solution)
		solution_statement=true;
		m_drawingArea.queue_draw();    
        }
        
        if(m_playerX==nbColumns-1 && m_playerY==nbRows-1){
        	victory();
        }

        return true;
}

//A partir des coordonées graphique indique s'il y a un mur entre la position actuelle et la position futur
bool MazeWindow::withoutWalls(int m_playerX, int m_playerY, int m_playerX_dest, int m_playerY_dest){
	int id = getId(m_playerX, m_playerY);
	int id_dest = getId(m_playerX_dest, m_playerY_dest);
	pair<int,int> xy = make_pair(id, id_dest);
	if (find(walls.begin(), walls.end(), xy) == walls.end()) {
		return true;
	}
	return false;	
 }
    
//A partir des coordonnées graphique renvoit l'id de la cellule
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
    
//Affichage d'un nouvelle fenetre en cas de victoire (score, sauvegarde du score, menu de sélection utilisateur)
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

    	m_button_continue.set_label("Continuer de jouer");
        m_button_continue.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::init_game));
        m_grid.attach(m_button_continue, 0, 3, 2, 1);

        m_button_quit.set_label("Quit");
        m_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::close));
        m_grid.attach(m_button_quit, 0, 4, 2, 1);
        
        show_all();
    
}
   
//Permet de relancer le menu principale si l'utilisateur souhaite continuer de jouer
void MazeWindow::init_game(){
    
	remove_all_widgets(m_grid);

	solution_statement=false;
	bad_insert=false;
	
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

	m_button_random.set_label("Dimensions et algorithme aléatoire");
	m_button_random.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::on_button_clicked_random));
	m_grid.attach(m_button_random, 0, 4, 2, 1);

        m_button_quit.set_label("Quit");
        m_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &MazeWindow::close));
        m_grid.attach(m_button_quit, 0, 5, 2, 1);
        
	m_label.set_markup("<span size='large'>\n\t\t\t\t\t\t\t  Règles du jeu : \n\n\t Rejoins la sortie rouge du labyrinthe au plus vite afin d'avoir le meilleure score\n\n\n\t\t\t\t\t\t\t  Modes de jeu :  \n\n Saisis les dimensions de ton labyrinthe et clique sur l'algorithme que tu souhaites \n\t  Si tu es joueur, alors utilise le mode 'dimensions et algorithme aléatoire ! \n\n\n\t\t\t\t\t\t\t\t   Info : \n\n\t\t Déplace toi en utilisant les flèches directionnelles de ton clavier \n Ce labyrinthe est trop dur pour toi, utilise le code triche pour afficher la solution : 't'</span>");
	m_label.set_hexpand(true);
	m_grid.attach(m_label, 0, 6, 2, 1);
        
	
        show_all();
}
    
//Permet de renvoyer le score de l'utilisateur en cas de victoire
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
    
void MazeWindow::on_button_clicked_random(){
	
	int rd_rows = rand()%40 + 3;
	int rd_columns = rand()%40 + 3;
	int rd_choice = rand()%2;
	this->nbRows = rd_rows;// Choix du nombre de ligne
	this->nbColumns = rd_columns;// Choix du nombre de colonnes
	
	Graph graph(this->nbColumns, this->nbRows);// Création d'un Graph
	
        if(rd_choice==0){
        	graph.fusion();
        }else{graph.aldousBroder();}

	this->walls = graph.getWalls();// On récupère les murs
	this->solution = graph.getSolution();// On récupère la solution
	
	display_maze();//Affichage du labyrinthe 
}





