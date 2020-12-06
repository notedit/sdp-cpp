//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_TRACKENODINGINFO_H
#define SDPTRANS_TRACKENODINGINFO_H

#include <string>

#include "codecinfo.h"

namespace sdp {

    struct TrackEncodingInfo {
        std::string					id;
        bool					paused = false;
        std::map<std::string, CodecInfo>	codecs;
        std::map<std::string, std::string>		params;

        TrackEncodingInfo() {}
        TrackEncodingInfo(const std::string& Id, bool pause)
                : id(Id), paused(pause)
        {}

        void AddCodec(const CodecInfo& codec) {
            codecs[codec.codec] = codec;
        }

        void AddParam(const std::string& id, const std::string& param) {
            params[id] = param;
        }

        bool IsPaused() {
            return paused;
        }
    };

}

#endif //SDPTRANS_TRACKENODINGINFO_H
