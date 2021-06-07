//util
#include "string_util.h"
#include <string.h>

namespace comm
{
void StrTrim(std::string* str)
{
    size_t start_pos = 0;
    size_t end_pos = str->length();
    while (start_pos != end_pos && isspace(str->at(start_pos)))
        start_pos++;
    if (start_pos == end_pos)
    {
        str->clear();
        return;
    }
    end_pos--;
    while (isspace(str->at(end_pos))) // end_pos always >= 0
        end_pos--;
    *str = str->substr(start_pos, end_pos - start_pos + 1);
}

bool StrStartsWith(const std::string& str, const std::string& prefix)
{
    return str.size() >= prefix.size() && strncasecmp(str.c_str(), prefix.c_str(), prefix.size()) == 0;
}


void StrSplit(const std::string& s, std::vector<std::string>* tokens, const std::string& delimiters)
{
    std::string::size_type lastPos = s.find_first_not_of(delimiters, 0) ;
    std::string::size_type pos = s.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens->push_back(s.substr(lastPos, pos - lastPos)); //todo use emplace_back after C++11
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
}
}