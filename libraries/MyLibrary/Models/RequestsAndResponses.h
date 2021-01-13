#pragma once

#include "MyLibrary.h"
#include "Account.h"
#include "Groupchat.h"
#include <string>
#include <vector>

using namespace std;
using namespace MyLibrary;
using namespace MyLibrary::Models;

namespace MyLibrary::Models::Requests
{
    class BaseAuthenRequest
    {
    public:
        string AuthKey;
        BaseAuthenRequest(string authkey);
    };
    class LogoutAccountRequest
    {
    public:
        string Username;
        LogoutAccountRequest(string username);
    };
    class ReadGroupUserRequest : public BaseAuthenRequest
    {
    public:
        unsigned int GroupId;
        ReadGroupUserRequest(string authKey, unsigned int groupid);
    };
    class ModifyGroupRequest : public BaseAuthenRequest
    {
    public:
        unsigned int GroupId;
        string clientName;
        string newName;
        string newPass;
        string Password;
        ModifyGroupRequest(string key, unsigned int id, string name, string newname, string newpass, string pass);
    };
    class DeleteGroupRequest : public BaseAuthenRequest
    {
    public:
        unsigned int GroupId;
        string clientName;
        string Password;
        DeleteGroupRequest(string authKey, unsigned int id, string name, string pass);
    };
    class CreateGroupRequest : public BaseAuthenRequest
    {
    public:
        string Name;
        bool Access;
        string Owner;
        string Password;
        CreateGroupRequest(string authKey, string name, bool access, string owner, string password);
    };

    class LeaveGroupRequest : public BaseAuthenRequest
    {
    public:
        unsigned int GroupId;
        string ClientName;
        LeaveGroupRequest(string authKey, int groupid, string clientname);
    };
    class JoinGroupRequest : public BaseAuthenRequest
    {
    public:
        int GroupId;
        string Password;
        string ClientName;
        JoinGroupRequest(string authKey, int groupid, string password, string clientname);
    };
    class InviteGroupRequest : public BaseAuthenRequest
    {
    public:
        // string AuthKey;
        vector<string> Clients;
        unsigned int GroupId;
        InviteGroupRequest(string authKey, unsigned int groupId, vector<string> clients);
        // JoinGroupRequest(string authKey, string groupId);
    };
    class GroupsRequest : public BaseAuthenRequest
    {
    public:
        // string AuthKey;
        GroupsRequest(string authKey);
    };
    class AuthenAccountRequest
    {
    public:
        string Username;
        string Password;

        AuthenAccountRequest(string username, string password);
    };
    class AccountsRequest : public BaseAuthenRequest
    {
    public:
        AccountsRequest(string authkey);
    };
    class ActiveAccountsRequest : public BaseAuthenRequest
    {
    public:
        ActiveAccountsRequest(string authkey);
    };
    class CreateAccountRequest : public BaseAuthenRequest
    {
    public:
        string Username;
        string Password;
        string Role;

        CreateAccountRequest(string authkey);
    };
    class UpdateAccountRequest : public BaseAuthenRequest
    {
    public:
        string Username;
        string Password;
        string Role;

        UpdateAccountRequest(string authkey);
    };
    class DeleteAccountRequest : public BaseAuthenRequest
    {
    public:
        string Username;

        DeleteAccountRequest(string authkey);
    };
    class GetRoomRequest
    {
    public:
        string AuthKey;

        GetRoomRequest(string authkey);
    };
    class JoinRoomRequest : public BaseAuthenRequest
    {
    public:
        int GroupID;
        string Password;
        string Name;

        JoinRoomRequest(int groupid, string password, string name, string authkey);
    };
    class InviteRoomRequest : public BaseAuthenRequest
    {
    public:
        int GroupID;
        vector<string> Username;

        InviteRoomRequest(int groupid, vector<string> username, string authkey);
    };
    class LeaveRoomRequest : public BaseAuthenRequest
    {
    public:
        int GroupID;
        string Name;

        LeaveRoomRequest(int groupid, string name, string authkey);
    };
    class CreateRoomRequest : public BaseAuthenRequest
    {
    public:
        string Roomname;
        int Access;
        string Owner;
        string Password;

        CreateRoomRequest(string roomname, int access, string owner, string password, string authkey);
    };
    class DeleteRoomRequest : public BaseAuthenRequest
    {
    public:
        int GroupID;
        string Username;
        string Password;

        DeleteRoomRequest(int groupid, string username, string password, string authkey);
    };
    class ModifyRoomRequest : public BaseAuthenRequest
    {
    public:
        int GroupID;
        string Username;
        string Newname;
        string Newpass;
        string Password;

