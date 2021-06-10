#include "config.h"

#include "../../comm/format.h"

long frpc::Config::GetInteger(const std::string &section, const std::string &name, const int &defaultVal) {
    return reader_.GetInteger(section, name, defaultVal);
}

std::string
frpc::Config::GetString(const std::string &section, const std::string &name, const std::string &defaultVal) {
    return reader_.GetString(section, name, defaultVal);
}

bool frpc::Config::GetBoolean(const std::string &section, const std::string &name, const bool &defaultVal) {
    return reader_.GetBoolean(section, name, defaultVal);
}

double frpc::Config::GetDouble(const std::string &section, const std::string &name, const double &defaultVal) {
    return reader_.GetReal(section, name, defaultVal);
}

