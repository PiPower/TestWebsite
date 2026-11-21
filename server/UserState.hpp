#include <memory>

struct UserState
{
    uint64_t userId;
};

std::shared_ptr<UserState> createUserState(uint64_t userId);