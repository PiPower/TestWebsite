#include "Utils.hpp"
#include <cctype>
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
using namespace drogon;

bool validateCredentials(
    const std::string& username, 
    const std::string& email, 
    const std::string& password, 
    const std::string& password_repeat)
{
    if(!isValidEmail(email))
    {
        return false;
    }

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

bool isValidEmail(const std::string& email)
{
    int atPos = -1;
    int dotPos = -1;

    if (email.empty()) return false;

    for (int i = 0; i < email.length(); i++) 
    {
        char c = email[i];

        if (c == '@') 
        {
            if (atPos != -1) return false;
            atPos = i;
        }
        else if( c == '.')
        {
            dotPos = i;
        }
        else if(!isalnum(c) && c != '.' && c != '_' &&
            c != '-' && c != '+' && c != '@') 
        {
            return false;
        }
    }

    // Check @ position
    if (atPos <= 0 || atPos == email.length() - 1)
        return false;


    // Check dot position
    if (dotPos <= atPos + 1 || dotPos == email.length() - 1)
        return false;


    // Check domain extension (at least 2 characters)
    if (email.length() - dotPos - 1 < 2)
        return false;

    return true;
}

bool redirectHomeOnExistingSession(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &callback)
{
    if(!req->session()->find(USER_KEY))
    {
        return false;
    }

    HttpResponsePtr resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k301MovedPermanently);
    resp->addHeader("Location", "/home");
    callback(resp);
    return true;
}