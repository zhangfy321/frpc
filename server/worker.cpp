//
// Created by fy on 2021/1/14.
//

#include "worker.h"
using std::cout;
using std::endl;

void Worker::operator ()() {
    cout << "线程开始运行" << endl;
    m_epoll.main_loop();
}


