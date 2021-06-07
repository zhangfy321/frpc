#pragma once
#ifndef FRPC_COMM_STRING_UTIL_H
#define FRPC_COMM_STRING_UTIL_H
#include <string>
#include <vector>
namespace comm
{

void StrTrim(std::string* str);
bool StrStartsWith(const std::string& str, const std::string& prefix);
void StrSplit(const std::string& s, std::vector<std::string>* tokens, const std::string& delimiters);

} //namespace comm

#endif //FRPC_COMM_STRING_UTIL_H