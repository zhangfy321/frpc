#include <iostream>
#include "config/config.h"

namespace frpc
{
class IOLoop {
public:
    IOLoop(Config* config): config_(config)
    {
        Init();
    };
    Run();
private:
    void Init();
    
    
    Config* config_;
    int epfd_;
    uint32_t max_conns_; //最大连接数
    bool is_et_; //et or ft

};
}

