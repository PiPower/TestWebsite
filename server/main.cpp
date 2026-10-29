#include <drogon/drogon.h>
#include <trantor/utils/Logger.h>

int main()
{
    drogon::HttpAppFramework& app = drogon::app();

    app.registerPreRoutingAdvice(
        [](const drogon::HttpRequestPtr& req) {
            std::string log = req->methodString() + std::string(" ") +
                            req->path() + " HTTP/1.1\r\n";

            for (const auto& [name, value] : req->getHeaders()) {
                log += name + std::string(": ") + value + "\r\n";
            }

            log += "\r\n";
            log += req->body();

            LOG_DEBUG << log;
        });

    app.setDocumentRoot("./frontend")
        .setLogPath("", "")
        .setLogLevel(trantor::Logger::kDebug)
        .setBeforeListenSockOptCallback([](int fd) {
            int enable = 1;
            setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                       &enable, sizeof(enable));
            })
        .addListener("127.0.0.1", 6543)
        .setThreadNum(3)
        .run();
}