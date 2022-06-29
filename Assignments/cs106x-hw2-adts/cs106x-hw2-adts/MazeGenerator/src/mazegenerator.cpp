/*
 * Maze generator
 * by Lesley Wagner
 * 31-10-2018
 *
 * Animates the generation of an n by n maze.
 */

#include "vector.h"
#include "mazetypes.h"
#include "mazegui.h"
#include "hashset.h"

using namespace std;

HashSet<HashSet<Cell>> create_chambers(int&);
void remove_wall(HashSet<HashSet<Cell>>&, Vector<Wall>&);

/*
 * Generates maze and and displays it in a GUI.
 */
void generateMaze() {
    int num_cells;
    HashSet<HashSet<Cell>> chambers = create_chambers(num_cells);
    Vector<Wall> walls = MazeGUI::getAllWalls();
    // Remove num_cells - 1 number of walls to create a path through the maze.
    // After completion the maze has chamber containing all cells (path through all cells).
    for (int i = 0; i <= num_cells; i++) {
        remove_wall(chambers, walls);
    }
}

/*
 * Create one chamber for every cell and return the chambers.
 */
HashSet<HashSet<Cell>> create_chambers(int& num_cells) {
    HashSet<HashSet<Cell>> chambers;
    Vector<Cell> cells = MazeGUI::getAllCells();
    num_cells = cells.size();
    for (const Cell& cell : cells) {
        chambers.add(HashSet<Cell> {cell});
    }
    return chambers;
}

/*
 * Remove random wall from the maze. A wall should not be removed if the cells
 * on each side of the wall are already reachable from each other.
 */
void remove_wall(HashSet<HashSet<Cell>>& chambers, Vector<Wall>& walls) {
    Wall wall;
    HashSet<Cell> chamber1;
    HashSet<Cell> chamber2;
    // If the wall contains cells that are not reachable from each other,
    // remove it and break out of the loop.
    for (int i = 0; i < walls.size(); i++) {
        wall = walls[i];
        for (const HashSet<Cell>& chamber : chambers) {
            if (chamber.contains(wall.one)) {
                chamber1 = chamber;
            }
            if (chamber.contains(wall.two)) {
                chamber2 = chamber;
            }
        }
        if (chamber1 != chamber2) {
            MazeGUI::removeWall(wall);
            walls.removeValue(wall);
            break;
        }
    }
    // Connect chambers from which the wall is removed.
    chambers.remove(chamber1);
    chambers.remove(chamber2);
    chamber2.addAll(chamber1);
    chambers.add(chamber2);
}
