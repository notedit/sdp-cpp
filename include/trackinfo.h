//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_TRACKINFO_H
#define SDPTRANS_TRACKINFO_H


#include <cwctype>
#include <locale>
#include <string>

#include "stringUtil.h"
#include "sourcegroupinfo.h"
#include "trackencodinginfo.h"

namespace sdp {

    struct TrackInfo {
        std::string					id;
        std::string					mediaID;
        std::string					media;     	// "audio" | "video"
        std::vector<uint64_t>		ssrcs;
        std::vector<SourceGroupInfo>	groups;
        std::vector<std::vector<TrackEncodingInfo> >	encodings;

        TrackInfo() {}
        TrackInfo(const std::string& ID, const std::string& med) :
                id(ID),
                media(med)
        {}

        void AddSSRC(uint64_t ssrc) {
            ssrcs.push_back(ssrc);
        }

        void AddSourceGroup(const SourceGroupInfo& group) {
            groups.push_back(group);
        }

        SourceGroupInfo* GetSourceGroup(const std::string& schematics) {
            for (auto& group : groups) {
                if (stringUtil::tolower(group.semantics) == stringUtil::tolower(schematics)) {
                    return &group;
                }
            }
            return nullptr;
        }

        void AddEncoding(const TrackEncodingInfo& encoding) {
            std::vector<TrackEncodingInfo> ts;
            ts.push_back(encoding);
            encodings.push_back(ts);
        }

        void AddAlternativeEncodings(const std::vector<TrackEncodingInfo>& alternatives) {
            encodings.push_back(alternatives);
        }
    };
}

#endif //SDPTRANS_TRACKINFO_H
