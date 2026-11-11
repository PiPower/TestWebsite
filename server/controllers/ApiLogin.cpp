#include "ApiLogin.hpp"
#include <crypt.h>

inline bool isEmail(const std::string& str)
{
    for(size_t i = 0; i < str.size(); ++i)
    {
        if(str[i] == '@')
        {
            return true;
        }
    }
    return false;
}

void ApiLogin::asyncHandleHttpRequest(
    const HttpRequestPtr &req,
     std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto clientPtr = drogon::app().getDbClient();

    const std::string* path = &req->getPath();
    const SafeStringMap<std::string>& params = req->getParameters();
    if(params.empty() || 
        params.find("username") == params.end() || 
        params.find("password") == params.end())
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    const std::string& username = params.at("username");
    const std::string& password = params.at("password");    

    bool isUsernameEmail = isEmail(username);
    drogon::orm::Result queryResult = isUsernameEmail ?
        clientPtr->execSqlSync(
            "SELECT salt, password_hash FROM users WHERE email = $1",
            username) 
        : 
        clientPtr->execSqlSync(
            "SELECT salt, password_hash FROM users WHERE username = $1",
            username);

    if(queryResult.size() == 0)
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k404NotFound);
        resp->setBody("login");
        callback(resp);
        return;
    }

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    callback(resp);
}