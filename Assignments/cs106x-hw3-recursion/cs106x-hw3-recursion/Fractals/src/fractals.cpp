/*
 * File: fractals.cpp
 * --------------------------
 * Name:
 * Section leader:
 * This file contains fractal problems for CS106X.
 */


#include "fractals.h"
#include <cmath>
#include "gbufferedimage.h"
#include "plasmacolor.h"
#include "gtypes.h"

using namespace std;

/**
 * Draws a Sierpinski triangle of the specified size and order, placing its
 * bottom-left corner at position (x, y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param window - The window in which to draw the Sierpinski triangle.
 * @param x - The x coordinate of the bottom-left corner of the triangle.
 * @param y - The y coordinate of the bottom-left corner of the triangle.
 * @param sideLength - The length of one side of the triangle.
 * @param order - The order of the fractal.
 */
void drawSierpinskiTriangle(GWindow& window, double x, double y, double sideLength, int order) {
    // illegal arguments
    if (x < 0 || y < 0 || sideLength < 0 || order < 0) {
        throw "This function cannot take negative parameters";
    }
    if (order == 0) {
        GPoint point1(x, y);
        GPoint point2(x+sideLength/2, y-sqrt(0.75*sideLength*sideLength));
        GPoint point3(x+sideLength, y);
        window.drawLine(point1, point2);
        window.drawLine(point2, point3);
        window.drawLine(point3, point1);
    }
    else {
        // one upside down triangle inside
        double calc = sqrt(0.1875*sideLength*sideLength);
        GPoint point1(x+0.25*sideLength, y-calc);
        GPoint point2(x+0.75*sideLength, y-calc);
        GPoint point3(x+sideLength/2, y);
        // three more triangles inside the corners
        double new_sideLength = sideLength/2;
        drawSierpinskiTriangle(window, x, y, new_sideLength, order-1);
        drawSierpinskiTriangle(window, point3.getX(), y, new_sideLength, order-1);
        drawSierpinskiTriangle(window, point1.getX(), point1.getY(), new_sideLength, order-1);
    }
}

/*
 * Draw branch at degree theta relative to the origin
 *
 * @param window - The window in which to draw the recursive tree.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 * @param theta - The angle relative to the origin.
 * @param order - The order of the fractal.
 */
void drawBranch(GWindow& window, double x, double y, double size, int theta, int order) {
    if (order == 1) {
        window.setColor("#2e8b57");
        window.drawPolarLine(x, y, size, theta);
    }
    else {
        window.setColor("#8b7765");
        GPoint new_point = window.drawPolarLine(x, y, size, theta);
        double new_size = size/2;
        for (int i = -45; i <= 45; i += 15) {
            drawBranch(window, new_point.getX(), new_point.getY(), new_size, theta+i, order-1);
        }
    }
}

/*
 * Draws a recursive tree fractal image of the specified size and order,
 * placing the bounding box's top-left corner at position (x,y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param window - The window in which to draw the recursive tree.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 * @param order - The order of the fractal.
 */
void drawTree(GWindow& window, double x, double y, double size, int order) {
    // illegal arguments
    if (x < 0 || y < 0 || size < 0 || order <= 0) {
        throw "This function cannot take negative parameters";
    }
    drawBranch(window, x+size/2, y+size, size/2, 90, order);
}

// helper function: determines whether a complex number is in the mandelbrot set
void pixel_in_set(int& pixel, const Complex& number, Complex z, int iterations, const int color) {
    if (iterations == 0) {
        pixel = color;
    }
    else {
        z = z*z + number;
        if (z <= 4) {
            pixel_in_set(pixel, number, z, iterations-1, color);
        }
    }
}

