#ifndef CONFIG_H
#define CONFIG_H
#include <string>
#include "INIReader.h"

using std::string;
namespace frpc{
    class Config{
    public:
        Config(const string & config_file_path):config_file_path_(config_file_path),reader_(config_file_path){};
        long GetInteger(const string & section, const string & name, const int & defaultVal);
        string GetString(const string & section, const string & name, const string & defaultVal);
        bool GetBoolean(const string & section, const string & name, const bool & defaultVal);
        double GetDouble(const string & section, const string & name, const double & defaultVal);
        ~Config() = default;
    private:
        const string config_file_path_;//config文件的存储位置
        INIReader reader_;
    };
}//end namespace frpc

#endif