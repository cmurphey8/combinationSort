#include "helpers.h"
#import <stdio.h>
#include <stdlib.h>
#include <math.h>

/*  -------------------------------------------------------------------------------------------------------------------------------------
    FUNCTION PROTOTYPES
    -------------------------------------------------------------------------------------------------------------------------------------  */

void selectionSort(int height, int width, HSVTRIPLE image[height][width]);
int findMinIndex(int height, int width, HSVTRIPLE image[height][width], int index[height]);
int findMin(int start, int width, HSVTRIPLE image[width]);

HSVTRIPLE rgbHSV(RGBTRIPLE px);
int max(int x, int y);
int min(int x, int y);
double getHue(RGBTRIPLE px, double cMax, double cMin);
double getSat(RGBTRIPLE px, double cMax, double cMin);

RGBTRIPLE hsvRGB(HSVTRIPLE px);

// combination sort
void combinationSort(int height, int width, RGBTRIPLE image[height][width]) {

    // convert image from rgb to hsv
    HSVTRIPLE newIm[height][width];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            newIm[i][j] = rgbHSV(image[i][j]);
        }
    }

    // sort each row in the image
    selectionSort(height, width, newIm);

    // declare a new linear array to store our "universally" sorted list
    HSVTRIPLE finIm[height * width];

    // declare and initialize an array of indexes for each row
    int index[height];
    for (int i = 0; i < height; i++) {
        index[i] = 0;
    }

    // fill in our linear array with each minimum we find after comparing the earliest remaining index from each row
    for (int i = 0; i < height * width; i++) {
        // make the comparison
        int k = findMinIndex(height, width, newIm, index);

        // load the minimum
        finIm[i] = newIm[k][index[k]];

        // update the index for that row
        index[k]++;
    }

    // convert image from hsv to rgb
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = hsvRGB(finIm[i * width + j]);
        }
    }

    return;
}

// returns the (0 indexed) row number that contains the next minimum we will add to our sorted array
int findMinIndex(int height, int width, HSVTRIPLE image[height][width], int index[height]) {
    int min = image[0][index[0]].hue;
    int ret = 0;
    // TODO
    return ret;
}

/*  -------------------------------------------------------------------------------------------------------------------------------------
   COMPLETE HELPER FUNCTIONS -- MAKE NO CHANGES BELOW
    -------------------------------------------------------------------------------------------------------------------------------------  */

// Selection Sort
void selectionSort(int height, int width, HSVTRIPLE image[height][width]) {

    // selection sort each row by hue
    // walk through rows
    for (int i = 0; i < height; i++) {
        // for each j index, find the minimum in range [j,width) and swap with j
        for (int j = 0; j < width; j++) {
            int index = findMin(j, width, image[i]);
            if (index > j) {
                HSVTRIPLE tmp = image[i][j];
                image[i][j] = image[i][index];
                image[i][index] = tmp;
            }
        }
    }

    return;
}

int findMin(int start, int width, HSVTRIPLE image[width]) {
    int min = image[start].hue;
    int index = start;
    for (int i = start; i < width; i++) {
        if (image[i].hue < min) {
            min = image[i].hue;
            index = i;
        }
    }
    return index;
}

HSVTRIPLE rgbHSV(RGBTRIPLE px) {
    HSVTRIPLE newPx;
    double cMax = max(max(px.rgbtRed, px.rgbtGreen), px.rgbtBlue);
    double cMin = min(min(px.rgbtRed, px.rgbtGreen), px.rgbtBlue);
    newPx.hue = getHue(px, cMax, cMin);

    // regularize saturation and value
    newPx.sat = 0.85;
    newPx.val = 1;
    return newPx;
}

// generic max / min functions
int max(int x, int y) {
    if(x > y) {
        return x;
    }
    return y;
}

int min(int x, int y) {
    if(x < y) {
        return x;
    }
    return y;
}

// helper function to convert rgb to hsv
double getHue(RGBTRIPLE px, double cMax, double cMin) {
    double dx = cMax - cMin;
    if (dx == 0) return 0.0;
    if (cMax == px.rgbtRed) return (int) (60.0 * ((px.rgbtGreen - px.rgbtBlue) / dx) + 360) % 360;
    if (cMax == px.rgbtGreen) return (int) (60.0 * ((px.rgbtBlue - px.rgbtRed) / dx) + 120) % 360;
    if (cMax == px.rgbtBlue) return (int) (60.0 * ((px.rgbtRed - px.rgbtGreen) / dx) + 240) % 360;
    return 0.0;
}

// helper function to convert rgb to hsv
double getSat(RGBTRIPLE px, double cMax, double cMin) {
    if (cMax < 0.0001) {
        return 0.0;
    }
    return (cMax - cMin) / cMax;
}

RGBTRIPLE hsvRGB(HSVTRIPLE px) {
    RGBTRIPLE newPx;
    double c = px.val * px.sat;
    double decimal = px.hue / 60 - (int) (px.hue / 60);
    double x = c * (1 - fabs(( ((int) px.hue / 60) % 2) + decimal - 1));
    double m = px.val - c;

    double red = c, green = x, blue = 0;

    if (px.hue < 60) {
        // do nothing, already initialized!
    } else if (px.hue < 120) {
        red = x; green = c; blue = 0;
    } else if (px.hue < 180) {
        red = 0; green = c; blue = x;
    } else if (px.hue < 240) {
        red = 0; green = x; blue = c;
    } else if (px.hue < 300) {
        red = x; green = 0; blue = c;
    } else if (px.hue < 360) {
        red = c; green = 0; blue = x;
    }

    newPx.rgbtRed = (red + m) * 255;
    newPx.rgbtGreen = (green + m) * 255;
    newPx.rgbtBlue = (blue + m) * 255;
    return newPx;
}
