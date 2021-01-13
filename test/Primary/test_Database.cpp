#include <gtest/gtest.h>
#include "Database.h"
#include "Account.h"
#include "Groupchat.h"
#include "MyLibrary.h"
using namespace MyLibrary;
// using::testing::AtLeast;
// using::testing::Return;
// using::testing::_;

Database *MySqlServer = Database::getInstance();

TEST(createChatRoom, CanCreateChatRoom)
{
    vector<string> users;
    users.push_back("bao");
    users.push_back("hoang");
    EXPECT_EQ(90, MySqlServer->createChatRoom(users));
}

TEST(createAccount, FailToCreateAccountIfUsernameIsDuplicate)
{
    Account *h = new Account("hoang", "123456789", "1");
    EXPECT_FALSE(MySqlServer->createAccount(h));
    delete h;
}

TEST(deleteAccount, DeleteNonExistAccount)
{
    EXPECT_TRUE(MySqlServer->deleteAccount("khang"));
}

TEST(updateRole, UpdateInvalidRole)
{
    EXPECT_FALSE(MySqlServer->updateRole("nguyen", "2"));
}

TEST(updateRole, UpdateRole)
{
    EXPECT_TRUE(MySqlServer->updateRole("nguyen", "1"));
}

TEST(updateRole, ValidToUpdateRoleFromUserToAdmin)
{
    EXPECT_TRUE(MySqlServer->updateRole("nguyen", "0"));
}

TEST(updateRole, ValidToUpdateRoleFromAdminToUser)
{
    EXPECT_TRUE(MySqlServer->updateRole("nguyen", "1"));
}

TEST(updateRole, UpdateRoleForNonExistUser)
{
    EXPECT_TRUE(MySqlServer->updateRole("khang", "1"));
}

TEST(updatePassword, UpdatePassword)
{
    EXPECT_TRUE(MySqlServer->updatePassword("nguyen", "123456789"));
}

TEST(updatePassword, UpdatePasswordForNonExistUser)
{
    EXPECT_TRUE(MySqlServer->updatePassword("khang", "123456789"));
}

TEST(getRoomUser, GetRoomUser)
{
    vector<string> users = MySqlServer->getRoomUser(90);
    EXPECT_EQ("bao", users[0]);
    EXPECT_EQ("hoang", users[1]);
}

TEST(getRoomUser, GetNonExistRoomUser)
{
    EXPECT_TRUE(MySqlServer->getRoomUser(50).empty());
}

TEST(readAccount, ReadAccount)
{
    Account *khuong = MySqlServer->readAccount("khuong");
    bool readaccount;
    if ("khuong" == khuong->Username && "123456789" == khuong->Password && "1" == khuong->Role)
        readaccount = 1;
    else
        readaccount = 0;
    EXPECT_TRUE(readaccount);
}

TEST(readAccount, ReadNonExistAccount)
{
    EXPECT_EQ(NULL, MySqlServer->readAccount("khang"));
}
/*
 TEST(DatabaseTest, AddMessage) {
   EXPECT_TRUE(MySqlServer->addMessage("hello", 80));
 }

 TEST(DatabaseTest, AddMessageToNonExistTest) {
   EXPECT_TRUE(MySqlServer->addMessage("hello", 70));
 }
*/
TEST(DatabaseTest, ReadRecentMessage)
{
    EXPECT_EQ(-1, MySqlServer->readRecentMessage(80));
}

TEST(DatabaseTest, ReadMessage)
{
    EXPECT_EQ("./chat_history/80.txt", MySqlServer->readMessage(80));
}

TEST(DatabaseTest, ReadMessageFromNonExistRoom)
{
    EXPECT_TRUE(MySqlServer->readMessage(70).empty());
}

TEST(DatabaseTest, FailToInsertAuthenRecord)
{
    EXPECT_EQ(-2, MySqlServer->insertAuthenRecord("nguyen", "2"));
}

TEST(DatabaseTest, AddUserToGroupChat)
{
    vector<string> users;
    users.push_back("hoang");
    users.push_back("hai");
    EXPECT_TRUE(MySqlServer->addUserToGroupChat(users, 1));
}

TEST(DatabaseTest, DeleteUserFromGroupChat)
{
    EXPECT_TRUE(MySqlServer->deleteUserFromGroupChat("khang", 1));
}

TEST(DatabaseTest, CreatePrivateGroupChatWithNullPassword)
{
    EXPECT_FALSE(MySqlServer->createGroupChat("test", true, "test", ""));
}

