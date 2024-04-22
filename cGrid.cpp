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
    file.open("grid.sav", std::ios::app);
    if (!file)
    {
        std::cerr << "Cannot open file grid.sav" << std::endl;
        return;
    }
    file << m_width << "\n";
    file << m_height << "\n";
    file << m_bits << "\n";

    for (int i = 0; i < m_width * m_height; i++)
    {
        file << m_grid[i] << " ";
    }
    file << "\n";
    file.close();
}

/**
 * @brief Load the grid from a file to be able to restore the current state
 *
 */
void cGrid::LoadGrid(void)
{
    std::fstream file;
    file.open("grid.sav", std::ios::in);
    if (!file)
    {
        std::cerr << "Cannot open file grid.sav" << std::endl;
        return;
    }
    file >> m_width;
    file >> m_height;
    file >> m_bits;

    delete[] m_grid;
    m_grid = new int[m_width * m_height];

    for (int i = 0; i < m_width * m_height; i++)
    {
        file >> m_grid[i];
    }
    file.close();
}