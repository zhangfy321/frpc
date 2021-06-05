#pragma once
#include <string>
#include <vector>
namespace frpc
{
namespace util
{

void StrTrim(std::string* str);
bool StrStartsWith(const std::string& str, const std::string& prefix)
void StrSplit(const std::string& s, std::vector<string>* tokens, const string& delimiters)

} //util
} //namespace frpc