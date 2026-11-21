#pragma once
#include <string>

bool validateCredentials(
    const std::string& username, 
    const std::string& email, 
    const std::string& password, 
    const std::string& password_repeat);

bool isValidEmail(const std::string& email);