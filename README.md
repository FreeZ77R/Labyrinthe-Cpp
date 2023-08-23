# Labyrinthe C++

## Project information 

This involves programming in C++ the creation and graphical visualization of a labyrinth
which a user must exit from an initial position to a final position
output. For the GUI, you will use the Gtkmm library. The user
will move through the maze using the 4 arrows on the computer keyboard.
You will propose several mazes of variable sizes built automatically.
The automatic generation of mazes will necessarily follow two algorithms: Fu-
and Aldous-Broder, presented in the resources given below.
The following resources will be helpful:

- https://info.blaisepascal.fr/nsi-labyrinthes#Construction_dun_labyrinthe

- https://fr.wikipedia.org/wiki/Modélisation_mathématique_d’un_labyrinthe

Remember to implement the notion of graph as indicated by the first url.
The project is not frozen, you can bring all the extensions you want.

## Using it

Before downloading anything you need to install the gtkmm libraries on your working environment,
the project was done under gtkmm3-0. Gtkmm is a GUI toolkit designed to work well with all other C++ libraries.

You can refer to this page for installation : https://www.gtkmm.org/en/download.html

The project-COO file and the .tar.gz archive contain the code of the labyrinth. Once downloaded moved to the main project folder and then used the following commands to recompile the files: 

- make clean
- make
- ./app
