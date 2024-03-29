#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>

#include <gd.h>
#include <gdfontg.h>
#include <gdfontl.h>
#include <gdfontmb.h>
#include <gdfonts.h>
#include <gdfontt.h>
#include <gdfx.h>

using namespace std;

// class to hold a grid of integers
class cGrid
{
  public:
    int width;  // width of the grid
    int height; // height of the grid
    int *grid;  // the grid

    // constructor
    cGrid(int w, int h)
    {
        width = w;
        height = h;
        int size = width * height;
        grid = new int[size];

        // zero out the grid
        for (int i = 0; i < size; i++)
        {
            grid[i] = 0;
        }
    }

    // destructor
    ~cGrid()
    {
        delete[] grid;
    }

    // convert x,y to an index
    int xy2i(int x, int y)
    {
        return y * width + x;
    }

    // set a value in the grid
    void set(int x, int y, int v)
    {
        if (x < 0 || x > width - 1 || y < 0 || y > height - 1)
            return;
        grid[xy2i(x, y)] = v;
    }

    // get a value from the grid
    int get(int x, int y)
    {
        if (x < 0 || x > width - 1 || y < 0 || y > height - 1)
            return 0;
        return grid[xy2i(x, y)];
    }
};

// class to hold an image
class Image
{
  private:
    gdImagePtr img; // the image
    int color[5];   // the colors

  public:
    int width;  // width of the image
    int height; // height of the image

    // constructor
    Image()
    {
        img = NULL; // no image
        width = 0;  // no width
        height = 0; // no height
    }

    // set the width and height of the image
    void setWidthHeight(int w, int h)
    {
        Image(w, h);
    }

    // constructor with width and height
    Image(int w, int h)
    {
        img = gdImageCreate(w, h); // create the image

        // check for no image
        if (img == NULL)
        {
            cout << "Cannot create image" << endl;
            return;
        }

        initColors();                                                // initialize the colors
        width = w;                                                   // set the width
        height = h;                                                  // set the height
        gdImageFilledRectangle(img, 0, 0, width, height, colors[0]); // fill the image with black
    }

    // destructor
    ~Image()
    {
        gdImageDestroy(img); // destroy the image
    }

    // set a pixel to a color
    void setPixel(int x, int y, int cc)
    {
        // check for out of bounds
        if (x < 0 || y < 0 || x > width - 1 || y > height - 1)
            return;

        // check for out of bounds color
        if (cc < 0)
            cc = 0;
        if (cc > 4)
            cc = 4;

        // check for no image
        if (img == NULL)
        {
            cout << "setPixel, img==NULL" << endl;
            return;
        }

        // set the pixel
        gdImageSetPixel(img, x, y, colors[cc]);
    }

    // save the image to a file
    bool saveImage(int i, int grains)
    {
        string fn = "img/sandpile-" + to_string(width) + "-" + to_string(height) + "-" + to_string(grains) + "-" +
                    to_string(i) + ".jpg";
        FILE *fp;
        fp = fopen(fn.c_str(), "wb");
        if (fp == NULL)
        {
            cout << "Cannot open file " << fn << endl;
            return false;
        }
        gdImageJpeg(img, fp, 100);
        fclose(fp);
        return true;
    }

  protected:
  private:
    int colors[5]; // black, green, purple, gold, white

    // initialize the colors
    void initColors()
    {
        if (img == NULL)
        {
            cout << "initColors, img==NULL" << endl;
            return;
        }
        colors[0] = gdImageColorAllocate(img, 0, 0, 0);          // black
        colors[1] = gdImageColorAllocate(img, 0, 0xff, 0);       // green
        colors[2] = gdImageColorAllocate(img, 0xff, 0, 0xff);    // purple
        colors[3] = gdImageColorAllocate(img, 0xff, 215, 0);     // gold
        colors[4] = gdImageColorAllocate(img, 0xff, 0xff, 0xff); // white
    }
};

void run(int width, int height, int initialGrains, int imgnum)
{
    cout << "run " << width << " " << height << " " << initialGrains << " " << imgnum << endl << flush;

    cGrid grid{width, height}; // grid of grains
    Image img{width, height};  // image of grains

    int mx = width / 2;
    int my = height / 2;

    grid.set(mx, my, initialGrains);

    bool changed = true;

    while (changed)
    {
        // cout << ".";
        changed = false;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int v = grid.get(x, y);
                if (v > 3)
                {
                    // cout << ".";
                    grid.set(x, y, v - 4);
                    grid.set(x + 1, y, grid.get(x + 1, y) + 1);
                    grid.set(x - 1, y, grid.get(x - 1, y) + 1);
                    grid.set(x, y + 1, grid.get(x, y + 1) + 1);
                    grid.set(x, y - 1, grid.get(x, y - 1) + 1);
                    changed = true;
                }
            }
        }
    }
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            img.setPixel(x, y, grid.get(x, y));
        }
    }
    img.saveImage(imgnum, initialGrains);
}

int main(int argc, char **argv)
{

    int width = 1024;
    int height = 1024;

    // run(width, height, 64000, 0);

    for (int i = 8; i < 32; i++)
    {
        cout << "Starting " << i << " " << flush;
        time_t start = time(NULL);
        int ig = 1 << i;
        run(width, height, ig, i);
        time_t end = time(NULL);
        cout << "Time: " << end - start << " seconds"
             << " for " << ig << endl
             << flush;
    }
}
