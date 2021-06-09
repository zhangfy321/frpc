# set(hook_SRCS
#   Hook.cc)

# # https://www.coder.work/article/6663009

# set_source_files_properties(
#   ${hook_SRCS}
#   PROPERTIES
#   COMPILE_FLAGS "-w"
# )
SET(JEMALLOC_INCLUDE_DIR /usr/include/jemalloc/include)
SET(JEMALLOC_LIB /usr/local/jemalloc/lib)
 



set(net_SRCS
  server.cc
  io/io_loop.cc
  config/config.cc
)

# link_directories(${JEMALLOC_LIB})
# include_directories(${JEMALLOC_INCLUDE_DIR})



add_library(frpc_base ${net_SRCS})
target_link_libraries(frpc_base frpc_comm)

# install(TARGETS frpc_comm DESTINATION lib)

# file(GLOB HEADERS "*.h")
# install(FILES ${HEADERS} DESTINATION include/burger/net)