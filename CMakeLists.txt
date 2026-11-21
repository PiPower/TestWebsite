cmake_minimum_required(VERSION 3.5)

project(web_server)

add_subdirectory("third_party/drogon")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SERVER_SRC 
    server/main.cpp
    server/EndPoints.cpp
    server/UserState.cpp
    server/Utils.cpp
)   

add_executable(wce_server ${SERVER_SRC})
target_link_libraries(wce_server PRIVATE Drogon::Drogon)
target_link_libraries(wce_server PRIVATE crypt)