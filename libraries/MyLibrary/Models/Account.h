#pragma once

#include <string>

using namespace std;

namespace MyLibrary::Models
{
    class Account
    {
    public:
        Account(string username, string password, string role, string authkey = "");
        string Username;
        string Password;
        string Role;
        string AuthKey;
    };
} // namespace MyLibrary::Models
