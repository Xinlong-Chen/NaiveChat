include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

aux_source_directory(. SRC_LIST)
aux_source_directory(./net NET_LIST)
aux_source_directory(./dispatcher DISPATCHER_LIST)
aux_source_directory(./db DB_LIST)
aux_source_directory(./dao DAO_LIST)
aux_source_directory(./controller CONTROLLER_LIST)
aux_source_directory(./service SERVICE_LIST)
aux_source_directory(./utils UTILS_LIST)


add_executable(ChatServer ${SRC_LIST} ${NET_LIST} ${DISPATCHER_LIST} 
                            ${DB_LIST} ${DAO_LIST} ${CONTROLLER_LIST}
                            ${SERVICE_LIST} ${UTILS_LIST})

target_link_libraries(ChatServer muduo_net muduo_base mysqlclient hiredis pthread)
