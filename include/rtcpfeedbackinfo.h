//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_RTCPFEEDBACKINFO_H
#define SDPTRANS_RTCPFEEDBACKINFO_H

#include <string>
#include <vector>

namespace sdp {
    struct RtcpFeedbackInfo {
        std::string			id;
        std::vector<std::string>	params;

        RtcpFeedbackInfo() {}
        RtcpFeedbackInfo(const std::string& i, const std::vector<std::string>& ps) : id(i), params(ps) {}
    };
}

#endif //SDPTRANS_RTCPFEEDBACKINFO_H
