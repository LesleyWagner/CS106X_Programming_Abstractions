/* Game Of Life
 * by Lesley Wagner
 *
 * Simplified version of Conway's Game Of Life with a basic user interface.
 * Simulates the birth and dead of future generations of cells.
 *
 * Last edited: 19-10-2018
 */

#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
#include "urlstream.h"

using namespace std;

void print_intro();
Grid<int> get_grid();
void build_grid(Grid<int>&, ifstream&);
string get_grid_string(Grid<int>&);
bool get_wrap_option();
string get_simulation_command();
bool is_another_simulation();
void run_animation(Grid<int>&, bool);
void run_tick(Grid<int>&, bool);
int get_neighbors(Grid<int>&, int, int, bool);
void make_screenshot();

const int ANIMATION_PAUSE = 50;

int main() {
    print_intro();
    while (true) {
        Grid<int> cells = get_grid();
        bool wrap = get_wrap_option();
        LifeGUI::initialize();
        LifeGUI::resize(cells.numRows(), cells.numCols());
        cout << get_grid_string(cells);

        // Ask user for simulation command and perform the corresponding action
        while (true) {
            string command = get_simulation_command();
            if (command == "a" || command == "A") {
                run_animation(cells, wrap);
            }
            else if (command == "t" || command == "T") {
                run_tick(cells, wrap);
            }
            else if (command == "s" || command == "S") {
                make_screenshot();
            }
            else {
                // quit current simulation
                break;
            }
        }
        if (!is_another_simulation()) {
            // quit program
            break;
        }
    }
    return 0;
}

// print introductory message
void print_intro() {
    const string intro_message = "Welcome to the CS 106X Game of Life,\n"
    "a simulation of the lifecycle of a bacteria colony.\n"
    "Cells (X) live and die by the following rules:\n"
    "- A cell with 1 or fewer neighbors dies.\n"
    "- Locations with 2 neighbors remain stable.\n"
    "- Locations with 3 neighbors will create life.\n"
    "- A cell with 4 or more neighbors dies.\n"
    "- A cell that has lived for more than 10 generations dies.\n\n";

    cout << intro_message;
}

// Get a grid of cells from a file, from a url or make a random grid and return it.
Grid<int> get_grid() {
    Grid<int> cells;
    char read_from = 0;
    cout << "Read from (f)ile, (u)rl or (r)andom?";
    cin >> read_from;
    while (read_from != 'f' && read_from != 'u' && read_from != 'r' &&
           read_from != 'F' && read_from != 'U' && read_from != 'R') {
        cout << "Invalid choice; please try again.\n";
        cin >> read_from;
    }
    cin.ignore();
    // read grid from a file
    if (read_from == 'f' || read_from == 'F') {
        string file_name;
        while (true) {
            file_name = getLine("Grid input file name?");
            if (fileExists(file_name)) {
                break;
            }
            cout << "Unable to open that file. Try again.\n";
        }
        ifstream input_file;
        input_file.open(file_name);
        build_grid(cells, input_file);
        input_file.close();
    }
    // read grid from a url
    else if (read_from == 'u' || read_from == 'U') {
        iurlstream urlStream;
        while (true) {
            string url = getLine("Grid input file name?");
            urlStream.open(url);
            if (endsWith(url, ".txt") && urlStream.getErrorCode() == 0) {
                break;
            }
            cout << "Invalid URL; please try again.\n";
        }
        build_grid(cells, urlStream);
        urlStream.close();
    }
    // make a random grid
    else if (read_from == 'r' || read_from == 'R') {
        int rows = getInteger("How many rows?");
        int columns = getInteger("How many columns?");
        cells.resize(rows, columns);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                cells[i][j] = randomBool();
            }
        }
    }
    return cells;
}

void build_grid(Grid<int> &cells, ifstream &input_file) {
    int rows;
    int columns;
    input_file >> rows;
    input_file >> columns;
    cells.resize(rows, columns);
    char line[columns];
    for (int i = 0; i < rows; i++) {
        input_file >> line;
        for (int j = 0; j < columns; j++) {
            if (line[j] == '-') {
                cells[i][j] = 0;
            }
            else if (line[j] == 'X'){
                cells[i][j] = 1;
            }
        }
    }
}

string get_grid_string(Grid<int> &cells) {
    ostringstream grid_string;
    for (int i = 0; i < cells.numRows(); i++) {
        for (int j = 0; j < cells.numCols(); j++) {
            LifeGUI::fillCell(i, j, cells[i][j]);
            if (cells[i][j] == 0) {
                grid_string << "-";
            }
            else if (cells[i][j] < 5) {
                grid_string << "x";
            }
            else {
                grid_string << "X";
            }
        }
        grid_string << "\n";
    }
    LifeGUI::repaint();
    return grid_string.str();
}

