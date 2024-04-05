#pragma once

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

/**
 * @brief class to hold and modify the image
 *
 */
class cImage
{
  private:
    gdImagePtr m_img; // the image
    int m_colors[5];  // the colors

    void initColors();

  public:
    int m_width;  // width of the image
    int m_height; // height of the image
    std::string m_imgFileName;

    cImage();
    cImage(int t_w, int t_h);
    ~cImage();
    void setWidthHeight(int t_w, int t_h);

    bool saveImage(int t_i, int t_grains);
    std::string getImgFileName();

    /**
     * @brief Set the Pixel at x,y to color t_cc
     *
     * @param t_x
     * @param t_y
     * @param t_cc value 0-4
     */
    inline void setPixel(int t_x, int t_y, int t_cc)
    {
        // check for out of bounds
        if (t_x < 0 || t_y < 0 || t_x > m_width - 1 || t_y > m_height - 1)
            return;

        // check for out of bounds color
        if (t_cc < 0)
            t_cc = 0;
        if (t_cc > 4)
            t_cc = 4;

        // check for no image
        if (m_img == NULL)
        {
            std::cout << "setPixel, img==NULL" << std::endl;
            return;
        }

        // set the pixel
        gdImageSetPixel(m_img, t_x, t_y, m_colors[t_cc]);
    }
};