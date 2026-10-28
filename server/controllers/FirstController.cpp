#include "FirstController.hpp"

void FirstController::asyncHandleHttpRequest(
    const HttpRequestPtr &req, 
    std::function<void(const HttpResponsePtr &)> &&callback)
{
    const std::string* path = &req->getPath();
    if(path->empty() || *path == "/") {
        static const std::string indexPath = "/index.html";
        path = &indexPath;
    }

    const std::string &rootPath = app().getDocumentRoot();
    std::cout << "Received request for path: " << rootPath + *path << std::endl;
    if (!std::filesystem::is_regular_file(rootPath + *path)) {
        callback(HttpResponse::newNotFoundResponse(req));
        return;
    }

    auto resp = HttpResponse::newFileResponse(rootPath + *path);
    resp->setStatusCode(k200OK);
    callback(resp);
}