#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cImage.hh"

/**
 * @brief class to hold and modify the grid
 *
 */
class cGrid
{
  public:
    int m_width;  // width of the grid
    int m_height; // height of the grid
    int m_bits;   // bits shifted for the grid. Only used for save point.
    int *m_grid;  // the grid

    int m_bMinX; // bounding box min x
    int m_bMinY; // bounding box min y
    int m_bMaxX; // bounding box max x
    int m_bMaxY; // bounding box max y

    cGrid(int t_w, int t_h, int t_bits);
    ~cGrid();

    /**
     * @brief convert x,y to index
     *
     * @param t_x
     * @param t_y
     * @return int
     */
    inline int xy2i(int t_x, int t_y)
    {
        return t_y * m_width + t_x;
    }

    /**
     * @brief set the value v at x,y
     *
     * @param t_x
     * @param t_y
     * @param t_v
     */
    inline void set(int t_x, int t_y, int t_v)
    {
        if (t_x < 0 || t_x > m_width - 1 || t_y < 0 || t_y > m_height - 1)
            return;
        m_grid[xy2i(t_x, t_y)] = t_v;
    }

    /**
     * @brief get the value at x,y
     *
     * @param t_x
     * @param t_y
     * @return int
     */
    inline int get(int t_x, int t_y)
    {
        if (t_x < 0 || t_x > m_width - 1 || t_y < 0 || t_y > m_height - 1)
            return 0;
        return m_grid[xy2i(t_x, t_y)];
    }

    /**
     * @brief resize the current grid
     *
     * @param t_x new width
     * @param t_y new height
     */
    inline void resize(int t_x, int t_y)
    {

        int diffX = t_x - m_width;
        int diffY = t_y - m_height;

        int *newgrid = new int[t_x * t_y];
        for (int i = 0; i < t_x * t_y; i++)
        {
            newgrid[i] = 0;
        }
        for (int i = 0; i < m_width; i++)
        {
            int ii = i + diffX / 2;
            for (int j = 0; j < m_height; j++)
            {
                int jj = j + diffY / 2;
                newgrid[jj * t_x + ii] = m_grid[j * m_width + i];
            }
        }
        delete[] m_grid;
        m_grid = newgrid;
        m_width = t_x;
        m_height = t_y;
    }

    int getWidth();
    int getHeight();

    /**
     * @brief Save the grid to a file to be able to save the current state
     *
     */
    void SaveGrid(void);
};
