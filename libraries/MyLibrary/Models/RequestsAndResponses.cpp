#include "RequestsAndResponses.h"

using namespace std;

namespace MyLibrary::Models::Requests
{
    BaseAuthenRequest::BaseAuthenRequest(string authkey)
    {
        AuthKey = authkey;
    }

    AuthenAccountRequest::AuthenAccountRequest(string username, string password)
    {
        Username = username;
        Password = password;
    }
    LogoutAccountRequest::LogoutAccountRequest(string username)
    {
        Username = username;
    }
    AccountsRequest::AccountsRequest(string authkey) : BaseAuthenRequest(authkey)
    {
    }
    ActiveAccountsRequest::ActiveAccountsRequest(string authkey) : BaseAuthenRequest(authkey)
    {
    }
    CreateAccountRequest::CreateAccountRequest(string authkey) : BaseAuthenRequest(authkey)
    {
    }
    UpdateAccountRequest::UpdateAccountRequest(string authkey) : BaseAuthenRequest(authkey)
    {
    }
    DeleteAccountRequest::DeleteAccountRequest(string authkey) : BaseAuthenRequest(authkey)
    {
    }
    GroupsRequest::GroupsRequest(string authKey) : BaseAuthenRequest(authKey)
    {
    }
    InviteGroupRequest::InviteGroupRequest(string authKey, unsigned int groupId, vector<string> clients) : BaseAuthenRequest(authKey)
    {
        GroupId = groupId;
        this->Clients = clients;
    }
    JoinGroupRequest::JoinGroupRequest(string authKey, int groupid, string password, string clientname) : BaseAuthenRequest(authKey)
    {
        this->GroupId = groupid;
        this->Password = password;
        this->ClientName = clientname;
    }
    LeaveGroupRequest::LeaveGroupRequest(string authKey, int groupid, string clientname) : BaseAuthenRequest(authKey)
    {
        this->GroupId = groupid;
        this->ClientName = clientname;
    }
    CreateGroupRequest::CreateGroupRequest(string authKey, string name, bool access, string owner, string password) : BaseAuthenRequest(authKey)
    {
        this->Name = name;
        this->Access = access;
        this->Owner = owner;
        this->Password = password;
    }
    DeleteGroupRequest::DeleteGroupRequest(string authKey, unsigned int id, string name, string pass) : BaseAuthenRequest(authKey)
    {
        this->GroupId = id;
        this->clientName = name;
        this->Password = pass;
    }
    ModifyGroupRequest::ModifyGroupRequest(string authKey, unsigned int id, string name, string newname, string newpass, string pass) : BaseAuthenRequest(authKey)
    {
        this->GroupId = id;
        this->clientName = name;
        this->newName = newname;
        this->newPass = newpass;
        this->Password = pass;
    }
    GetRoomRequest::GetRoomRequest(string authkey)
    {
        AuthKey = authkey;
    }
    JoinRoomRequest::JoinRoomRequest(int groupid, string password, string name, string authkey) : BaseAuthenRequest(authkey)
    {
        GroupID = groupid;
        Password = password;
        Name = name;
    }
    InviteRoomRequest::InviteRoomRequest(int groupid, vector<string> username, string authkey) : BaseAuthenRequest(authkey)
    {
        GroupID = groupid;
        Username = username;
    }
    LeaveRoomRequest::LeaveRoomRequest(int groupid, string name, string authkey) : BaseAuthenRequest(authkey)
    {
        GroupID = groupid;
        Name = name;
    }
    CreateRoomRequest::CreateRoomRequest(string roomname, int access, string owner, string password, string authkey) : BaseAuthenRequest(authkey)
    {
        Roomname = roomname;
        Access = access;
        Owner = owner;
        Password = password;
    }
    DeleteRoomRequest::DeleteRoomRequest(int groupid, string username, string password, string authkey): BaseAuthenRequest(authkey)
    {
        GroupID = groupid;
        Username = username;
        Password = password;
    }
    ModifyRoomRequest::ModifyRoomRequest(int groupid, string username, string newname, string newpass, string password, string authkey) : BaseAuthenRequest(authkey)
    {
        GroupID = groupid;
        Username = username;
        Newname = newname;
        Newpass = newpass;
        Password = password;
    }
    RoomMemberRequest::RoomMemberRequest(int groupid, string authkey) : BaseAuthenRequest(authkey)
    {
        GroupID = groupid;
    }
    ReadGroupUserRequest::ReadGroupUserRequest(string authKey, unsigned int id) : BaseAuthenRequest(authKey)
    {
        GroupId = id;
    }
} // namespace MyLibrary::Models::Requests

