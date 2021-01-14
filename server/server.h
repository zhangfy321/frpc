//
// Created by fy on 2021/1/14.
//

#ifndef FRPC_SERVER_H
#define FRPC_SERVER_H

class Server {
public:
    InitServer(int argc, char **argv);
    virtual ~Server();

    void* GetConfig();
    int Run();

private:


};


#endif //FRPC_SERVER_H
