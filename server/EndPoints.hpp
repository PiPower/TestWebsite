#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class EndPoints : public HttpController<EndPoints>
{
public:
    METHOD_LIST_BEGIN
    
    ADD_METHOD_TO(EndPoints::createAccount,"api/create_account", Post);
    ADD_METHOD_TO(EndPoints::login, "/api/login", Post);
    ADD_METHOD_TO(EndPoints::index, "/", Get);
    ADD_METHOD_TO(EndPoints::home, "/home", Get);

    METHOD_LIST_END

    void createAccount(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void login(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void index(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void home(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};