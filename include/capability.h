//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_CAPABILITY_H
#define SDPTRANS_CAPABILITY_H


#include <string>
#include <vector>

#include "rtcpfeedbackinfo.h"

namespace sdp {

    struct Capability {
        std::vector<std::string> Codecs;
        bool Rtx;
        std::vector<RtcpFeedbackInfo> Rtcpfbs;
        std::vector<std::string> Extensions;
        bool Simulcast;
    };
}


#endif //SDPTRANS_CAPABILITY_H

