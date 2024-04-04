#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <gd.h>
#include <gdfontg.h>
#include <gdfontl.h>
#include <gdfontmb.h>
#include <gdfonts.h>
#include <gdfontt.h>
#include <gdfx.h>

#include "cGrid.hh"
#include "cImage.hh"

using namespace std;

struct box
{
    int w;
    int h;
};

inline box run(int t_width, int t_height, int t_initialGrains, int t_imgnum)
{
    // cout << "run " << width << " " << height << " " << initialGrains << " " << imgnum << endl << flush;

    int border = 10;
    // int bodergap = 50;
    uint8_t twirly = 0;

    cGrid grid{t_width, t_height}; // grid of grains
    // Image img{width, height};  // image of grains

    int mx = t_width / 2;
    int my = t_height / 2;

    grid.set(mx, my, t_initialGrains);

    bool changed = true;
    int mw = t_width;
    int mh = t_height;
    bool sizechanged = false;
    while (changed)
    {
        // cout << "." << flush;

        if (sizechanged)
        {
            t_width = mw;
            t_height = mh;
            sizechanged = false;
        }

        // cout << ".";
        changed = false;

        int oldmw = mw;
        int oldmh = mh;

        for (int y = 0; y < t_height; y++)
        {
            for (int x = 0; x < t_width; x++)
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
                    if (tx1a >= t_width - border)
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
                    if (ty1a >= t_height - border)
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
                        mw = t_width + bgw;
                        mh = t_height + bgh;
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

                        cout << "Resizing " << t_width << "x" << t_height << " to " << mw << "x" << mh << "\r" << flush;

                        grid.resize(mw, mh);
                    }

                    changed = true;
                }
            }
        }
    }

    box b = {t_width, t_height};

    cImage img{t_width, t_height}; // image of grains
    for (int y = 0; y < t_height; y++)
    {
        for (int x = 0; x < t_width; x++)
        {
            img.setPixel(x, y, grid.get(x, y));
        }
    }
    img.saveImage(t_imgnum, t_initialGrains);
    return b;
}

inline string to_string_leading(int t_i)
{
    string t = to_string(t_i); // convert to string
    if (t.length() == 1)       // if only one digit
    {
        t = "0" + t; // add a leading zero
    }
    return t;
}

inline string ConvertSectoDay(int t_n)
{
    string result;

    int day = t_n / (24 * 3600); // get days from seconds
    t_n = t_n % (24 * 3600);
    string d = to_string(day); // days

    int hour = t_n / 3600;              // get hours from remaining seconds
    string h = to_string_leading(hour); // hours

    t_n %= 3600;
    int minutes = t_n / 60;                // get minutes from remaining seconds
    string m = to_string_leading(minutes); // minutes

    t_n %= 60;
    int seconds = t_n;                     // get remaining seconds
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

    for (int i = 8; i < 16; i++)
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
        fn = "relative_test-test.md";
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
