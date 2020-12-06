//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_DIRECTION_H
#define SDPTRANS_DIRECTION_H


#include <string>

namespace sdp {

    typedef  uint8_t Direction;

    enum {
        SENDRECV = 1,
        SENDONLY = 2,
        RECVONLY = 3,
        INACTIVE = 4
    };

    static Direction Direction_Reverse(Direction d) {
        switch (d) {
            case SENDRECV:
                return SENDRECV;
            case SENDONLY:
                return RECVONLY;
            case RECVONLY:
                return SENDONLY;
            case INACTIVE:
                return INACTIVE;
            default:
                return 0;
        }
    }

    static std::string Direction_String(Direction d) {
        switch (d) {
            case SENDRECV:
                return "sendrecv";
            case SENDONLY:
                return "sendonly";
            case RECVONLY:
                return "recvonly";
            case INACTIVE:
                return "inactive";
            default:
                return "";
        }
    }

    static Direction Direction_ByValue(const std::string& direction) {
        if (direction == "sendrecv") {
            return SENDRECV;
        }
        else if (direction == "sendonly") {
            return SENDONLY;
        }
        else if (direction == "recvonly") {
            return RECVONLY;
        }
        else if (direction == "inactive") {
            return INACTIVE;
        }

        return 0;
    }
}



#endif //SDPTRANS_DIRECTION_H
