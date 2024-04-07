#include "cGrid.hh"

/**
 * @brief Construct a new cGrid::cGrid object
 *
 * @param t_w width of the grid
 * @param t_h height of the grid
 */
cGrid::cGrid(int t_w, int t_h, int t_bits)
{
    m_width = t_w;
    m_height = t_h;
    m_bits = t_bits;
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

/**
 * @brief Destroy the cGrid::cGrid object
 *
 */
cGrid::~cGrid()
{
    delete[] m_grid;
}

/**
 * @brief get the current width of the grid
 *
 * @return int
 */
int cGrid::getWidth()
{
    return m_width;
}

/**
 * @brief get the current height of the grid
 *
 * @return int
 */
int cGrid::getHeight()
{
    return m_height;
}

/**
 * @brief Save the grid to a file to be able to save the current state
 *
 */
void cGrid::SaveGrid(void)
{

    std::fstream file;
    file.open("grid.bin", std::ios::app | std::ios::binary);
    file.write(reinterpret_cast<char *>(&m_width), sizeof(m_width));
    file.write(reinterpret_cast<char *>(&m_height), sizeof(m_height));
    file.write(reinterpret_cast<char *>(&m_bits), sizeof(m_bits));
    file.write(reinterpret_cast<char *>(&m_grid), sizeof(m_grid));
    file.close();
}