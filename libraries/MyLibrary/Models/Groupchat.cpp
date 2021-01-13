#include "Groupchat.h"

namespace MyLibrary::Models
{
	Groupchat::Groupchat(unsigned int ID, string NAME, bool ACCESS, string OWNER, string PASSWORD)
	{
		Id = ID;
		Name = NAME;
		Access = ACCESS;
		Owner = OWNER;
		Password = PASSWORD;
	}
	Groupchat::Groupchat(string NAME, bool ACCESS, string owner, string PASSWORD)
	{
		Name = NAME;
		Access = ACCESS;
		Owner = owner;
		Password = PASSWORD;
	}
	Groupchat::Groupchat(unsigned int ID, string NAME, bool ACCESS, string owner)
	{
		Id = ID;
		Name = NAME;
		Access = ACCESS;
		Owner = owner;
	}
} // namespace MyLibrary::Models