        ModifyRoomRequest(int groupid, string username, string newname, string newpass, string password, string authkey);
    };
    class RoomMemberRequest : public BaseAuthenRequest
    {
    public:
        int GroupID;

        RoomMemberRequest(int groupid, string authkey);
    };
} // namespace MyLibrary::Models::Requests

namespace MyLibrary::Models::Responses
{
    class BaseResponse
    {
    public:
        RESULT_CODE Result;
        BaseResponse(RESULT_CODE result);
    };
    class AuthenAccountResponse : public BaseResponse
    {
    public:
        string AuthKey;
        string Role;

        AuthenAccountResponse();
        AuthenAccountResponse(RESULT_CODE result, string authkey, string role);
    };
    class ActiveAccountsResponse : public BaseResponse
    {
    public:
        vector<string> Usernames;

        ActiveAccountsResponse();
        ActiveAccountsResponse(RESULT_CODE result, vector<string> usernames);
    };
    class AccountsResponse : public BaseResponse
    {
    public:
        vector<Account> Accounts;

        AccountsResponse();
        AccountsResponse(RESULT_CODE result, vector<Account> accounts);
    };
    class GroupsResponse
    {
    public:
        RESULT_CODE Result;
        vector<Groupchat> Groups;

        GroupsResponse();
        GroupsResponse(RESULT_CODE result, vector<Groupchat> groups);
    };
    class InviteGroupResponse
    {
    public:
        RESULT_CODE Result;
        string Status;

        InviteGroupResponse();

        InviteGroupResponse(RESULT_CODE Result, string Status);
    };
    class JoinGroupResponse
    {
    public:
        RESULT_CODE Result;
        string Status;

        JoinGroupResponse();
        JoinGroupResponse(RESULT_CODE result, string status);
    };
    class GetRoomResponse
    {
    public:
        //RESULT_CODE Result;
        vector<int> Access;
        vector<int> Id;
        vector<string> Name;
        vector<string> Owner;

        GetRoomResponse();
        GetRoomResponse(RESULT_CODE result, vector<int> access, vector<int> id, vector<string> name, vector<string> owner);
    };
    class JoinRoomResponse
    {
    public:
        RESULT_CODE Result;
        string Status;

        JoinRoomResponse();
        JoinRoomResponse(RESULT_CODE result, string status);
    };
    class InviteRoomResponse
    {
    public:
        RESULT_CODE Result;
        string Status;

        InviteRoomResponse();
        InviteRoomResponse(RESULT_CODE result, string status);
    };
    class LeaveRoomResponse
    {
    public:
        RESULT_CODE Result;
        string Status;

        LeaveRoomResponse();
        LeaveRoomResponse(RESULT_CODE result, string status);
    };
    class CreateRoomResponse
    {
    public:
        RESULT_CODE Result;
        string Status;

        CreateRoomResponse();
        CreateRoomResponse(RESULT_CODE result, string status);
    };
    class LeaveGroupResponse
    {
    public:
        RESULT_CODE Result;
        string Status;

        LeaveGroupResponse();
        LeaveGroupResponse(RESULT_CODE result, string status);
    };
    class CreateGroupResponse
    {
    public:
        RESULT_CODE Result;
        string Status;
        CreateGroupResponse();
        CreateGroupResponse(RESULT_CODE result, string status);
    };

    class DeleteRoomResponse
    {
    public:
        RESULT_CODE Result;
        string Status;
        DeleteRoomResponse();
        DeleteRoomResponse(RESULT_CODE result, string status);
    };

    class DeleteGroupResponse
    {
    public:
        RESULT_CODE Result;
        string Status;
        DeleteGroupResponse(RESULT_CODE result, string status);
        DeleteGroupResponse();
    };

    class ModifyRoomResponse
    {
    public:
        RESULT_CODE Result;
        string Status;
        ModifyRoomResponse();
        ModifyRoomResponse(RESULT_CODE result, string status);
    };

    class ModifyGroupResponse
    {
    public:
        RESULT_CODE Result;
        string Status;
        ModifyGroupResponse(RESULT_CODE result, string status);
        ModifyGroupResponse();
    };
    class RoomMemberResponse
    {
    public:
        RESULT_CODE Result;
        string Status;
        vector<string> Username;

        RoomMemberResponse();
        RoomMemberResponse(RESULT_CODE result, string status, vector<string> username);
    };
    class ReadGroupUserResponse
    {
    public:
        RESULT_CODE Result;
        string Status;
        vector<string> users;
        ReadGroupUserResponse(RESULT_CODE result, string status);
        ReadGroupUserResponse();
    };

} // namespace MyLibrary::Models::Responses