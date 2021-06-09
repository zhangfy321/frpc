add_executable(cli client.cc client.h)
target_link_libraries(cli frpc_comm)