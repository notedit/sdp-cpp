//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_SDPINFO_H
#define SDPTRANS_SDPINFO_H


#include <string>
#include <map>
#include <vector>

#include <time.h>
#include <iostream>

#include "streaminfo.h"
#include "mediainfo.h"
#include "candidate.h"
#include "sourceinfo.h"
#include "iceinfo.h"
#include "dtlsinfo.h"

namespace sdp {
    typedef	 std::vector<CandidateInfo> CandidateInfos;

    class SDPInfo {
        int						version_ = 1;
        std::map<std::string, StreamInfo>	streams_;
        std::vector<MediaInfo>		medias_;			// as we need to keep order
        CandidateInfos	candidates_; 	// keep order
        ICEInfo*				ice_ = nullptr;
        DTLSInfo*				dtls_ = nullptr;

    public:
        SDPInfo();
        SDPInfo(ICEInfo* ice, DTLSInfo* dtls, const CandidateInfos& candidates, const std::map<std::string, Capability>& capabilities);
        SDPInfo& operator=(const SDPInfo& sdp);
        SDPInfo(const SDPInfo& sdp);
        ~SDPInfo(void);

        void SetVersion(int v) {
            version_ = v;
        }

        void AddMedia(const MediaInfo& media) {
            this->medias_.push_back(media);
        }

        MediaInfo* GetMedia(const std::string& mtype);
        bool GetMedia(const std::string& mtype, MediaInfo& m);

        MediaInfo * GetAudioMedia();

        MediaInfo*  GetVideoMedia();

        std::vector<MediaInfo> GetMediasByType(const std::string& mtype);

        MediaInfo* GetMediaByID(const std::string& mid);

        bool ReplaceMedia(const MediaInfo& media);

        const std::vector<MediaInfo>& GetMedias() const {
            return this->medias_;
        }

        int GetVersion() const {
            return this->version_;
        }

        const DTLSInfo* GetDTLS() const {
            return this->dtls_;
        }

        void SetDTLS(const DTLSInfo* const d);

        const ICEInfo* GetICE() const {
            return this->ice_;
        }

        void SetICE(const ICEInfo* d);

        void AddCandidate(const CandidateInfo& candi) {
            // todo check
            this->candidates_.push_back(candi);
        }

        void AddCandidates(const CandidateInfos& candidates) {
            for (auto& candidate : candidates) {
                this->AddCandidate(candidate);
            }
        }

        const CandidateInfos& GetCandidates() const {
            return this->candidates_;
        }

        StreamInfo* GetStream(const std::string& id) {
            auto it = streams_.find(id);
            return it == streams_.end() ? nullptr : &it->second;
        }

        const std::map<std::string, StreamInfo>& GetStreams() const {
            return this->streams_;
        }

        StreamInfo* GetFirstStream() {
            return streams_.empty() ? nullptr : &streams_.begin()->second;
        }

        void AddStream(const StreamInfo& stream) {
            this->streams_[stream.id] = stream;
        }

        void RemoveStream(const StreamInfo& stream) {
            streams_.erase(stream.id);
        }

        void RemoveAllStreams() {
            streams_.clear();
        }

        TrackInfo* GetTrackByMediaID(const std::string& mid);

        StreamInfo* GetStreamByMediaID(const std::string& mid);

        SDPInfo Answer(const ICEInfo* ice, const DTLSInfo* dtls, const CandidateInfos& candidates, const std::map<std::string, Capability>& medias);

        std::string String() const;

        bool Parse(const std::string& sdp);

    };
}


#endif //SDPTRANS_SDPINFO_H
