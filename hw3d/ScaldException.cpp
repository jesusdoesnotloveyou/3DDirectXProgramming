#include "ScaldException.h"
#include <sstream>

ScaldException::ScaldException(int line, const char* file) noexcept
    :
    line(line),
    file(std::string(file)) {}

const char* ScaldException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << "\n"
        << GetOriginString();
    whatBuffer = oss.str();
    // to prevent pointing to the dead memory after oss dying, and for saving exception info
    return whatBuffer.c_str();
}

const char* ScaldException::GetType() const noexcept
{
    return "Scald Exception";
}

int ScaldException::GetLine() const noexcept
{
    return line;
}

const std::string& ScaldException::GetFile() const noexcept
{
    return file;
}

std::string ScaldException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << file << "\n"
        << "[Line] " << line;
    return oss.str();
}