#include <drogon/drogon.h>
#include <trantor/utils/Logger.h>

int main()
{
    drogon::HttpAppFramework& app = drogon::app();
    drogon::orm::PostgresConfig pgConfig = {};
    pgConfig.port = 5432;
    pgConfig.host = "localhost";
    pgConfig.databaseName = "testdb";
    pgConfig.username = "postgres";
    pgConfig.password = "secret"; 
    pgConfig.connectionNumber = 3; // Number of connections in the pool
    pgConfig.name = "default";
    app.addDbClient(pgConfig);
    app.enableSession(20);
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

    app.enableReusePort();
    
    app.setDocumentRoot("./frontend")
        .setLogPath("", "")
        .setLogLevel(trantor::Logger::kDebug)
        .addListener("127.0.0.1", 6543)
        .setThreadNum(3)
        .run();
}