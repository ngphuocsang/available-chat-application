#pragma once

#include "RequestsAndResponses.h"

using namespace MyLibrary::Models::Requests;
using namespace MyLibrary::Models::Responses;
namespace Server::RestServer::Services
{
    class GroupchatService
    {
    public:
        static GroupsResponse getAllGroups(GroupsRequest req);
        static InviteGroupResponse InviteGroup(InviteGroupRequest req);
        static JoinGroupResponse JoinGroup(JoinGroupRequest req);
        static LeaveGroupResponse LeaveGroup(LeaveGroupRequest req);
        static CreateGroupResponse CreateGroup(CreateGroupRequest req);
        static DeleteGroupResponse DeleteGroup(DeleteGroupRequest req);
        static ModifyGroupResponse ModifyGroup(ModifyGroupRequest req);
        static ReadGroupUserResponse ReadGroup(ReadGroupUserRequest req);
    };
} // namespace Server::RestServer::Services