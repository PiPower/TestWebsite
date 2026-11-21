#include "UserState.hpp"
#include <memory>

std::shared_ptr<UserState> createUserState(uint64_t userId) 
{
    std::shared_ptr<UserState> out = std::make_shared<UserState>();
    out->userId = userId;
    return out;
}