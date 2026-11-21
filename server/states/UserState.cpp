#include "UserState.hpp"
#include <memory>

 std::shared_ptr<UserState> createUserState(std::string username) 
{
    std::shared_ptr<UserState> out = std::make_shared<UserState>();
    out->username = std::move(username);
    return out;
}