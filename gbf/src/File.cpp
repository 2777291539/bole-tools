#include "File.h"

Dcr::File::File(std::string path, bool hasLog) : m_path(path), m_hasLog(hasLog)
{
    if (!std::filesystem::exists(path))
    {
        std::cerr << "Invalid file path: " + path << std::endl;
        exit(0);
    }
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