TEST(getGroupUser, GetAllUsersInGroup)
{
    vector<string> users;
    users.push_back("hai");
    users.push_back("hoang");
    users.push_back("khuong");
    users.push_back("nguyen");
    users.push_back("SevenEleven");
    EXPECT_EQ(users, MySqlServer->getGroupUser(10));
}

TEST(getGroupUser, GetAllUsersInGroupHas1Person)
{
    vector<string> users;
    users.push_back("Dat");
    EXPECT_EQ(users, MySqlServer->getGroupUser(6));
}

/*
TEST(createGroupChat, CanCreateGroupChat) {
    EXPECT_TRUE(MySqlServer->createGroupChat("testing room",0,"test_user","1234"));
}

TEST(createGroupChat, CanCreateGroupChatWithSameName) {
	EXPECT_TRUE(MySqlServer->createGroupChat("testing room",0,"test_user","1234"));
}*/

TEST(deletGroupChat, DeleteNonExistGroupChatID)
{
    EXPECT_TRUE(MySqlServer->deleteGroupChat(17));
}
/*
TEST(deletGroupChat, DeleteExistGroupChatID) {
	EXPECT_TRUE(MySqlServer->deleteGroupChat(1));
}*/

TEST(getUserGroupList, GetUserFromOneGroupOnly)
{
    vector<int> k;
    k.push_back(8);
    EXPECT_EQ(k, MySqlServer->getUserGroupList("Sayonara"));
}

TEST(getUserGroupList, GetUserFromManyGroups)
{
    vector<int> k;
    k.push_back(1);
    k.push_back(9);
    k.push_back(10);
    EXPECT_EQ(k, MySqlServer->getUserGroupList("hai"));
}

TEST(DatabaseTest, GetUserFromManyGroupsAnd1GroupHasOnly1Person)
{
    vector<int> k;
    k.push_back(2);
    k.push_back(7);
    k.push_back(11);
    k.push_back(12);
    EXPECT_EQ(k, MySqlServer->getUserGroupList("bao"));
}

TEST(updateGroupChatPassword, UpdatePasswordOfARoom)
{
    EXPECT_TRUE(MySqlServer->updateGroupChatPassword("2345", 7));
}

TEST(updateGroupChatPassword, UpdatePasswordOfARoomWithTheSamePreviousPassword)
{
    EXPECT_TRUE(MySqlServer->updateGroupChatPassword("1234", 7));
}

TEST(updateGroupChatPassword, UpdatePasswordOfARoomWithTheSamePassword)
{
    EXPECT_TRUE(MySqlServer->updateGroupChatPassword("1234", 7));
}

TEST(updateGroupChatName, UpdateGroupChatName)
{
    EXPECT_TRUE(MySqlServer->updateGroupChatName("Epic Of Remnant", 7));
}
TEST(updateGroupChatName, UpdateGroupChatNameWithTheSamePreviousName)
{
    EXPECT_TRUE(MySqlServer->updateGroupChatName("Falling Temple SOLOMON", 7));
}
TEST(updateGroupChatName, UpdateGroupChatNameWithTheSameName)
{
    EXPECT_TRUE(MySqlServer->updateGroupChatName("Falling Temple SOLOMON", 7));
}

TEST(readGroupchat, AbleReadPublicGroupChat)
{
    Groupchat *a, *b;
    a = new Groupchat("Gate of Chaldea", 0, "khuong", "");
    b = MySqlServer->readGroupchat(5);
    EXPECT_EQ(a->Name, b->Name);
}
TEST(readGroupchat, AbleToReadPrivateGroupChat)
{
    Groupchat *a, *b;
    a = new Groupchat("Falling Temple SOLOMON", 1, "Dat", "1234");
    b = MySqlServer->readGroupchat(7);
    bool khuong;
    if (a->Name == b->Name && a->Access == b->Access && a->Owner == b->Owner && a->Password == b->Password)
        khuong = 1;
    else
        khuong = 0;
    EXPECT_EQ(1, khuong);
}
int main(int argc, char **argv)
{
    AppSetting::loadSettingServerFromFile();
    MySqlServer->initialize(
        AppSetting::SettingValues["MySqlServIP"],
        AppSetting::SettingValues["MySqlServUser"],
        AppSetting::SettingValues["MySqlServPass"],
        AppSetting::SettingValues["MySqlServDbname"]);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}