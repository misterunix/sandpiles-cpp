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

class cGrid
{
  public:
    int width;
    int height;
    unsigned int *grid;

    cGrid(int w, int h)
    {
        width = w;
        height = h;
        int size = width * height;
        grid = new unsigned int[size];
    }

    ~cGrid()
    {
        delete[] grid;
    }

    int xy2i(int x, int y)
    {
        return y * width + x;
    }

    void set(int x, int y, unsigned int v)
    {
        if (x < 0 || x > width - 1 || y < 0 || y > height - 1)
            return;
        grid[xy2i(x, y)] = v;
    }

    unsigned int get(int x, int y)
    {
        if (x < 0 || x > width - 1 || y < 0 || y > height - 1)
            return 0;
        return grid[xy2i(x, y)];
    }
};

class Image
{
  private:
    gdImagePtr img;
    int color[5];

  public:
    int width;
    int height;
    // constructors
    Image()
    {
        img = NULL;
        width = 0;
        height = 0;
    }

    void setWidthHeight(int w, int h)
    {
        Image(w, h);
    }

    Image(int w, int h)
    {
        img = gdImageCreate(w, h);
        if (img == NULL)
        {
            cout << "Cannot create image" << endl;
            return;
        }
        initColors();
        width = w;
        height = h;
        gdImageFilledRectangle(img, 0, 0, width, height, colors[0]);
    }

    // destructor
    ~Image()
    {
        gdImageDestroy(img);
    }

    // set a pixel to a color
    void setPixel(int x, int y, int cc)
    {
        if (img == NULL)
        {
            cout << "setPixel, img==NULL" << endl;
            return;
        }
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

void run(int width, int height, int initialGrains)
{
    int width = 1000;
    int height = 1000;
    cGrid grid{width, height};
    Image img{width, height};

    int mx = width / 2;
    int my = height / 2;
    int initialGrains = 1000000;
    grid.set(mx, my, initialGrains);

    bool changed = true;

    while (changed)
    {
        changed = false;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                unsigned int v = grid.get(x, y);
                if (v >= 4)
                {
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
    img.saveImage(1, initialGrains);
}

int main(int argc, char **argv)
{

    int width = 1000;
    int height = 1000;
    cGrid grid{width, height};
    Image img{width, height};

    int mx = width / 2;
    int my = height / 2;
    int initialGrains = 1000000;
    grid.set(mx, my, initialGrains);

    bool changed = true;

    while (changed)
    {
        changed = false;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                unsigned int v = grid.get(x, y);
                if (v >= 4)
                {
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
    img.saveImage(1, initialGrains);
}