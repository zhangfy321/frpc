#ifndef FRPC_COMM_FORMAT_H
#define FRPC_COMM_FORMAT_H

#define DEBUG(fmt, args...) fprintf(stderr, "\n\033[0;32m DEBUG (%s:%d:%s): \033[0m" fmt, __FILE__, __LINE__, __func__, ##args)
#define ERROR(fmt, args...) fprintf(stderr, "\n\033[1;31m ERROR (%s:%d:%s): \033[0m" fmt, __FILE__, __LINE__, __func__, ##args)
#define INFO(fmt, args...) fprintf(stderr, "\nINFO (%s:%d:%s): " fmt, __FILE__, __LINE__, __func__, ##args)

/*
    格式：\033[显示方式;前景色;背景色m

    说明：
    前景色            背景色           颜色
    ---------------------------------------
    30                40              黑色
    31                41              红色
    32                42              绿色
    33                43              黃色
    34                44              蓝色
    35                45              紫红色
    36                46              青蓝色
    37                47              白色
    显示方式           意义
    -------------------------
    0                终端默认设置
    1                高亮显示
    4                使用下划线
    5                闪烁
    7                反白显示
    8                不可见
*/

#endif //FRPC_COMM_FORMAT_H