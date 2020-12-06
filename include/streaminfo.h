//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_STREAMINFO_H
#define SDPTRANS_STREAMINFO_H


#include <string>

#include "stringUtil.h"
#include "trackinfo.h"

namespace sdp {


    struct StreamInfo {
        std::string					id;
        std::map<std::string, TrackInfo>	tracks;

        StreamInfo() {}
        StreamInfo(const std::string& streamID) : id(streamID) {}

        void AddTrack(const TrackInfo& track) {
            tracks[track.id] = track;
        }

        void RemoveTrack(const TrackInfo& track) {
            tracks.erase(track.id);
        }

        void RemoveTrackById(const std::string& trackId) {
            tracks.erase(trackId);
        }

        TrackInfo* GetFirstTrack(const std::string& media) {
            for (auto& trak : tracks) {
                if (stringUtil::tolower(trak.second.media) == stringUtil::tolower(media)) {
                    return &trak.second;
                }
            }
            return nullptr;
        }

        void RemoveAllTracks() {
            tracks.clear();
        }

        bool GetTrack(const std::string& trackId, TrackInfo& t) {
            std::map<std::string, TrackInfo>::iterator it = tracks.find(trackId);
            if (it == tracks.end())
                return false;
            t = it->second;
            return true;
        }
        TrackInfo* GetTrack(const std::string& trackId) {
            std::map<std::string, TrackInfo>::iterator it = tracks.find(trackId);
            return it == tracks.end() ? nullptr : &it->second;
        }
    };

}

#endif //SDPTRANS_STREAMINFO_H
