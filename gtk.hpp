#pragma once

#include <gtkmm.h>
#include "graph.hpp"

using namespace std;

// La classe MazeWindow hérite de Window pour utiliser ces méthodes 
class MazeWindow : public Gtk::Window {
protected:
    int nbRows; // Nombre de ligne dans le labyrinthe
    int nbColumns; // Nombre de colonnes dans le labyrinthe
    vector<std::pair<int, int>> walls; // Liste des murs présent dans le labyrinthe
    vector<int> solution; // Liste d'id représentant le chemin le plus court pour sortir du labyrinthe
    bool solution_statement; // Flag qu indique si la solution est demandé ou non par l'utilisateur
    bool bad_insert; // Falg qui indique si il y a une mauvaise insertion de la part de l'utilisateur


    int m_playerX, m_playerY; // Position du joueur sur la DrawingArea
    int m_exitX, m_exitY; // Position de la sortie du labyrinthe sur la DrawingArea
    string choice; // Choix de l'algorithme par l'utilisateur
    chrono::steady_clock::time_point start_time; // Chronomètre utile pour le calcul du score

    Gtk::Label m_label; // Texte affiché dans le menu
    Gtk::DrawingArea m_drawingArea; // Zone de dessin pour afficher le labyrinthe
    Gtk::Grid m_grid; // Grille sur laquelle on attache les widgets à visualiser
    Gtk::Button m_button_fusion; // Bouton choix algo fusion
    Gtk::Button m_button_aldous_broder; // Bouton choix algo aldous-broder
    Gtk::Button m_button_quit; // Bouton quitter le programme
    Gtk::Entry entry_rows, entry_columns; // Zone d'insertion pour l'utilisateur
    Gtk::Button m_button_continue; // Bouton continuer de jouer

public:
    
    // Constructeur 
    MazeWindow(); 

    // Méthodes utilisées
    void on_button_clicked_fusion();
    void on_button_clicked_aldous_broder();
    void close();
    void remove_all_widgets(Gtk::Grid& grid);
    void display_maze();
    bool on_drawingArea_draw_grid(const Cairo::RefPtr<Cairo::Context>& cr);
    bool on_drawingArea_key_press(GdkEventKey* event) ;
    bool withoutWalls(int m_playerX, int m_playerY, int m_playerX_dest, int m_playerY_dest);
    int getId(int m_playerX, int m_playerY);
    void victory();
    void init_game();
    int getScore();
    
};






