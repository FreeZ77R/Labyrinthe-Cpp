#include <ctime>
#include "gtk.hpp"


int main() {
    //Initialisation graine du temps
    srand (time(NULL));
    
    //Création d'une application gtk qui execute un élément de type MazeWindow
    auto app = Gtk::Application::create();
    MazeWindow mazeWindow;
    return app->run(mazeWindow);
}

