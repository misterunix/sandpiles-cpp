#include "cOptions.hh"

/**
 * @brief Construct a new cOptions::cOptions object
 *
 */
cOptions::cOptions()
{
    m_options["w"] = 64;
    m_options["h"] = 64;
    m_options["b"] = 8;
    m_options["p0"] = 32, 32;
}

/**
 * @brief Destroy the cOptions::cOptions object
 *
 */
cOptions::~cOptions()
{
}