/**
 * Draws a Mandelbrot Set in the graphical window at the specified location/size,
 * with maxIterations and in a given color.
 *
 * This will be called by fractalgui.cpp.
 *
 * @param window - The window in which to draw the Mandelbrot set.
 * @param leftX - The x coordinate of the top-left corner of the bounding box.
 * @param topY - The y coordinate of the top-left corner of the bounding box.
 * @param min - the minimum Complex number you should examine
 * @param max - the maximum Complex number you should examine
 * @param iterations - The maximum number of iterations to run recursive step
 * @param color - The color of the fractal
 */
void drawMandelbrotSet(GWindow& window, double leftX, double topY, double size,
                       const Complex& min, const Complex& max,
                       int iterations, int color) {
    GBufferedImage image(size, size, 0xffffff);
    window.add(&image, leftX, topY);
    Grid<int> pixels = image.toGrid(); // Convert image to grid
    double x;
    double y;
    double range_x = abs(max.real() - min.real());
    double range_y = abs(max.imag() - min.imag());
    cout << "range: " << range_x <<  ", " << range_y << "\n";
    cout.flush();
    for (int i = 0; i < size; i++) {
        x = range_x*(i/size)+min.real();
        for (int j = 0; j < size; j++) {
            y = range_y*(j/size)+min.imag();
            // cout << x << ", " << y << "\n";
            pixel_in_set(pixels[j][i], Complex(x, y), 0, iterations, color);
        }
    }

    image.fromGrid(pixels); // Converts and puts the grid back into the image onscreen
}

/**
 * Generates a random displacement value you can add to a color to randomly shift it.
 *
 * You should call this function when calculating middle color values.
 *
 * @param newWidth - the width of the subdivided rectangles you are creating at that moment
 * @param newHeight - the height of the subdivided rectangles you are creating at that moment
 * @param totalWidth - the width of the entire plasma fractal (not just the part you are filling in at the moment)
 * @param totalHeight - the height of the entire plasma fractal (not just the part you are filling in at the moment)
 */
double displace(double newWidth, double newHeight, double totalWidth, double totalHeight) {
    double max = 3*(newWidth + newHeight) / (totalWidth + totalHeight);
    return (randomReal(0, 1) - 0.5) * max;
}

/**
 * Colors a rectangle in the graphical window at the specified location/size.
 *
 * This will be called by fractalgui.cpp.
 *
 * @param window - The window in which to draw the fractal.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 */
void colorRectangle(Grid<int>& pixels, double x, double y, double size, double totalSize,
                    PlasmaColor lt, PlasmaColor rt, PlasmaColor rb, PlasmaColor lb) {
    double new_size = size/2;
    if (size <= 1) {
        pixels[x][y] = ((lt+rt+rb+lb)/4.0).toRGBColor();
    }
    else {
        PlasmaColor top = (lt+rt)/2;
        PlasmaColor bottom = (rb+lb)/2;
        PlasmaColor left = (lt+lb)/2;
        PlasmaColor right = (rt+rb)/2;
        PlasmaColor middle = (lt+rt+rb+lb)/4 - displace(new_size, new_size, totalSize, totalSize);
        colorRectangle(pixels, x, y, new_size, totalSize, lt, top, middle, left);
        colorRectangle(pixels, x+new_size, y, new_size, totalSize, top, rt, right, middle);
        colorRectangle(pixels, x+new_size, y+new_size, new_size, totalSize, middle, right, rb, bottom);
        colorRectangle(pixels, x, y+new_size, new_size, totalSize, left, middle, bottom, lb);
    }
}

/**
 * Draws a Plasma Fractal in the graphical window at the specified location/size.
 *
 * This will be called by fractalgui.cpp.
 *
 * @param window - The window in which to draw the fractal.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 */
void drawPlasma(GWindow &window, double x, double y, double size) {
    GBufferedImage image(size, size, 0xffffff);
    window.add(&image, x, y);
    Grid<int> pixels = image.toGrid(); // Convert image to grid
    colorRectangle(pixels, 0, 0, size, size, PlasmaColor(), PlasmaColor(), PlasmaColor(), PlasmaColor());
    image.fromGrid(pixels); // Converts and puts the grid back into the image onscreen
}
