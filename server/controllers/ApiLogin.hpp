#pragma once
#include <drogon/HttpSimpleController.h>

using namespace drogon;
class ApiLogin : public drogon::HttpSimpleController<ApiLogin>
{
public:
    virtual void asyncHandleHttpRequest(
        const HttpRequestPtr &req,
        std::function<void (const HttpResponsePtr &)> &&callback) override;

    PATH_LIST_BEGIN
    //list path definitions here;
    PATH_ADD("/api/login", Get);
    PATH_LIST_END
};