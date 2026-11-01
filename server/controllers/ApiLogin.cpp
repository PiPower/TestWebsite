#include "ApiLogin.hpp"

void ApiLogin::asyncHandleHttpRequest(
    const HttpRequestPtr &req,
     std::function<void(const HttpResponsePtr &)> &&callback)
{
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

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    callback(resp);
}