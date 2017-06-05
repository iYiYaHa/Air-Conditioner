
//
// Air Conditioner - Server MVC Model
// BOT Man, 2017
//

#ifndef AC_SERVER_MODEL_H
#define AC_SERVER_MODEL_H

#include <chrono>
#include <unordered_map>

#include "../common/common-model.h"

namespace Air_Conditioner
{
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

    struct LogOnOff
    {
        RoomId room;
        TimePoint timeBeg, timeEnd;
    };

    struct LogRequest
    {
        RoomId room;
        Wind wind;
        Temperature tempBeg, tempEnd;
        TimePoint timeBeg, timeEnd;
        Cost cost;
    };

    struct ClientState
    {
        GuestId guest;
        Temperature curTemp;
        Temperature targetTemp;
        Wind wind;
        Energy energy;
        Cost cost;
    };

    using ClientList = std::unordered_map<RoomId, ClientState>;
}

#endif AC_SERVER_MODEL_H