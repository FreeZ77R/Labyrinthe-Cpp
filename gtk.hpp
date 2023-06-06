#pragma once

#include <gtkmm.h>
#include "graph.hpp"



using namespace std;

class MazeWindow : public Gtk::Window {
protected:
    int nbRows;
    int nbColumns;
    std::vector<Cell> mazeCells;
    std::vector<std::pair<int, int>> walls;
    vector<int> solution;
    bool solution_statement;


    int m_playerX, m_playerY;
    int m_exitX, m_exitY;
    string choice;
    std::chrono::steady_clock::time_point start_time;

    Gtk::Label m_label;
    Gtk::DrawingArea m_drawingArea;
    Gtk::Grid m_grid;
    Gtk::Button m_button_fusion;
    Gtk::Button m_button_aldous_broder;
    Gtk::Button m_button_quit;
    Gtk::Button m_solve;
    Gtk::Entry entry_rows, entry_columns;
    Gtk::Button m_button_valider;

public:
    MazeWindow(); 

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






