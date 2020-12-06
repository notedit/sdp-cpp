//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_SOURCEINFO_H
#define SDPTRANS_SOURCEINFO_H


#include <string>

namespace sdp {

    struct SourceInfo {
        uint64_t	ssrc;
        std::string		cname;
        std::string		streamID;
        std::string		trackID;

        SourceInfo() : ssrc(0) {}
        SourceInfo(uint64_t ss) : ssrc(ss) {}
    };

}

#endif //SDPTRANS_SOURCEINFO_H
