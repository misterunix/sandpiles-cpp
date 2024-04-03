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

struct box
{
    int w;
    int h;
};

// class to hold a grid of integers
class cGrid
{
  public:
    int width;  // width of the grid
    int height; // height of the grid
    int *grid;  // the grid

    int bMinX; // bounding box min x
    int bMinY; // bounding box min y
    int bMaxX; // bounding box max x
    int bMaxY; // bounding box max y

    // constructor
    cGrid(int w, int h)
    {
        width = w;
        height = h;
        bMinX = 0;
        bMinY = 0;
        bMaxX = width;
        bMaxY = height;

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
    inline int xy2i(int x, int y)
    {
        return y * width + x;
    }

    // set a value in the grid
    inline void set(int x, int y, int v)
    {
        if (x < 0 || x > width - 1 || y < 0 || y > height - 1)
            return;
        grid[xy2i(x, y)] = v;
    }

    // get a value from the grid
    inline int get(int x, int y)
    {
        if (x < 0 || x > width - 1 || y < 0 || y > height - 1)
            return 0;
        return grid[xy2i(x, y)];
    }

    inline void resize(int x, int y)
    {

        int diffX = x - width;
        int diffY = y - height;

        int *newgrid = new int[x * y];
        for (int i = 0; i < x * y; i++)
        {
            newgrid[i] = 0;
        }
        for (int i = 0; i < width; i++)
        {
            int ii = i + diffX / 2;
            for (int j = 0; j < height; j++)
            {
                int jj = j + diffY / 2;
                newgrid[jj * x + ii] = grid[j * width + i];
            }
        }
        delete[] grid;
        grid = newgrid;
        width = x;
        height = y;
    }

    // get the new width
    int getWidth()
    {
        return width;
    }

    // get the new height
    int getHeight()
    {
        return height;
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
    string imgFileName;

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
    inline void setPixel(int x, int y, int cc)
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
        imgFileName = fn;

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

    // get the image file name
    string getImgFileName()
    {
        return imgFileName;
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

inline box run(int width, int height, int initialGrains, int imgnum)
{
    // cout << "run " << width << " " << height << " " << initialGrains << " " << imgnum << endl << flush;

    int border = 10;
    // int bodergap = 50;
    uint8_t twirly = 0;

    cGrid grid{width, height}; // grid of grains
    // Image img{width, height};  // image of grains

    int mx = width / 2;
    int my = height / 2;

    grid.set(mx, my, initialGrains);

    bool changed = true;
    int mw = width;
    int mh = height;
    bool sizechanged = false;
    while (changed)
    {
        // cout << "." << flush;

        if (sizechanged)
        {
            width = mw;
            height = mh;
            sizechanged = false;
        }

        // cout << ".";
        changed = false;

        int oldmw = mw;
        int oldmh = mh;

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int v = grid.get(x, y);
                if (v > 3)
                {
                    // cout << ".";
                    grid.set(x, y, v - 4);

                    int bgw = 50;
                    int bgh = 50;

                    /*




                    */

                    int tx1a = x + 1;
                    int tx1b = grid.get(tx1a, y) + 1;
                    if (tx1a >= width - border)
                    {
                        // width = tx1a + bodergap;
                        // width = tx1a + (width+(width/2));

                        sizechanged = true;
                    }
                    grid.set(tx1a, y, tx1b);

                    tx1a = x - 1;
                    tx1b = grid.get(tx1a, y) + 1;
                    if (tx1a < border)
                    {

                        sizechanged = true;
                    }
                    grid.set(x - 1, y, tx1b);

                    int ty1a = y + 1;
                    int ty1b = grid.get(x, ty1a) + 1;
                    if (ty1a >= height - border)
                    {
                        // height = ty1a + bodergap;

                        sizechanged = true;
                    }
                    grid.set(x, y + 1, ty1b);

                    ty1a = y - 1;
                    ty1b = grid.get(x, ty1a) + 1;
                    if (ty1a < border)
                    {

                        sizechanged = true;
                    }
                    grid.set(x, y - 1, ty1b);

                    /*




                    */

                    // if the size has changed, resize the grid
                    if (sizechanged)
                    {
                        mw = width + bgw;
                        mh = height + bgh;
                        if (oldmw == mw && oldmh == mh)
                        {
                            continue;
                        }
                        oldmw = mw;
                        oldmh = mh;

                        // sizechanged = false;

                        switch (twirly)
                        {
                        case 0:
                            cout << "|\t" << flush;
                            twirly++;
                            break;
                        case 1:
                            cout << "/\t" << flush;
                            twirly++;
                            break;
                        case 2:
                            cout << "-\t" << flush;
                            twirly++;
                            break;
                        case 3:
                            cout << "\\\t" << flush;
                            twirly = 0;
                            break;
                        }

                        cout << "Resizing " << width << "x" << height << " to " << mw << "x" << mh << "\r" << flush;

                        grid.resize(mw, mh);
                    }

                    changed = true;
                }
            }
        }
    }

    box b = {width, height};

    Image img{width, height}; // image of grains
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            img.setPixel(x, y, grid.get(x, y));
        }
    }
    img.saveImage(imgnum, initialGrains);
    return b;
}

inline string to_string_leading(int i)
{
    string t = to_string(i); // convert to string
    if (t.length() == 1)     // if only one digit
    {
        t = "0" + t; // add a leading zero
    }
    return t;
}

inline string ConvertSectoDay(int n)
{
    string result;

    int day = n / (24 * 3600); // get days from seconds
    n = n % (24 * 3600);
    string d = to_string(day); // days

    int hour = n / 3600;                // get hours from remaining seconds
    string h = to_string_leading(hour); // hours

    n %= 3600;
    int minutes = n / 60;                  // get minutes from remaining seconds
    string m = to_string_leading(minutes); // minutes

    n %= 60;
    int seconds = n;                       // get remaining seconds
    string s = to_string_leading(seconds); // seconds

    result = d + ":" + h + ":" + m + ":" + s;

    return result;
}

// convert an integer to a string with a leading zero if needed

int main(int argc, char **argv)
{

    int width = 50;
    int height = 50;
    box b = {width, height};

    // run(width, height, 64000, 0);

    for (int i = 8; i < 32; i++)
    {
        // cout << "Starting " << i << " " << flush;

        time_t start = time(NULL);
        int ig = 1 << i;

        b = run(width, height, ig, i);
        width = b.w;
        height = b.h;

        time_t end = time(NULL);
        float et = difftime(end, start);

        // cout << "Time: " << end - start << " seconds"
        //      << " for " << ig << endl
        //      << flush;

        string fn;
        fn = "relative_test.md";
        ofstream ofile(fn, ios::app);
        if (!ofile)
        {
            cerr << "Cannot open file " << fn << endl;
            return 1;
        }

        ofile << "|" << i << "|" << ig << "|" << ConvertSectoDay(int(et)) << "|" << width << "x" << height << "|  "
              << endl;

        ofile.close();
    }
    cout << endl;
}
