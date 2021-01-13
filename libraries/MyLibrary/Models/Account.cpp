#include "Account.h"

namespace MyLibrary::Models
{
    Account::Account(string username, string password, string role, string authkey)
    {
        Username = username;
        Password = password;
        Role = role;
        AuthKey = authkey;
    }
} // namespace MyLibrary::Models
