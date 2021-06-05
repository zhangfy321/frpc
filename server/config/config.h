#pragma once
#include <boost/noncopyable.hpp>
#include <string>
#include <fstream> 
#include <iostream> 
#include "comm/format.h"
#include "comm/string_util.h"


namespace frpc
{
class Config : boost::noncopyable
{
public:
    using Section = std::string;
    using Item = std::pair<std::string, std::string>;

    Config(const std::string& filepath): _conf_filepath(filepath)
    {
        ifstream conf_stream(_conf_filepath, ios::in);
        if(!conf_stream.is_open ())
        {
            ERROR("open config file err");
            exit(-1);
        }
            

        std::string line;
        while (!conf_stream.eof())
        {
            conf_stream >> line;
            util::StrTrim(&line);
            if (util::StrStartsWith("[")) // section name
            {
               
            }            
        }
    }

    Config& GetConfig() //单例模式
    {
        static Config("default.conf") config;
        return config;
    }
private:
    std::string conf_filepath_;
    std::map<Section, Item*> conf_data_;
}

} //namespace frpc