//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_DIRECTIONWAY_H
#define SDPTRANS_DIRECTIONWAY_H


#include <string>

namespace sdp {

    typedef uint8_t DirectionWay;

    static const DirectionWay SEND = 1;
    static const DirectionWay RECV = 2;

    inline DirectionWay DirectionWay_ByValue(const std::string& d) {
        if (d == "recv")
            return RECV;
        else if (d == "send")
            return SEND;
        else
            return 0;
    }

    inline DirectionWay DirectionWay_Reverse(DirectionWay d) {
        switch (d) {
            case SEND:
                return RECV;
            case RECV:
                return SEND;
            default:
                return 0;
        }
    }

    inline std::string DirectionWay_String(DirectionWay d) {
        switch (d) {
            case SEND:
                return "send";
            case RECV:
                return "recv";
            default:
                return "";
        }
    }

}


#endif //SDPTRANS_DIRECTIONWAY_H
