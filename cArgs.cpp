#include "cArgs.hh"

/**
 * @brief Construct a new cArgs::cArgs object
 *
 * @param t_argc argc from main
 * @param t_atgv argv from main
 */
cArgs::cArgs(int t_argc, char **t_atgv)
{

    m_argc = t_argc;
    m_argv = t_atgv;
    m_width = 64;
    m_height = 64;
    m_bits = 8;
}

/**
 * @brief Parse argv to get flags and values
 *
 */
void cArgs::ParseFlags(void)
{
    for (int i = 0; i < m_argc; i++)
    {
        std::string arg = m_argv[i];
        if (arg == "--")
        {
            break;
        }

        if (arg == "--help")
        {
            std::cout << "Help" << std::endl;
            std::cout << " -v, --version" << std::endl;
            std::cout << " --help" << std::endl;
            std::cout << " -w, --width" << std::endl;
            std::cout << " -h, --height" << std::endl;
            std::cout << " -b, --bits" << std::endl;
            exit(0);
        }

        if (arg == "-v" || arg == "--version")
        {
            std::cout << "Version " << _VERSION << std::endl;
            exit(0);
        }

        if (arg == "-w" || arg == "--width")
        {
            m_args["w"] = m_argv[i + 1];
            i++;
            m_width = std::stoi(m_args["w"]);
            continue;
        }

        if (arg == "-h" || arg == "--height")
        {
            m_args["h"] = m_argv[i + 1];
            i++;
            m_height = std::stoi(m_args["h"]);
            continue;
        }

        if (arg == "-b" || arg == "--bits")
        {
            m_args["b"] = m_argv[i + 1];
            i++;
            m_bits = std::stoi(m_args["b"]);
            continue;
        }
    }

    // std::cout << "Arguments:" << std::endl;
    // std::cout << "argv[0]: " << m_argv[0] << std::endl;
    // for (auto it = m_args.begin(); it != m_args.end(); ++it)
    // {
    //     std::cout << it->first << ", " << it->second << '\n';
    // }
}