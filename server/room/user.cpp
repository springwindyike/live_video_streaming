#include "user.h"

User::User():speak(true)
{

}

bool User::operator<(const User &rhs) const
{
    return this->username < rhs.username;
}
