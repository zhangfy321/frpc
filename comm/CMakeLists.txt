# include_directories() # 向工程添加头文件搜索路径，相当于指定g++编译器的 -I参数
# link_directories() # 添加库文件搜索路径
# add_library() # 生成库文件 add_library(libname [SHARED|STATIC|MOUDULE|EXCLUED_FROM_ALL] source1 source2...)
# SHARED 动态库.so  STATIC 静态库.a
# 静态链接库 和 动态链接库的区别 https://zhuanlan.zhihu.com/p/71372182
# 静态库链接时会把整个二进制代码拷贝进目标中，所以静态链接的可执行文件不需要依赖其他的内容即可运行，静态链接的要比动态链接加载更快 
# 静态库占用的磁盘空间相对比动态库要大

# add_executable(target_name, source1, source2...) # 生成可执行文件

# target_link_libraries(target_name, lib1, lib2...) # 为target添加 动态链接库

# add_subdirectory(src_dir, [BINARY_DIR][EXCLUDED_FROM_ALL]) # 添加存放源文件的子目录，可以指定中间/目标二进制的存放位置

# aux_source_directory(dir, VARIABLE) # 将一个目录下的所有源代码列表存储在一个变量中，用于构建源文件列表
# aux_source_directory(. SRC)
# add_executable(main ${SRC}})
set (comm_SRCS
    # format.h
    # macros.h
    string_util.cc
)


find_library(LIBJEMALLOC libjemalloc.so /usr/local/jemalloc/lib)
add_library(frpc_comm ${comm_SRCS})
target_link_libraries(frpc_comm ${LIBJEMALLOC})

# file(GLOB HEADERS "*.h")
# install(FILES ${HEADERS} DESTINATION include/burger/net)
