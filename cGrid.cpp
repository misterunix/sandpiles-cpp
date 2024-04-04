#include "cGrid.hh"

// constructor
cGrid::cGrid(int t_w, int t_h)
{
    m_width = t_w;
    m_height = t_h;
    m_bMinX = 0;
    m_bMinY = 0;
    m_bMaxX = m_width;
    m_bMaxY = m_height;

    int size = m_width * m_height;
    m_grid = new int[size];

    // zero out the grid
    for (int i = 0; i < size; i++)
    {
        m_grid[i] = 0;
    }
}

// destructor
cGrid::~cGrid()
{
    delete[] m_grid;
}

// get the new width
int cGrid::getWidth()
{
    return m_width;
}

// get the new height
int cGrid::getHeight()
{
    return m_height;
}
