#include <string>
#include <memory>

struct UserState
{
    std::string username;
};

std::shared_ptr<UserState> createUserState(std::string username);