include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

aux_source_directory(. SRC_LIST)
aux_source_directory(./net NET_LIST)


add_executable(ChatServer ${SRC_LIST} ${NET_LIST})

target_link_libraries(ChatServer muduo_net muduo_base pthread)
