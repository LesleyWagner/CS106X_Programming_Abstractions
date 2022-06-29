/*
 * Line Manager
 * by Lesley Wagner
 * 22-11-2018
 *
 * Class that stores all lines of the current webpage,
 * and provides information about which lines should be displayed onscreen.
 */

#include "LineManager.h"
using namespace std;

LineManager::LineManager(const Vector<Line *>& lines) {
   sortedLines.addAll(lines);
}

LineManager::~LineManager() {}

double LineManager::contentHeight() const {
    return sortedLines[sortedLines.size()-1]->highY();
}

Vector<Line *> LineManager::linesInRange(double lowY, double highY) const {
    Vector<Line *> lines;
    for (Line* line : sortedLines) {
        if (line->lowY() <= highY && line->highY() >= lowY) {
            lines.add(line);
        }
    }
    return lines;
}

Line* LineManager::lineAt(double y) const {
    Line* target = nullptr;
    for (Line* line : sortedLines) {
        if (line->lowY() <= y && line->highY() >= y) {
            target = line;
        }
    }
    return target;
}

ostream& operator <<(ostream& out, const LineManager& manager) {
    for (Line* line : manager.sortedLines) {
        out << "\n low: " << line->lowY() << ", high: " << line->highY();
    }
    return out;
}

