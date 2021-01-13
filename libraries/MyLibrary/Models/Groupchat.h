#pragma once

#include <string>

using namespace std;
namespace MyLibrary::Models
{
    class Groupchat
    {
    public:
        Groupchat();
        Groupchat(unsigned int ID, string NAME, bool ACCESS, string owner, string PASSWORD);
        Groupchat(string NAME, bool ACCESS, string owner, string PASSWORD);
        Groupchat(unsigned int ID, string NAME, bool ACCESS, string owner);
        unsigned int Id;
        string Name;
        bool Access;
        string Owner;
        string Password;
    };

} // namespace MyLibrary::Models