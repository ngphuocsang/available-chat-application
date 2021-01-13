#include "GroupchatService.h"
#include "AccountFunctions.h"
#include "AccountsService.h"
#include "Database.h"
Database *mydb = Database::getInstance();
using namespace MyLibrary::Models::Requests;
using namespace MyLibrary::Models::Responses;
using namespace Server::MySQL::Functions;

namespace Server::RestServer::Services
{

    GroupsResponse GroupchatService::getAllGroups(GroupsRequest req)
    {

        GroupsResponse res;
        Account *account = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!account)
        {
            res.Result = RESULT_CODE::UNAUTHENTICATION;
            delete account;
            return res;
        }
        res.Groups = mydb->listRoom();
        return res;
    }
    InviteGroupResponse GroupchatService::InviteGroup(InviteGroupRequest req)
    {
        InviteGroupResponse res;
        Account *account = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!account)
        {
            res.Result = RESULT_CODE::UNAUTHENTICATION;
            res.Status = "Unauthenticated";
            delete account;
            return res;
        }
        if (mydb->addUserToGroupChat(req.Clients, req.GroupId) != 0)
        {
            res.Result = RESULT_SUCCESS;
            res.Status = "Success";
        }
        else
        {
            res.Result = RESULT_UNKNOWN;
            res.Status = "Unsucess";
        }

        return res;
    }
    JoinGroupResponse GroupchatService::JoinGroup(JoinGroupRequest req)
    {
        JoinGroupResponse res;
        Account *account = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!account)
        {
            res.Result = RESULT_CODE::UNAUTHENTICATION;
            res.Status = "Unauthenticated";
            delete account;
            return res;
        }
        Groupchat *singleGroup = mydb->readGroupchat(req.GroupId);
        if (singleGroup == NULL)
        {
            res.Result = RESULT_CODE::GROUP_INEXIST;
            res.Status = "Group inexist";
            return res;
        }
        // cout << "Group Name: " << singleGroup->Name << endl;
        vector<string> client;
        client.push_back(req.ClientName);
        // cout << "Group Password: " << singleGroup->Password << endl;
        if (singleGroup->Password.compare("") == 0)
        {
            //password is empty, PUBLIC ROOM
            if (mydb->addUserToGroupChat(client, req.GroupId) != 0)
            {
                res.Result = RESULT_SUCCESS;
                res.Status = "Success";
            }
            else
            {
                res.Result = RESULT_UNKNOWN;
                res.Status = "Unsuccess";
            }
        }
        else
        {
            //password is not empty, PRIVATE ROOM
            if (req.Password.compare(singleGroup->Password) == 0)
            {
                //password qualify
                if (mydb->addUserToGroupChat(client, req.GroupId) != 0)
                {
                    res.Result = RESULT_SUCCESS;
                    res.Status = "Success";
                }
                else
                {
                    res.Result = RESULT_UNKNOWN;
                    res.Status = "Unsuccess";
                }
            }
            else
            {
                //password inqualify
                res.Result = RESULT_UNKNOWN;
                res.Status = "Unsuccess";
            }
        }
        return res;
    }
    LeaveGroupResponse GroupchatService::LeaveGroup(LeaveGroupRequest req)
    {
        LeaveGroupResponse res;
        Account *account = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!account)
        {
            res.Result = RESULT_CODE::UNAUTHENTICATION;
            res.Status = "Unauthenticated";
            delete account;
            return res;
        }
        Groupchat *singleGroup = mydb->readGroupchat(req.GroupId);
        if (singleGroup == NULL)
        {
            res.Result = RESULT_CODE::RESULT_UNKNOWN;
            res.Status = "Unsuccess";
            return res;
        }

        vector<string> gr_user = mydb->getGroupUser(req.GroupId);
        if (gr_user.size() > 0)
        {
            if (gr_user[0].compare("FAIL") == 0)
            {
                res.Result = RESULT_CODE::RESULT_UNKNOWN;
                res.Status = "Unsuccess";
                return res;
            }
        }

        if (singleGroup->Owner.compare(req.ClientName) == 0) // if the owner leave group chat delete the group
        {

            if (gr_user.size() == 1) //group only include owner
                mydb->deleteGroupChat(req.GroupId);
            else
            {
                if (mydb->deleteUserFromGroupChat(req.ClientName, req.GroupId) != 0)
                {

                    if (gr_user[0].compare(req.ClientName) != 0)
                    {
                        if (mydb->inheritFromOwner(req.GroupId, gr_user[0]) == 0)
                        {
                            res.Result = RESULT_CODE::RESULT_UNKNOWN;
                            res.Status = "Unsuccess";
                            return res;
                        }
                    }
                    else
                    {
                        if (mydb->inheritFromOwner(req.GroupId, gr_user[1]) == 0)
                        {
                            res.Result = RESULT_CODE::RESULT_UNKNOWN;
                            res.Status = "Unsuccess";
                            return res;
                        }
                    }
                }
                else
                {
                    res.Result = RESULT_CODE::RESULT_UNKNOWN;
                    res.Status = "Unsuccess";
                    return res;
                }
            }
            res.Result = RESULT_CODE::RESULT_SUCCESS;
            res.Status = "Success";
        }
        else
        {
            if (mydb->deleteUserFromGroupChat(req.ClientName, req.GroupId) != 0)
            {
                res.Result = RESULT_CODE::RESULT_SUCCESS;
                res.Status = "Success";
            }
            else
            {
                res.Result = RESULT_CODE::RESULT_UNKNOWN;
                res.Status = "Unsuccess";
            }
        }
        return res;
    }

    CreateGroupResponse GroupchatService::CreateGroup(CreateGroupRequest req)
    {
        CreateGroupResponse res;
        Account *account = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!account)
        {
            res.Result = RESULT_CODE::UNAUTHENTICATION;
            res.Status = "Unauthenticated";
            delete account;
            return res;
        }
        if (req.Access)
        {
            if (req.Password.compare("") == 0)
            {
                res.Result = RESULT_CODE::GROUP_PASSWORD_NULL;
                res.Status = "Password is NULL";
                return res;
            }
        }
        int code = mydb->createGroupChat(req.Name, req.Access, req.Owner, req.Password);
        if (code == 0)
        {
            res.Result = RESULT_CODE::RESULT_UNKNOWN;
            res.Status = "Unsuccess";
        }
        else
        {
            res.Result = RESULT_CODE::RESULT_SUCCESS;
            res.Status = "success";
        }
        return res;
    }
    DeleteGroupResponse GroupchatService::DeleteGroup(DeleteGroupRequest req)
    {
        DeleteGroupResponse res;
        // cout << "start Readgroupchat\n";
        Account *account = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!account)
        {
            res.Result = RESULT_CODE::UNAUTHENTICATION;
            res.Status = "Unauthenticated";
            delete account;
            return res;
        }
        Groupchat *singleGroup = mydb->readGroupchat(req.GroupId);
        if (singleGroup == NULL)
        {
            res.Result = RESULT_CODE::GROUP_INEXIST;
            res.Status = "Group inexist";
            return res;
        }
        // cout << "done Readgroupchat\n";
        Account *acc = mydb->readAccount(req.clientName);
        if (acc == 0)
        {
            res.Result = RESULT_CODE::GROUP_ACCOUNT_INVALID;
            res.Status = "Read Account Invalid";
            return res;
        }
        // cout << "User: " << req.clientName << endl;
        if (acc->Role.compare("0") == 0)
        {
            cout << "Role: " << acc->Role << endl;
            int code = mydb->deleteGroupChat(req.GroupId);

            if (code == 0)
            {
                res.Result = RESULT_CODE::RESULT_UNKNOWN;
                res.Status = "Unsuccess";
                return res;
            }
            else
            {
                res.Result = RESULT_CODE::RESULT_SUCCESS;
                res.Status = "Success";
                return res;
            }
        }
        if (singleGroup->Owner.compare(req.clientName) != 0 || singleGroup->Password.compare(req.Password) != 0)
        {
            res.Result = RESULT_CODE::GROUP_OWNER_OR_PASSWORD_INVALID;
            res.Status = "Owner or Password Invalid";
            return res;
        }

        int code = mydb->deleteGroupChat(req.GroupId);
        // cout << "done Delete group chat\n";
        if (code == 0)
        {
            res.Result = RESULT_CODE::RESULT_UNKNOWN;
            res.Status = "Unsuccess";
            return res;
        }
        else
        {
            res.Result = RESULT_CODE::RESULT_SUCCESS;
            res.Status = "Success";
            return res;
        }
    }
    ModifyGroupResponse GroupchatService::ModifyGroup(ModifyGroupRequest req)
    {
        ModifyGroupResponse res;
        Account *account = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!account)
        {
            res.Result = RESULT_CODE::UNAUTHENTICATION;
            res.Status = "Unauthenticated";
            delete account;
            return res;
        }
        Groupchat *singleGroup = mydb->readGroupchat(req.GroupId);
        if (singleGroup == NULL)
        {
            res.Result = RESULT_CODE::GROUP_INEXIST;
            res.Status = "Group inexist";
            return res;
        }

        Account *acc = mydb->readAccount(req.clientName);
        if (acc == 0)
        {
            res.Result = RESULT_CODE::GROUP_ACCOUNT_INVALID;
            res.Status = "Read Account Invalid";
            return res;
        }
        if (acc->Role.compare("0") == 0)
        {
            if (req.newName.compare(singleGroup->Name) != 0)
            {
                int code = mydb->updateGroupChatName(req.newName, req.GroupId);
                if (code == 0)
                {
                    res.Result = RESULT_CODE::RESULT_UNKNOWN;
                    res.Status = "Unsuccess";
                    return res;
                }
            }
            if (!req.newPass.empty())
            {
                int code1 = mydb->updateGroupChatPassword(req.newPass, req.GroupId);
                if (code1 == 0)
                {
                    res.Result = RESULT_CODE::RESULT_UNKNOWN;
                    res.Status = "Unsuccess";
                    return res;
                }
            }

            res.Result = RESULT_CODE::RESULT_SUCCESS;
            res.Status = "Success";
            return res;
        }

        string oldName = singleGroup->Name;
        string oldPass = singleGroup->Password;
        string newName = req.newName;
        string newPass = req.newPass;
        if (singleGroup->Access == 1)
        {
            //this is private group
            if (req.Password.compare(oldPass) == 0 && req.clientName.compare(singleGroup->Owner) == 0) // kiểm tra nếu chủ request là đúng owner va password
            {

                if (oldName.compare(newName) != 0 && !newName.empty()) // Nếu tên hiện tại khác với tên mới
                {
                    int code = mydb->updateGroupChatName(newName, req.GroupId);
                    if (code == 0)
                    {
                        res.Result = RESULT_CODE::RESULT_UNKNOWN;
                        res.Status = "Unsuccess";
                        return res;
                    }
                }
                if (oldPass.compare(newPass) != 0 && !newPass.empty()) // nếu password mới khác với password hiện tại
                {
                    int code = mydb->updateGroupChatPassword(newPass, req.GroupId);
                    if (code == 0)
                    {
                        res.Result = RESULT_CODE::RESULT_UNKNOWN;
                        res.Status = "Unsuccess";
                        return res;
                    }
                }
                if (newPass.empty() && newName.empty())
                {
                    res.Result = RESULT_CODE::GROUP_UNCHANGE;
                    res.Status = "Unchange";
                    return res;
                }
                res.Result = RESULT_CODE::RESULT_SUCCESS;
                res.Status = "Success";
                return res;
            }
            else
            {
                res.Result = RESULT_CODE::GROUP_OWNER_OR_PASSWORD_INVALID;
                res.Status = "Group Owner or Password is Invalid";
                return res;
            }
        }
        else
        {
            //this is public room
            if (!req.Password.empty() || req.clientName.compare(singleGroup->Owner) != 0)
            {
                res.Result = RESULT_CODE::GROUP_OWNER_OR_PASSWORD_INVALID;
                res.Status = "Group Owner or Password is Invalid";
                return res;
            }
            if (newName.empty())
            {
                res.Result = RESULT_CODE::GROUP_NAME_EMPTY;
                res.Status = "Group name empty";
                return res;
            }
            else if (oldName.compare(newName) != 0)
            {
                int code = mydb->updateGroupChatName(newName, req.GroupId);
                if (code == 0)
                {
                    res.Result = RESULT_CODE::RESULT_UNKNOWN;
                    res.Status = "Unsuccess";
                    return res;
                }
                res.Result = RESULT_CODE::RESULT_SUCCESS;
                res.Status = "Success";
                return res;
            }

            res.Result = RESULT_CODE::RESULT_SUCCESS;
            res.Status = "Success";
            return res;
        }
    }
    ReadGroupUserResponse GroupchatService::ReadGroup(ReadGroupUserRequest req)
    {
        ReadGroupUserResponse res;
        Account *account = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!account)
        {
            res.Result = RESULT_CODE::UNAUTHENTICATION;
            res.Status = "Unauthenticated";
            delete account;
            return res;
        }
        res.users = mydb->getGroupUser(req.GroupId);
        if (res.users.size() == 0)
        {
            res.Result = RESULT_CODE::GROUP_UNFOUND;
            res.Status = "Group Unfound";
            return res;
        }
        if (res.users[0].compare("FAIL") == 0)
        {
            res.Result = RESULT_CODE::GROUP_READ_USER_FAIL;
            res.Status = "Read Group User Fail";
            return res;
        }
        res.Result = RESULT_CODE::RESULT_SUCCESS;
        res.Status = "Success";
        return res;
    }
} // namespace Server::RestServer::Services