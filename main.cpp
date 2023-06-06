#include <ctime>
#include "gtk.hpp"


int main() {
    srand (time(NULL));
    
    auto app = Gtk::Application::create();
    MazeWindow mazeWindow;
    return app->run(mazeWindow);
}







