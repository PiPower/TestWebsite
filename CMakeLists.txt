cmake_minimum_required(VERSION 3.5)

project(web_server)

add_subdirectory("third_party/drogon")

set(SERVER_SRC 
    server/main.cpp
    server/controllers/FirstController.cpp
)   

add_executable(wce_server ${SERVER_SRC})
target_link_libraries(wce_server PRIVATE Drogon::Drogon)
