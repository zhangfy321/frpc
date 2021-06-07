#define DEBUG(fmt, args...) fprintf(stderr,"\nDEBUG (%s:%d:%s): " fmt, __FILE__, __LINE__, __func__, ##args)
#define ERROR(fmt, args...) fprintf(stderr,"\n\033[1;31m ERROR (%s:%d:%s): \033[0m" fmt, __FILE__, __LINE__, __func__, ##args)
#define INFO(fmt, args...) fprintf(stderr,"\nINFO (%s:%d:%s): " fmt, __FILE__, __LINE__, __func__, ##args)