namespace MyLibrary::Models::Responses
{
    BaseResponse::BaseResponse(RESULT_CODE result)
    {
        Result = result;
    }
    AuthenAccountResponse::AuthenAccountResponse() : BaseResponse(RESULT_CODE::RESULT_UNKNOWN)
    {
        AuthKey = "";
        Role = "";
    }
    AuthenAccountResponse::AuthenAccountResponse(
        RESULT_CODE result, string authkey, string role) : BaseResponse(result)
    {
        AuthKey = authkey;
        Role = role;
    }
    ActiveAccountsResponse::ActiveAccountsResponse() : BaseResponse(RESULT_CODE::RESULT_UNKNOWN)
    {
    }
    ActiveAccountsResponse::ActiveAccountsResponse(
        RESULT_CODE result, vector<string> usernames) : BaseResponse(result)
    {
        Usernames = usernames;
    }
    AccountsResponse::AccountsResponse() : BaseResponse(RESULT_CODE::RESULT_UNKNOWN)
    {
    }
    AccountsResponse::AccountsResponse(
        RESULT_CODE result, vector<Account> accounts) : BaseResponse(result)
    {
        Accounts = accounts;
    }
    GroupsResponse::GroupsResponse(RESULT_CODE result, vector<Groupchat> groups)
    {
        Result = result;
        Groups = groups;
    }
    GroupsResponse::GroupsResponse()
    {
        Result = RESULT_CODE::RESULT_UNKNOWN;
    }
    InviteGroupResponse::InviteGroupResponse()
    {
        Result = RESULT_CODE::RESULT_UNKNOWN;
    }

    InviteGroupResponse::InviteGroupResponse(RESULT_CODE Result, string status)
    {
        this->Result = Result;
        this->Status = Status;
    }
    JoinGroupResponse::JoinGroupResponse()
    {
        Result = RESULT_CODE::RESULT_SUCCESS;
    }
    JoinGroupResponse::JoinGroupResponse(RESULT_CODE result, string status)
    {
        this->Result = result;
        this->Status = status;
    }
    LeaveGroupResponse::LeaveGroupResponse()
    {
        Result = RESULT_CODE::RESULT_SUCCESS;
    }
    LeaveGroupResponse::LeaveGroupResponse(RESULT_CODE result, string status)
    {
        this->Result = result;
        this->Status = status;
    }
    CreateGroupResponse::CreateGroupResponse()
    {
        Result = RESULT_CODE::RESULT_SUCCESS;
    }
    CreateGroupResponse::CreateGroupResponse(RESULT_CODE result, string status)
    {
        this->Result = result;
        this->Status = status;
    }
    DeleteGroupResponse::DeleteGroupResponse(RESULT_CODE result, string status)
    {
        this->Result = result;
        this->Status = status;
    }
    DeleteGroupResponse::DeleteGroupResponse()
    {
        this->Result = RESULT_SUCCESS;
    }
    ModifyGroupResponse::ModifyGroupResponse(RESULT_CODE result, string status)
    {
        this->Result = result;
        this->Status = status;
    }
    ModifyGroupResponse::ModifyGroupResponse()
    {
        this->Result = RESULT_SUCCESS;
    }
    GetRoomResponse::GetRoomResponse()
    {
        //Result = RESULT_CODE::RESULT_UNKNOWN;
    }
    GetRoomResponse::GetRoomResponse(
        RESULT_CODE result, vector<int> access, vector<int> id, vector<string> name, vector<string> owner)
    {
        Access = access;
        //Result = result;
        Id = id;
        Name = name;
        Owner = owner;
    }
    JoinRoomResponse::JoinRoomResponse()
    {
        Result = RESULT_CODE::RESULT_UNKNOWN;
    }
    JoinRoomResponse::JoinRoomResponse(RESULT_CODE result, string status)
    {
        Result = result;
        Status = status;
    }
    InviteRoomResponse::InviteRoomResponse()
    {
        Result = RESULT_CODE::RESULT_UNKNOWN;
    }
    InviteRoomResponse::InviteRoomResponse(RESULT_CODE result, string status)
    {
        Result = result;
        Status = status;
    }
    LeaveRoomResponse::LeaveRoomResponse()
    {
        Result = RESULT_CODE::RESULT_UNKNOWN;
    }
    LeaveRoomResponse::LeaveRoomResponse(RESULT_CODE result, string status)
    {
        Result = result;
        Status = status;
    }
    CreateRoomResponse::CreateRoomResponse()
    {
        Result = RESULT_CODE::RESULT_UNKNOWN;
    }
    CreateRoomResponse::CreateRoomResponse(RESULT_CODE result, string status)
    {
        Result = result;
        Status = status;
    }
    DeleteRoomResponse::DeleteRoomResponse()
    {
        Result = RESULT_CODE::RESULT_UNKNOWN;
    }
    DeleteRoomResponse::DeleteRoomResponse(RESULT_CODE result, string status)
    {
        Result = result;
        Status = status;
    }
    ModifyRoomResponse::ModifyRoomResponse()
    {
        Result = RESULT_CODE::RESULT_UNKNOWN;
    }
    ModifyRoomResponse::ModifyRoomResponse(RESULT_CODE result, string status)
    {
        Result = result;
        Status = status;
    }
    ReadGroupUserResponse::ReadGroupUserResponse()
    {
        Result = RESULT_CODE::RESULT_UNKNOWN;
    }
    ReadGroupUserResponse::ReadGroupUserResponse(RESULT_CODE result, string status)
    {
        Result = result;
        Status = status;
    }
    RoomMemberResponse::RoomMemberResponse()
    {
        Result = RESULT_CODE::RESULT_UNKNOWN;
    }
    RoomMemberResponse::RoomMemberResponse(RESULT_CODE result, string status, vector<string> username)
    {
        Result = result;
        Status = status;
        Username = username;
    }
} // namespace MyLibrary::Models::Responses
