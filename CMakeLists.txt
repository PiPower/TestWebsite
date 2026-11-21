cmake_minimum_required(VERSION 3.5)

project(web_server)

add_subdirectory("third_party/drogon")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SERVER_SRC 
    server/main.cpp
    server/controllers/FirstController.cpp
    server/controllers/ApiLogin.cpp
    server/controllers/ApiCreateAccount.cpp
    server/states/UserState.cpp
)   

add_executable(wce_server ${SERVER_SRC})
target_link_libraries(wce_server PRIVATE Drogon::Drogon)
target_link_libraries(wce_server PRIVATE crypt)