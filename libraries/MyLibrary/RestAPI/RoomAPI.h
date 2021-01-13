#pragma once

#include "RequestsAndResponses.h"
#include "BaseRestAPI.h"

using namespace MyLibrary::Models::Requests;
using namespace MyLibrary::Models::Responses;

namespace Client::RestAPI
{
    class RoomAPI : public BaseRestAPI
    {
    public:
        static GetRoomResponse getRoom(GetRoomRequest req);
        static JoinRoomResponse joinRoom(JoinRoomRequest req);
        static InviteRoomResponse inviteRoom(InviteRoomRequest req);
        static LeaveRoomResponse leaveRoom(LeaveRoomRequest req);
        static CreateRoomResponse createRoom(CreateRoomRequest req);
        static DeleteRoomResponse deleteRoom(DeleteRoomRequest req);
        static ModifyRoomResponse modifyRoom(ModifyRoomRequest req);
        static RoomMemberResponse getRoomMember(RoomMemberRequest req);
    };
}