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

#define _VERSION "0.0.1"

class cArgs
{

    int m_argc;
    char **m_argv;
    std::map<std::string, std::string> m_args;

  public:
    int m_width;
    int m_height;
    int m_bits;
    cArgs(int t_argc, char **t_atgv);
    void ParseFlags(void);
};