#include "helpers.h"
#include <math.h>

// Copy an image's pixels to another image
// Check50 is too stupid for me to include this in helpers.h
void cpImg(int height, int width, RGBTRIPLE dest[height][width], RGBTRIPLE src[height][width]);


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE avg = round((image[i][j].rgbtRed+image[i][j].rgbtBlue+image[i][j].rgbtGreen)/3);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE orig = image[i][j];
            BYTE sepiaRed = round(fmin(.393 * orig.rgbtRed + .769 * orig.rgbtGreen + .189 * orig.rgbtBlue, 255));
            BYTE sepiaGreen = round(fmin(.349 * orig.rgbtRed + .686 * orig.rgbtGreen + .168 * orig.rgbtBlue, 255));
            BYTE sepiaBlue = round(fmin(.272 * orig.rgbtRed + .534 * orig.rgbtGreen + .131 * orig.rgbtBlue, 255));
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtBlue = sepiaGreen;
            image[i][j].rgbtGreen = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            new[i][width-j-1] = image[i][j];
        }
    }
    cpImg(height, width, image, new);
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int blueSum = image[i][j].rgbtBlue;
            int greenSum = image[i][j].rgbtGreen;
            int redSum = image[i][j].rgbtRed;
            float amt = 1;
            if (i > 0)
            {
                blueSum += image[i-1][j].rgbtBlue;
                greenSum += image[i-1][j].rgbtGreen;
                redSum += image[i-1][j].rgbtRed;
                amt++;
            }
            if (i < height-1)
            {
                blueSum += image[i+1][j].rgbtBlue;
                greenSum += image[i+1][j].rgbtGreen;
                redSum += image[i+1][j].rgbtRed;
                amt++;
            }
            if (j > 0)
            {
                blueSum += image[i][j-1].rgbtBlue;
                greenSum += image[i][j-1].rgbtGreen;
                redSum += image[i][j-1].rgbtRed;
                amt++;
            }
            if (j < width-1)
            {
                blueSum += image[i][j+1].rgbtBlue;
                greenSum += image[i][j+1].rgbtGreen;
                redSum += image[i][j+1].rgbtRed;
                amt++;
            }
            if (i > 0 && j > 0)
            {
                blueSum += image[i-1][j-1].rgbtBlue;
                greenSum += image[i-1][j-1].rgbtGreen;
                redSum += image[i-1][j-1].rgbtRed;
                amt++;
            }
            if (i < height-1 && j < width-1)
            {
                blueSum += image[i+1][j+1].rgbtBlue;
                greenSum += image[i+1][j+1].rgbtGreen;
                redSum += image[i+1][j+1].rgbtRed;
                amt++;
            }
            if (i > 0 && j < width-1)
            {
                blueSum += image[i-1][j+1].rgbtBlue;
                greenSum += image[i-1][j+1].rgbtGreen;
                redSum += image[i-1][j+1].rgbtRed;
                amt++;
            }
            if (i < height-1 && j > 0)
            {
                blueSum += image[i+1][j-1].rgbtBlue;
                greenSum += image[i+1][j-1].rgbtGreen;
                redSum += image[i+1][j-1].rgbtRed;
                amt++;
            }
            new[i][j].rgbtBlue = round(blueSum/amt);
            new[i][j].rgbtGreen = round(greenSum/amt);
            new[i][j].rgbtRed = round(redSum/amt);
        }
    }
    cpImg(height, width, image, new);
    return;
}

void cpImg(int height, int width, RGBTRIPLE dest[height][width], RGBTRIPLE src[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            dest[i][j] = src[i][j];
        }
    }
}

