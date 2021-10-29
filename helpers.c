#include "helpers.h"
#include <math.h>

// Copy an image's pixels to another image
// Check50 is too stupid for me to include this in helpers.h
void cpImg(int height, int width, RGBTRIPLE dest[height][width], RGBTRIPLE src[height][width]);

// Again, check50 won't use the updated header file, so it won't compile without this
void edge(int height, int width, RGBTRIPLE image[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE avg = round((image[i][j].rgbtRed+image[i][j].rgbtBlue+image[i][j].rgbtGreen)/3.0);
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
            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
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
            new[i][width-j - 1] = image[i][j];
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
                blueSum += image[i - 1][j].rgbtBlue;
                greenSum += image[i - 1][j].rgbtGreen;
                redSum += image[i - 1][j].rgbtRed;
                amt++;
            }
            if (i < height - 1)
            {
                blueSum += image[i + 1][j].rgbtBlue;
                greenSum += image[i + 1][j].rgbtGreen;
                redSum += image[i + 1][j].rgbtRed;
                amt++;
            }
            if (j > 0)
            {
                blueSum += image[i][j - 1].rgbtBlue;
                greenSum += image[i][j - 1].rgbtGreen;
                redSum += image[i][j - 1].rgbtRed;
                amt++;
            }
            if (j < width - 1)
            {
                blueSum += image[i][j + 1].rgbtBlue;
                greenSum += image[i][j + 1].rgbtGreen;
                redSum += image[i][j + 1].rgbtRed;
                amt++;
            }
            if (i > 0 && j > 0)
            {
                blueSum += image[i - 1][j - 1].rgbtBlue;
                greenSum += image[i - 1][j - 1].rgbtGreen;
                redSum += image[i - 1][j - 1].rgbtRed;
                amt++;
            }
            if (i < height - 1 && j < width - 1)
            {
                blueSum += image[i + 1][j + 1].rgbtBlue;
                greenSum += image[i + 1][j + 1].rgbtGreen;
                redSum += image[i + 1][j + 1].rgbtRed;
                amt++;
            }
            if (i > 0 && j < width - 1)
            {
                blueSum += image[i - 1][j + 1].rgbtBlue;
                greenSum += image[i - 1][j + 1].rgbtGreen;
                redSum += image[i - 1][j + 1].rgbtRed;
                amt++;
            }
            if (i < height - 1 && j > 0)
            {
                blueSum += image[i + 1][j - 1].rgbtBlue;
                greenSum += image[i + 1][j - 1].rgbtGreen;
                redSum += image[i + 1][j - 1].rgbtRed;
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


// Detect edges in image
void edge(int height, int width, RGBTRIPLE image[height][width])
{
    const int xKern[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    const int yKern[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    RGBTRIPLE new[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int xBlueSum = 0;
            int xGreenSum = 0;
            int xRedSum = 0;
            int yBlueSum = 0;
            int yGreenSum = 0;
            int yRedSum = 0;
            if (i > 0)
            {
                yBlueSum += image[i - 1][j].rgbtBlue * yKern[1];
                yGreenSum += image[i - 1][j].rgbtGreen * yKern[1];
                yRedSum += image[i - 1][j].rgbtRed * yKern[1];
            }
            if (i < height - 1)
            {
                yBlueSum += image[i + 1][j].rgbtBlue * yKern[7];
                yGreenSum += image[i + 1][j].rgbtGreen * yKern[7];
                yRedSum += image[i + 1][j].rgbtRed * yKern[7];
            }
            if (j > 0)
            {
                xBlueSum += image[i][j - 1].rgbtBlue * xKern[3];
                xGreenSum += image[i][j - 1].rgbtGreen * xKern[3];
                xRedSum += image[i][j - 1].rgbtRed * xKern[3];
            }
            if (j < width - 1)
            {
                xBlueSum += image[i][j + 1].rgbtBlue * xKern[5];
                xGreenSum += image[i][j + 1].rgbtGreen * xKern[5];
                xRedSum += image[i][j + 1].rgbtRed * xKern[5];
            }
            if (i > 0 && j > 0)
            {
                xBlueSum += image[i - 1][j - 1].rgbtBlue * xKern[0];
                xGreenSum += image[i - 1][j - 1].rgbtGreen * xKern[0];
                xRedSum += image[i - 1][j - 1].rgbtRed * xKern[0];
                yBlueSum += image[i - 1][j - 1].rgbtBlue * yKern[0];
                yGreenSum += image[i - 1][j - 1].rgbtGreen * yKern[0];
                yRedSum += image[i - 1][j - 1].rgbtRed * yKern[0];
            }
            if (i < height - 1 && j < width - 1)
            {
                xBlueSum += image[i + 1][j + 1].rgbtBlue * xKern[8];
                xGreenSum += image[i + 1][j + 1].rgbtGreen * xKern[8];
                xRedSum += image[i + 1][j + 1].rgbtRed * xKern[8];
                yBlueSum += image[i + 1][j + 1].rgbtBlue * yKern[8];
                yGreenSum += image[i + 1][j + 1].rgbtGreen * yKern[8];
                yRedSum += image[i + 1][j + 1].rgbtRed * yKern[8];
            }
            if (i > 0 && j < width - 1)
            {
                xBlueSum += image[i - 1][j + 1].rgbtBlue * xKern[2];
                xGreenSum += image[i - 1][j + 1].rgbtGreen * xKern[2];
                xRedSum += image[i - 1][j + 1].rgbtRed * xKern[2];
                yBlueSum += image[i - 1][j + 1].rgbtBlue * yKern[2];
                yGreenSum += image[i - 1][j + 1].rgbtGreen * yKern[2];
                yRedSum += image[i - 1][j + 1].rgbtRed * yKern[2];
            }
            if (i < height - 1 && j > 0)
            {
                xBlueSum += image[i + 1][j - 1].rgbtBlue * xKern[6];
                xGreenSum += image[i + 1][j - 1].rgbtGreen * xKern[6];
                xRedSum += image[i + 1][j - 1].rgbtRed * xKern[6];
                yBlueSum += image[i + 1][j - 1].rgbtBlue * yKern[6];
                yGreenSum += image[i + 1][j - 1].rgbtGreen * yKern[6];
                yRedSum += image[i + 1][j - 1].rgbtRed * yKern[6];
            }
            new[i][j].rgbtBlue = 255 - round(fmin(sqrt((xBlueSum * xBlueSum) + (yBlueSum * yBlueSum)), 255));
            new[i][j].rgbtGreen = 255 - round(fmin(sqrt((xGreenSum * xGreenSum) + (yGreenSum * yGreenSum)), 255));
            new[i][j].rgbtRed = 255 - round(fmin(sqrt((xRedSum * xRedSum) + (yRedSum * yRedSum)), 255));
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

