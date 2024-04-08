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

#include "cArgs.hh"
#include "cGrid.hh"
#include "cImage.hh"

using namespace std;

/**
 * @brief struct to hold multiple values for function run to return
 *
 */
struct box
{
    int w;
    int h;
};

/**
 * @brief create he grid and run the simulation, saving the image
 *
 * @param t_width width of the grid
 * @param t_height height of the grid
 * @param t_initialGrains number of grains to start with
 * @param t_imgnum image number to save
 * @return box
 */
inline box run(int t_width, int t_height, int t_initialGrains, int t_imgnum)
{
    // cout << "run " << width << " " << height << " " << initialGrains << " " << imgnum << endl << flush;

    int border = 16;
    // int bodergap = 50;
    uint8_t twirly = 0;

    cGrid grid{t_width, t_height, t_imgnum}; // grid of grains
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

                    int bgw = 64;
                    int bgh = 64;

                    ///
                    ///
                    ///

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

                    ///
                    ///
                    ///

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

/**
 * @brief convert an integer to a string with a leading zero if needed
 *
 * @param t_i integer to convert
 * @return string result
 */
inline string to_string_leading(int t_i)
{
    string t = to_string(t_i); // convert to string
    if (t.length() == 1)       // if only one digit
    {
        t = "0" + t; // add a leading zero
    }
    return t;
}

/**
 * @brief Convert seconds to days, hours, minutes, and seconds
 *
 * @param t_n number of seconds
 * @return string result
 */
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

/**
 * @brief main function
 *
 * @param argc
 * @param argv
 * @return int
 */

int main(int argc, char **argv)
{

    cArgs args{argc, argv};
    args.ParseFlags();

    int startBits = args.m_bits;
    int endBits = 32;

    int width = args.m_width;
    int height = args.m_height;

    box b = {width, height};

    cout << "Starting " << startBits << " bits" << endl << flush;
    cout << "Starting " << endBits << " bits" << endl << flush;
    cout << "Starting " << width << " width" << endl << flush;
    cout << "Starting " << height << " height" << endl << flush;

    for (int i = startBits; i <= endBits; i++)
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
