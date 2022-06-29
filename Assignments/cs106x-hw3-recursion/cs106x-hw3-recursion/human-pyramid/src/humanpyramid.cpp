/*
 * File: humanpyramid.cpp
 * --------------------------
 * Name:
 * Section leader:
 * This file implements the human pyramid function for Homework 3.
 */

#include "humanpyramid.h"
#include <iostream>
#include "hashmap.h"

using namespace std;

/**
 * Given a human pyramid in which each person weighs exactly 200 pounds,
 * returns the weight on the back of the person at the specified row and
 * column in the human pyramid. Rows and columns are zero-indexed, so,
 * for example, weightOnBackOf(2, 0) would give the weight on the back of
 * the leftmost person in the third row of the pyramid.
 *
 * @param row The zero-indexed row of the person in the pyramid.
 * @param col The zero-indexed column of the person in the pyramid.
 * @return The weight on the back of that person.
 */
double weightOnBackOf(int row, int col) {
    static HashMap<int, double> results;
    double result;
    if (row == 0 && col == 0) {
        return 0;
    }
    int index = row-1 + row + col;
    if (results.containsKey(index)) {
        result = results[index];
    }
    else {
        if (col == 0) {
            result = 100 + 0.5*weightOnBackOf(row-1, 0);
        }
        else if (col == row) {
            result = 100 + 0.5 * weightOnBackOf(row-1, col-1);
        }
        else {
            result = 200 + 0.5*weightOnBackOf(row-1, col-1) + 0.5*weightOnBackOf(row-1, col);
        }
        results.put(index, result);
    }
    return result;
}
