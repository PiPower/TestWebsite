#include "EndPoints.hpp"
#include "drogon/HttpTypes.h"
#include "drogon/orm/Result.h"
#include <random>
#include <crypt.h>
#include <trantor/utils/Logger.h>
#include <drogon/orm/Exception.h>
#include "UserState.hpp"
using namespace std;

static inline bool isEmail(const std::string& str)
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

static bool validateCredentials(
    const std::string& username, 
    const std::string& email, 
    const std::string& password, 
    const std::string& password_repeat)
{
    if (username.empty() || 
        email.empty()    || 
        password.empty() || 
        password_repeat.empty())
    {
        return false;
    }

    if (username.size() > 50  || 
        email.size() > 100    || 
        password.size() > 100 || 
        password_repeat.size() > 100)
    {
        return false;
    }

    if (password != password_repeat)
    {
        return false;
    }

  
    // Additional validation logic can be added here (e.g., regex for email format, password strength, etc.)
    return true;
}

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
    try
    {
        orm::Result queryRes = clientPtr->execSqlSync(
            "INSERT INTO users (username, salt, password_hash, email) "
            "VALUES ($1, $2, $3, $4)",
            username, salt.c_str() + saltPrefix.size(), hash + saltPrefix.size(), email);

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
    
    req->session()->insert("user_state", createUserState(std::move(username)) );
    resp = HttpResponse::newFileResponse( app().getDocumentRoot() + "/main.html");
    resp->setStatusCode(drogon::k200OK);
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

void EndPoints::index(
    const drogon::HttpRequestPtr &req, 
    function<void(const drogon::HttpResponsePtr &)> &&callback)
{
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