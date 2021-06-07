#pragma once
#include <boost/noncopyable.hpp>
#include <string>
#include <fstream> 
#include <iostream> 
#include "comm/format.h"
#include "comm/string_util.h"
#include <map>


namespace frpc
{
class Config : boost::noncopyable
{
public:
    using Section = std::string;
    using Item = std::pair<std::string, std::string>;

    Config(const std::string& filepath): conf_filepath_(filepath)
    {
        
    };

    // static Config& GetConfig() //单例模式
    // {
    //     static Config config_;
    //     return config_;
    // }
private:
    std::string conf_filepath_;
    std::map<Section, Item*> conf_data_;
};

} //namespace frpc