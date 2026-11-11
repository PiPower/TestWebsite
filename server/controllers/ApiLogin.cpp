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
    const Json::Value& json = *req->getJsonObject();
    const SafeStringMap<std::string>& params = req->getParameters();


    std::string username = json["username"].asString();
    std::string password = json["password"].asString();

    if(username.size() == 0 || password.size() == 0)
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

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