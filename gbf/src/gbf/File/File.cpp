#include "File.h"

Dcr::File::File(std::string path, bool hasLog) : m_path(path), m_hasLog(hasLog)
{
    try
    {
        if (m_path.empty())
        {
            throw std::runtime_error("Empty file m_path");
        }
        if (*begin(m_path) == '\'' || *begin(m_path) == '\"')
        {
            m_path.erase(begin(m_path));
        }
        if (m_path.empty())
        {
            throw std::runtime_error("Empty file m_path");
        }
        if (*(end(m_path) - 1) == '\'' || *(end(m_path) - 1) == '\"')
        {
            m_path.erase(end(m_path) - 1);
        }
        if (m_path.empty())
        {
            throw std::runtime_error("Empty file m_path");
        }
        if (!std::filesystem::exists(m_path))
        {
            throw std::runtime_error("Invalid file m_path: \"" + m_path + "\"");
        }
    }
    catch (std::runtime_error err)
    {
        std::cerr << std::endl << err.what() << std::endl;
        exit(0);
    }
    // if (!std::filesystem::exists(path))
    // {
    //     std::cerr << "Invalid file path: " + path << std::endl;
    // }
    _SetTargetFileInfo();
}

Dcr::File::~File()
{
}

void Dcr::File::_SetTargetFileInfo()
{
    int pos = m_path.rfind("/");
    if (pos != std::string::npos)
    {
        m_targetFilePathHead = m_path.substr(0, pos + 1);
    }
}
