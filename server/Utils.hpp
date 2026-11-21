#pragma once
#include <string>

bool validateCredentials(
    const std::string& username, 
    const std::string& email, 
    const std::string& password, 
    const std::string& password_repeat);

bool isValidEmail(const std::string& email);

inline bool isEmail(const std::string& str)
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