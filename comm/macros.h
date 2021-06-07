#define LIKELY(x)  __builtin_expect(!!(x), 1)
#define UNLIKELY(x)    __builtin_expect(!!(x), 0)

#define RETURN_ERROR(cond, fmt, args...) if (UNLIKELY(cond)) { fprintf(stderr,"\n\033[1;31m FATAL_ERROR (%s:%d:%s): \033[0m" fmt, __FILE__, __LINE__, __func__, ##args); exit(-1);}