bool get_wrap_option() {
    bool wrap;
    string wrap_input = getLine("Should the simulation wrap around the grid (y/n)?");
    while (wrap_input != "y" && wrap_input != "Y" && wrap_input != "n" && wrap_input != "N") {
        cout << "Please type a word that starts with 'Y' or 'N'.";
        wrap_input = getLine("Should the simulation wrap around the grid (y/n)?");
    }
    if (wrap_input == "y" || wrap_input == "Y") {
        wrap = true;
    }
    else {
        wrap = false;
    }
    return wrap;
}

string get_simulation_command() {
    string command;
    while (true) {
        command = getLine("a)nimate, t)ick, s)creenshot, q)uit?");
        if (command == "a" || command == "t" || command == "s" || command == "q" ||
                command == "A" || command == "T" || command == "S" || command == "Q") {
            break;
        }
        cout << "Invalid choice; please try again.\n";
    }
    return command;
}

bool is_another_simulation() {
    bool is_another;
    string wrap_input = getLine("Simulate another colony?");
    while (wrap_input != "y" && wrap_input != "Y" && wrap_input != "n" && wrap_input != "N") {
        cout << "Please type a word that starts with 'Y' or 'N'.";
        wrap_input = getLine("Simulate another colony?");
    }
    if (wrap_input == "y" || wrap_input == "Y") {
        is_another = true;
    }
    else {
        is_another = false;
    }
    return is_another;
}

/* Runs animation of multiple generations of cells.
 * First asks user for how many frames (generations),
 * then performs the given amount of simulations of cells with animation pause.
 *
 * parameter: cells, grid of current generation of cells; wrap, whether the grid should wrap around.
 * 'cells' gets mutated by run_tick. After 'run_animation' has executed it represents the last generation of cells.
 */
void run_animation(Grid<int> &cells, bool wrap) {
    int frames = getInteger("How many frames?");
    for (int i = 0; i < frames; i++) {
        clearConsole();
        LifeGUI::clear();
        run_tick(cells, wrap);
        cout.flush();
        pause(ANIMATION_PAUSE);
    }
}

/* Simulates one generation of cells and prints it to the console.
 * Cells live and die by the following rules:
 * A location that has zero or one neighbors will be empty in the next generation. If a cell was there, it dies.
 * A location with two neighbors is stable. If it had a cell, it still contains a cell. If it was empty, it's still empty.
 * A location with three neighbors will contain a cell in the next generation. If it was unoccupied before, a new cell is born. If it currently contains a cell, the cell remains.
 * A location with four or more neighbors will be empty in the next generation. If there was a cell in that location, it dies of overcrowding.
 * A cell that ages more than 10 generations dies, and will be empty the next generation, regardless of how many neighbors it has.
 *
 * parameter: cells, grid of current generation of cells; wrap, whether the grid should wrap around
 * Modifies 'cells' to represent the next generation of cells.
 */
void run_tick(Grid<int> &cells, bool wrap) {
    Grid<int> new_cells(cells.numRows(), cells.numCols());
    for (int i = 0; i < cells.numRows(); i++) {
        for (int j = 0; j < cells.numCols(); j++) {
            int alive_neighbors = get_neighbors(cells, j, i, wrap);
            if (alive_neighbors < 2) {
                new_cells[i][j] = 0;
            }
            else if (alive_neighbors == 2) {
                if (cells[i][j] > 0) {
                    new_cells[i][j] = cells[i][j] + 1;
                }
                else {
                    new_cells[i][j] = 0;
                }
            }
            else if (alive_neighbors == 3) {
                new_cells[i][j] = cells[i][j] + 1;
            }
            else if (alive_neighbors > 4) {
                new_cells[i][j] = 0;
            }
            if (new_cells[i][j] > 10) {
                new_cells[i][j] = 0;
            }
        }
    }
    cout << get_grid_string(new_cells);
    cells = new_cells;
}

// return number of alive neighbor cells
int get_neighbors(Grid<int> &cells, int x, int y, bool wrap) {
    int offsets[8][2] = {{-1, -1}, {-1, 0}, {-1, +1},
                                 { 0, -1}, { 0, +1},
                                 {+1, -1}, {+1, 0}, {+1, +1}};
    int count = 0;
    int num_cols = cells.numCols();
    int num_rows = cells.numRows();
    for (int i = 0; i < 8; i++) {
        int x_offset = offsets[i][1];
        int y_offset = offsets[i][0];
        if (wrap) {
            if (cells[(y+y_offset+num_rows) % num_rows][(x+x_offset+num_cols) % num_cols] > 0) {
                count++;
            }
        }
        else {
            if (x+x_offset >= 0 && x+x_offset < num_cols &&
                    y+y_offset >= 0 && y+y_offset < num_rows) {
                if (cells[y+y_offset][x+x_offset] > 0) {
                    count++;
                }
            }
        }
    }
    return count;
}

void make_screenshot() {
    string file_name = getLine("Enter a filename to save to:");
    file_name += ".png";
    LifeGUI::screenshot(file_name);
}
