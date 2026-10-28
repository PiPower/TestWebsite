#include <drogon/drogon.h>

int main()
{
    drogon::HttpAppFramework& app = drogon::app();
    app.setLogPath("./")
        .setDocumentRoot("./frontend")
       .setLogLevel(trantor::Logger::kWarn)
       .setBeforeListenSockOptCallback([](int fd) {
            int enable = 1;
            setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                       &enable, sizeof(enable));
        })
       .addListener("127.0.0.1", 6543)
       .setThreadNum(3)
       .run();
}