#include "EndPoints.hpp"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include "drogon/orm/Result.h"
#include <memory>
#include <random>
#include <crypt.h>
#include <trantor/utils/Logger.h>
#include <drogon/orm/Exception.h>
#include "UserState.hpp"
#include "Utils.hpp"


using namespace std;

constexpr std::array<char, 64> yescrypt_chars = {
    '.', '/',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z'
};

const std::string saltPrefix = "$y$j9T$";


void EndPoints::createAccount(
     const drogon::HttpRequestPtr &req, 
     function<void(const drogon::HttpResponsePtr &)> &&callback)
{
    const Json::Value& json = *req->getJsonObject();
    auto resp = HttpResponse::newHttpResponse();

    string username = json["create_username"].asString();
    string email = json["create_email"].asString();
    string password = json["create_password"].asString();
    string password_repeat = json["repeat_password"].asString();

    if(!validateCredentials(username, email, password, password_repeat))
    {
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }
    
    string salt(16, '0');
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, yescrypt_chars.size());
    for(size_t i = 0; i < 16; i++)
    {
        salt[i] = yescrypt_chars[dist(gen)];
    }
    salt = saltPrefix + salt;

    struct crypt_data data = {0};
    char* hash = crypt_r(password.c_str(), salt.c_str(), &data);
    if (hash == NULL) 
    {
        LOG_DEBUG << "Password hashing failed\n";
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    auto clientPtr = drogon::app().getDbClient();
    uint64_t userId = -1;
    try
    {
        orm::Result queryRes = clientPtr->execSqlSync(
            "INSERT INTO users (username, salt, password_hash, email) "
            "VALUES ($1, $2, $3, $4)"
            "RETURNING id;",
            username, salt.c_str() + saltPrefix.size(), hash + saltPrefix.size(), email);

        userId = queryRes[0]["id"].as<uint64_t>();
    }
    catch (const orm::DrogonDbException &e)
    {
        resp->setStatusCode(k400BadRequest);
        const drogon::orm::SqlError *s = dynamic_cast<const drogon::orm::SqlError*>(&e.base());
        if (!s)
        {
            callback(resp);
            return;
        }
        if(s->sqlState() == "23505")
        {
            resp->setBody("Username or Email is used");
        }
        callback(resp);
        return;
        
    }
    
    req->session()->insert(USER_KEY, createUserState(userId));
    resp->setStatusCode(drogon::k302Found);
    callback(resp);

}

void EndPoints::login(
     const drogon::HttpRequestPtr &req, 
     function<void(const drogon::HttpResponsePtr &)> &&callback)
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
            "SELECT id, salt, password_hash FROM users WHERE email = $1;",
            username) 
        : 
        clientPtr->execSqlSync(
            "SELECT id, salt, password_hash FROM users WHERE username = $1;",
            username);

    if(queryResult.size() == 0)
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k404NotFound);
        resp->setBody("login");
        callback(resp);
        return;
    }

    uint64_t userId = queryResult[0]["id"].as<uint64_t>();
    req->session()->insert(USER_KEY, createUserState(userId));
    
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k302Found);
    callback(resp);
}

void EndPoints::index(
     const drogon::HttpRequestPtr &req, 
     function<void(const drogon::HttpResponsePtr &)> &&callback)
{
    if(redirectHomeOnExistingSession(req, callback))
    {
        return;
    }

    const std::string* path = &req->getPath();
    if(path->empty() || *path == "/") {
        static const std::string indexPath = "/index.html";
        path = &indexPath;
    }

    const std::string &rootPath = app().getDocumentRoot();
    if (!std::filesystem::is_regular_file(rootPath + *path)) {
        callback(HttpResponse::newNotFoundResponse(req));
        return;
    }

    auto resp = HttpResponse::newFileResponse(rootPath + *path);
    resp->setStatusCode(k200OK);
    callback(resp);
}

void EndPoints::home(
     const drogon::HttpRequestPtr &req,
     std::function<void(const drogon::HttpResponsePtr &)> &&callback)
{
    if(!req->session()->find(USER_KEY))
    {
        HttpResponsePtr resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k301MovedPermanently);
        resp->addHeader("Location", "/");
        callback(resp);
        return;
    }

    shared_ptr<UserState> user = req->session()->get<shared_ptr<UserState>>(USER_KEY);
    auto clientPtr = drogon::app().getDbClient();
    drogon::orm::Result queryResult = clientPtr->execSqlSync(
                "SELECT * FROM users WHERE users.id = $1;",
                to_string(user->userId));


    const std::string &rootPath = app().getDocumentRoot();
    const char* field = queryResult.at(0).at("username").c_str();
    HttpResponsePtr resp =  HttpResponse::newFileResponse(rootPath + "/home.html");
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
}