#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class ApiCreateAccount : public HttpController<ApiCreateAccount>
{
public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    ADD_METHOD_TO (ApiCreateAccount::createAccount,"api/create_account", Post);

    METHOD_LIST_END

    void createAccount(const drogon::HttpRequestPtr &req,
                       std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};