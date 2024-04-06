#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

class cOptions
{
  public:
    int m_width;  // width of the grid
    int m_height; // height of the grid
    int m_bits;   // bits per pixel
    std::map<std::string, std::string> m_options;

    cOptions();
    ~cOptions();
};