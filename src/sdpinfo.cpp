//
// Created by xiang on 2020/11/30.
//





#include <string>
#include <map>
#include <vector>

#include <time.h>
#include <iostream>


#include "include/sdptransform.h"

using json = nlohmann::json;

#include "include/stringUtil.h"
#include "include/sdpinfo.h"

double defaultValue(double) { return 0; }
int defaultValue(int) { return 0; }
uint64_t defaultValue(const uint64_t) { return 0; }
std::string defaultValue(const std::string&) { return ""; }
template<class T> T omitemptyJson(const json& j, const std::string& name) {
    try {
        return j.at(name).get<T>();
    }
    catch (const std::exception&) {
        return defaultValue(T());
    }
}

bool jsonExist(const json& j, const std::string& name) {
    return j.find(name) != j.end();
}

namespace sdp {

    SDPInfo::SDPInfo() : ice_(nullptr), dtls_(nullptr) {}
    SDPInfo::SDPInfo(ICEInfo* ice, DTLSInfo* dtls, const std::vector<CandidateInfo>& candidates, const std::map<std::string, Capability>& capabilities)
            : candidates_(candidates), ice_(nullptr), dtls_(nullptr)
    {
        SetICE(ice);
        SetDTLS(dtls);

        int dyn = 96;
        for (auto cap : capabilities) {
            MediaInfo media(cap.first, &cap.second);
            for (auto& codec : media.Codecs()) {
                if (codec.second.ctype >= 96) {
                    codec.second.ctype = ++dyn;
                }
                if (codec.second.rtx > 0) {
                    codec.second.rtx = ++dyn;
                }
            }
            AddMedia(media);
        }
    }
    SDPInfo& SDPInfo::operator=(const SDPInfo& sdp) {
        version_ = sdp.GetVersion();
        streams_ = sdp.GetStreams();
        medias_ = sdp.GetMedias();
        candidates_ = sdp.GetCandidates();
        SetICE(this->GetICE());
        SetDTLS(this->GetDTLS());
        return *this;
    }

    SDPInfo::SDPInfo(const SDPInfo& sdp) {
        *this = sdp;
    }
    SDPInfo::~SDPInfo(void) {
        DeleteObj(ice_);
        DeleteObj(dtls_);
    }


    MediaInfo* SDPInfo::GetMedia(const std::string& mtype) {
        for (auto& media : this->medias_) {
            if (stringUtil::tolower(media.GetType()) == stringUtil::tolower(mtype)) {
                return &media;
            }
        }
        return nullptr;
    }
    bool SDPInfo::GetMedia(const std::string& mtype, MediaInfo& m) {
        for (auto& media : this->medias_) {
            if (stringUtil::tolower(media.GetType()) == stringUtil::tolower(mtype)) {
                m = media;
                return true;
            }
        }
        return false;
    }

    MediaInfo * SDPInfo::GetAudioMedia() {
        for (auto& media : this->medias_) {
            if (stringUtil::tolower(media.GetType()) == "audio") {
                return &media;
            }
        }
        return nullptr;
    }

    MediaInfo*  SDPInfo::GetVideoMedia() {
        for (auto& media : this->medias_) {
            if (stringUtil::tolower(media.GetType()) == "video") {
                return &media;
            }
        }
        return nullptr;
    }

    std::vector<MediaInfo> SDPInfo::GetMediasByType(const std::string& mtype)
    {
        std::vector<MediaInfo> medias;
        for (auto& media : this->medias_) {
            if (stringUtil::tolower(media.GetType()) == stringUtil::tolower(mtype)) {
                medias.push_back(media);
            }
        }
        return medias;
    }

    MediaInfo* SDPInfo::GetMediaByID(const std::string& mid) {

        for (auto& media : this->medias_) {
            if (stringUtil::tolower(media.GetID()) == stringUtil::tolower(mid)) {
                return &media;
            }
        }
        return nullptr;
    }

    bool SDPInfo::ReplaceMedia(const MediaInfo& media) {
        for (auto& rmedia : this->medias_) {
            if (rmedia.GetID() == media.GetID()) {
                rmedia = media;
                return true;
            }
        }
        return false;
    }


    void SDPInfo::SetDTLS(const DTLSInfo* const d) {
        utils::setPointerData(this->dtls_, d);
    }

    void SDPInfo::SetICE(const ICEInfo* d) {
        utils::setPointerData(this->ice_, d);
    }


    TrackInfo* SDPInfo::GetTrackByMediaID(const std::string& mid) {
        for (auto& stream : streams_) {
            for (auto& track : stream.second.tracks) {
                if (track.second.mediaID == mid) {
                    return &track.second;
                }
            }
        }
        return nullptr;
    }

    StreamInfo* SDPInfo::GetStreamByMediaID(const std::string& mid) {

        for (auto& stream : streams_) {
            for (auto& track : stream.second.tracks) {
                if (track.second.mediaID == mid) {
                    return &stream.second;
                }
            }
        }
        return nullptr;
    }

    SDPInfo SDPInfo::Answer(const ICEInfo* ice, const DTLSInfo* dtls, const std::vector<CandidateInfo>& candidates, const std::map<std::string, Capability>& medias) {
        SDPInfo sdpInfo;

        sdpInfo.SetICE(ice);
        sdpInfo.SetDTLS(dtls);

        sdpInfo.candidates_ = candidates;

        for (auto media : medias_) {
            std::map<std::string, Capability>::const_iterator it = medias.find(media.GetType());
            if (it != medias.end()) {
                sdpInfo.AddMedia(media.AnswerCapability(it->second));
            }
        }

        return sdpInfo;
    }


    std::string SDPInfo::String() const {
        json sdpMap;
        sdpMap["version"] = 0;

        sdpMap["origin"]["username"] = "-";
        sdpMap["origin"]["sessionId"] = std::to_string(time(nullptr));
        sdpMap["origin"]["sessionVersion"] = this->version_;
        sdpMap["origin"]["netType"] = "IN";
        sdpMap["origin"]["address"] = "127.0.0.1";

        sdpMap["connection"]["version"] = 4;
        sdpMap["connection"]["ip"] = "0.0.0.0";

        sdpMap["name"] = "media";

        if (ice_ != nullptr && ice_->IsLite()) {
            sdpMap["icelite"] = "ice-lite";
        }

        sdpMap["timing"] = { { "start", 0 },{ "stop", 0 } };

        sdpMap["msidSemantic"]["semantic"] = "WMS";
        sdpMap["msidSemantic"]["token"] = "*";

        std::vector<std::string> bundleMids;

        for (auto& media : this->medias_) {
            json mediaMap;
            mediaMap["type"] = media.GetType();
            mediaMap["port"] = 9;
            mediaMap["protocal"] = "UDP/TLS/RTP/SAVP";
            mediaMap["direction"] = Direction_String(media.GetDirection());
            mediaMap["rtcpMux"] = "rtcp-mux";
            mediaMap["rtcpRsize"] = "rtcp-rsize";
            mediaMap["mid"] = media.GetID();

            bundleMids.push_back(media.GetID());

            if (media.GetBitrate() > 0) {
                mediaMap["bandwidth"].push_back({ { "type", "AS" },{ "limit", media.GetBitrate() } });
            }

            mediaMap["candidates"] = GetCandidates();
            mediaMap["iceUfrag"] = this->GetICE()->ufrag;
            mediaMap["icePwd"] = this->GetICE()->password;
            mediaMap["fingerprint"] = { { "type", dtls_->hash },{ "hash", dtls_->fingerprint } };
            mediaMap["setup"] = Setup_String(this->GetDTLS()->setup);


            for (auto& codec : media.GetCodecs()) {
                if ("video" == stringUtil::tolower(media.GetType())) {
                    mediaMap["rtp"].push_back({
                                                      { "payload", codec.second.ctype },
                                                      { "codec",	stringUtil::toupper(codec.second.codec) },
                                                      { "rate",	90000 },
                                                      { "encoding",0 } });
                }
                else {
                    if ("opus" == stringUtil::tolower(codec.second.codec)) {
                        mediaMap["rtp"].push_back({
                                                          { "payload", codec.second.ctype },
                                                          { "codec",	codec.second.codec },
                                                          { "rate",	48000 },
                                                          { "encoding",2 } });
                    }
                    else {
                        mediaMap["rtp"].push_back({
                                                          { "payload", codec.second.ctype },
                                                          { "codec",	codec.second.codec },
                                                          { "rate",	8000 },
                                                          { "encoding",0 } });
                    }
                }

                for (auto& rtcpfb : codec.second.rtcpfbs) {
                    mediaMap["rtcpFb"].push_back({
                                                         { "payload", codec.second.ctype },
                                                         { "type", rtcpfb.id },
                                                         { "subtype", stringUtil::join(rtcpfb.params, " ") } });
                }

                if (codec.second.HasRTX()) {
                    mediaMap["rtp"].push_back({
                                                      { "payload", codec.second.rtx },
                                                      { "codec",	"rtx" },
                                                      { "rate",	90000 },
                                                      { "encoding",0 } });
                    mediaMap["fmtp"].push_back({
                                                       { "payload", codec.second.rtx },
                                                       { "config", "apt=" + std::to_string(codec.second.ctype) } });
                }


                if (!codec.second.params.empty()) {
                    std::string config;

                    for (auto param : codec.second.params) {
                        if (config != "") {
                            config += ";";
                        }

                        // k and value
                        if (param.second != "") {
                            config += param.first + "=" + param.second;
                        }
                        else {
                            config += param.first;
                        }
                    }

                    mediaMap["fmtp"].push_back({
                                                       { "payload", codec.second.ctype },
                                                       { "config", config } });
                }
            }

            std::vector<int> payloads;
            for (auto& rtp : mediaMap["rtp"]) {
                payloads.push_back(rtp["payload"]);
            }
            mediaMap["payloads"] = stringUtil::digitArrayToString(payloads, " ");

            for (auto ext : media.GetExtensions()) {
                mediaMap["ext"].push_back({ { "value", ext.first },{ "uri", ext.second } });
            }

            mediaMap["rids"] = media.GetRIDS();

            const SimulcastInfo* simulcast = media.GetSimulcastInfo();
            if (simulcast != nullptr) {
                int index = 1;

                const SimulcastStreamInfoss& sendStreams = simulcast->GetSimulcastStreams(SEND);
                const SimulcastStreamInfoss& recvStreams = simulcast->GetSimulcastStreams(RECV);

                if (!sendStreams.empty()) {
                    std::string list = "";
                    for (auto& stream : sendStreams) {
                        std::string alternatives;
                        for (auto& item : stream) {
                            if (alternatives == "") {
                                alternatives += item.IsPaused() ? "~" : "" + item.id;
                            }
                            else {
                                alternatives += "," + item.IsPaused() ? "~" : "" + item.id;
                            }
                        }

                        list += (list == "") ? "" : ";" + alternatives;
                    }

                    mediaMap["simulcast"]["dir1"] = "send";
                    mediaMap["simulcast"]["list1"] = list;
                    ++index;
                }

                if (!recvStreams.empty()) {
                    std::string list;
                    for (auto& stream : recvStreams) {
                        std::string alternatives;
                        for (auto& item : stream) {
                            if (alternatives == "") {
                                alternatives += item.IsPaused() ? "~" : "" + item.id;
                            }
                            else {
                                alternatives = "," + item.IsPaused() ? "~" : "" + item.id;
                            }
                        }

                        list += (list == "") ? "" : ";" + alternatives;
                    }

                    if (index == 1) {
                        mediaMap["simulcast"]["dir1"] = "recv";
                        mediaMap["simulcast"]["list1"] = list;
                    }
                    else if (index == 2) {
                        mediaMap["simulcast"]["dir2"] = "recv";
                        mediaMap["simulcast"]["list2"] = list;
                    }
                }
            }

            sdpMap["media"].push_back(mediaMap);
        }

        // streams
        for (auto& stream : this->GetStreams()) {
            for (auto& track : stream.second.tracks) {
                for (auto& md : sdpMap["media"]) {
                    // check if it is unified or plan b
                    if (track.second.mediaID != "") {
                        if (track.second.mediaID == md["mid"]) {
                            for (auto& group : track.second.groups) {
                                md["ssrcGroups"].push_back({
                                                                   { "semantics", group.semantics },
                                                                   { "ssrcs"	, stringUtil::digitArrayToString(group.ssrcs, " ") } });
                            }

                            for (auto& ssrc : track.second.ssrcs) {
                                md["ssrcs"].push_back({
                                                              { "id"		, ssrc },
                                                              { "attribute", "cname" },
                                                              { "value"	, stream.second.id } });
                                md["ssrcs"].push_back({
                                                              { "id"		,ssrc },
                                                              { "attribute","msid" },
                                                              { "value"	,stream.second.id + " " + track.second.id } });
                            }
                            md["msid"] = stream.second.id + " " + track.second.id;

                            break;
                        }
                    }
                    else if (stringUtil::tolower(md["type"]) == stringUtil::tolower(track.second.media)) {
                        for (auto& group : track.second.groups) {
                            md["ssrcGroups"].push_back({
                                                               { "semantics", group.semantics },
                                                               { "ssrcs"	, stringUtil::digitArrayToString(group.ssrcs, " ") } });
                        }

                        for (auto& ssrc : track.second.ssrcs) {
                            md["ssrcs"].push_back({
                                                          { "id"		,ssrc },
                                                          { "attribute","cname" },
                                                          { "value"	,stream.second.id } });
                            md["ssrcs"].push_back({
                                                          { "id"		,ssrc },
                                                          { "attribute","msid" },
                                                          { "value"	,stream.second.id + " " + track.second.id } });
                        }
                        break;
                    }
                }
            }
        }

        sdpMap["groups"].push_back({
                                           { "mids", stringUtil::join(bundleMids, " ") },
                                           { "type", "BUNDLE" } });

        json j = sdpMap;
        return sdptransform::write(j);
    }


#if 0
    // Unify return an unified plan version of the SDP info;  ......
	func(s *SDPInfo) Unify() *SDPInfo {
	cloned: = NewSDPInfo()

		cloned.version = this->version

		for _, media : = range this->medias {
			cloned.AddMedia(media.Clone())
		}

		medias: = map[string][] * MediaInfo{
			"audio": cloned.GetMediasByType("audio"),
			"video" : cloned.GetMediasByType("video"),
		}

		for _, stream : = range this->streams {
		clonedStream: = stream.Clone()
			for _, clonedTrack : = range clonedStream.GetTracks() {
				var clonedMedia *MediaInfo
					if len(medias[clonedTrack.GetMedia()]) == 0 {
					media: = this->GetMedia(clonedTrack.GetMedia())
						clonedMedia = media.Clone()
						clonedMedia.SetID(clonedTrack.GetID())
						cloned.AddMedia(clonedMedia)
					}
					else {
					mediaList: = medias[clonedTrack.GetMedia()]
						clonedMedia = mediaList[len(mediaList) - 1]
						mediaList = mediaList[:len(mediaList) - 1]
						medias[clonedTrack.GetMedia()] = mediaList
					}
					clonedTrack.SetMediaID(clonedMedia.GetID())
			}
					  cloned.AddStream(clonedStream)
		}

				for _, candidate : = range this->GetCandidates() {
					cloned.AddCandidate(candidate.Clone())
				}

				cloned.SetICE(this->GetICE().Clone())
					cloned.SetDTLS(this->GetDTLS().Clone())

					return cloned
	}
#endif


    bool SDPInfo::Parse(const std::string& sdp) {
        json sdpMap;
        try {
            sdpMap = sdptransform::parse(sdp);
        }
        catch (const std::exception& e)
        {
            std::cout << std::string("Exception: ") + e.what() + "\r\n";
            std::cout << sdptransform::parse(sdp).dump() + "\r\n";
            return false;
        }

        this->SetVersion(omitemptyJson<int>(sdpMap, "version"));

        for (auto& md : sdpMap.at("media")) {
            std::string media = omitemptyJson<std::string>(md, "type");
            std::string mid = omitemptyJson<std::string>(md, "mid");

            MediaInfo mediaInfo(mid, media);

            ICEInfo ii(omitemptyJson<std::string>(md, "iceUfrag"), omitemptyJson<std::string>(md, "icePwd"));
            this->SetICE(&ii);
            if (jsonExist(md, "candidates")) {
                this->candidates_ = md["candidates"].get<std::vector<CandidateInfo> >();
            }
            std::string fp_type, fp_hash;
            if (sdpMap.find("fingerprint") != sdpMap.end()) {
                fp_type = omitemptyJson<std::string>(sdpMap["fingerprint"], "type");
                fp_hash = omitemptyJson<std::string>(sdpMap["fingerprint"], "hash");
            }
            if (jsonExist(md, "fingerprint")) {
                fp_type = omitemptyJson<std::string>(md["fingerprint"], "type");
                fp_hash = omitemptyJson<std::string>(md["fingerprint"], "hash");
            }
            std::string setup = (md.find("setup") != md.end()) ? omitemptyJson<std::string>(md, "Setup") : "";
            setup = setup.empty() ? SETUPACTPASS : Setup_ByValue(setup);
            DTLSInfo dtls({ setup, fp_type, fp_hash });
            this->SetDTLS(&dtls);

            std::string direction = (md.find("direction") != md.end()) ? md.at("direction") : "";
            mediaInfo.SetDirection(direction.empty() ? SENDRECV : Direction_ByValue(direction));

            std::map<int, int> apts;
            if (md.find("rtp") != md.end()) {
                for (auto& fmt : md.at("rtp")) {
                    int payload = fmt.at("payload");
                    std::string codec = omitemptyJson<std::string>(fmt, "codec");
                    if ("RED" == stringUtil::toupper(codec) || "ULPFEC" == stringUtil::toupper(codec)) {
                        continue;
                    }

                    std::map<std::string, std::string> params;
                    if (md.find("fmtp") != md.end()) {
                        for (auto& fmtp : md.at("fmtp")) {
                            if (fmtp.at("payload") == payload) {
                                if (jsonExist(fmtp, "config")) {
                                    for (auto& kv : stringUtil::split(fmtp.at("config"), ";")) {
                                        std::vector<std::string> param = stringUtil::split(kv, "=");
                                        if (param.size() < 2) {
                                            continue;
                                        }
                                        params[param[0]] = param[1];
                                    }
                                }
                            }
                        }
                    }

                    if ("rtx" == stringUtil::tolower(codec)) {
                        if (params.find("apt") != params.end()) {
                            try {
                                apts[std::atoi(params.at("apt").c_str())] = payload;
                            }
                            catch (...) {
                            }
                        }
                    }
                    else {
                        CodecInfo codecInfo(codec, payload);
                        codecInfo.AddParams(params);
                        mediaInfo.AddCodec(codecInfo);
                    }
                }
            }

            // rtx
            for (auto pt : apts) {
                CodecInfo* codecInfo = mediaInfo.CodecForType(pt.first);
                if (codecInfo != nullptr) {
                    codecInfo->rtx = pt.second;
                }
            }

            // rtcpFb
            if (md.find("rtcpFb") != md.end()) {
                for (auto& rtcfb : md.at("rtcpFb")) {
                    CodecInfo* codecInfo = mediaInfo.CodecForType(rtcfb.at("payload"));
                    if (codecInfo != nullptr) {
                        std::vector<std::string> params;
                        if (rtcfb.find("subtype") != rtcfb.end()) {
                            params = stringUtil::split(rtcfb.at("subtype"), " ");
                        }
                        codecInfo->AddRTCPFeedback(RtcpFeedbackInfo(omitemptyJson<std::string>(rtcfb, "type"), params));
                    }
                }
            }

            // extmap
            if (jsonExist(md, "ext")) {
                for (auto& extmap : md.at("ext")) {
                    mediaInfo.AddExtension(extmap.at("value"), extmap.at("uri"));
                }
            }

            if (jsonExist(md, "rids")) {
                for (auto i = md["rids"].begin(); i != md["rids"].end(); ++i) {
                    mediaInfo.AddRID(i.value().get<RIDInfo>());
                }
            }

            std::vector<std::vector<TrackEncodingInfo> > encodings;
            if (md.find("simulcast") != md.end()) {
                SimulcastInfo simulcast;

                if (jsonExist(md.at("simulcast"), "dir1")) {
                    std::vector<std::vector<SimulcastStreamInfo>> tei =
                            sdptransform::parseSimulcastStreamList(md.at("simulcast").at("list1")).get<std::vector<std::vector<SimulcastStreamInfo> > >();
                    for (auto& streams : tei) {
                        simulcast.AddSimulcastAlternativeStreams(DirectionWay_ByValue(md.at("simulcast").at("dir1")), streams);
                    }
                }

                if (jsonExist(md.at("simulcast"), "dir2")) {
                    std::vector<std::vector<SimulcastStreamInfo>> tei =
                            sdptransform::parseSimulcastStreamList(md.at("simulcast").at("list2")).get<std::vector<std::vector<SimulcastStreamInfo> > >();
                    for (auto& streams : tei) {
                        simulcast.AddSimulcastAlternativeStreams(DirectionWay_ByValue(md.at("simulcast").at("dir2")), streams);
                    }
                }

                // For all sending encodings
                for (auto& streams : simulcast.GetSimulcastStreams(SEND)) {
                    std::vector<TrackEncodingInfo> alternatives;
                    for (auto& stream : streams) {
                        TrackEncodingInfo encoding(stream.id, stream.IsPaused());
                        RIDInfo* ridInfo = mediaInfo.GetRID(encoding.id);
                        if (ridInfo != nullptr) {
                            //Get associated payloads
                            for (auto format : ridInfo->formats) {
                                const CodecInfo* codecInfo = mediaInfo.GetCodecForType(format);
                                if (codecInfo != nullptr) {
                                    encoding.AddCodec(*codecInfo);
                                }
                            }
                            encoding.params = ridInfo->params;
                            alternatives.push_back(encoding);
                        }
                    }

                    if (!alternatives.empty()) {
                        encodings.push_back(alternatives);
                    }
                }

                mediaInfo.SetSimulcastInfo(&simulcast);
            }

            std::map<uint64_t, SourceInfo> sources;
            if (md.find("ssrcs") != md.end()) {
                for (auto& ssrcAttr : md.at("ssrcs")) {
                    std::string key = omitemptyJson<std::string>(ssrcAttr, "attribute");
                    uint64_t ssrc = omitemptyJson<uint64_t>(ssrcAttr, "id");
                    std::string value = omitemptyJson<std::string>(ssrcAttr, "value");

                    if (sources.find(ssrc) == sources.end()) {
                        sources[ssrc] = SourceInfo(ssrc);
                    }
                    SourceInfo* source = &sources[ssrc];

                    if (stringUtil::tolower(key) == "cname") {
                        source->cname = value;
                    }
                    else if (stringUtil::tolower(key) == "msid") {
                        std::vector<std::string> ids = stringUtil::split(value, " ");
                        // get stream id and track id
                        std::string& streamId = ids[0];
                        std::string& trackId = ids[1];

                        source->streamID = streamId;
                        source->trackID = trackId;

                        auto* stream = this->GetStream(streamId);
                        if (stream == nullptr) {
                            this->AddStream(StreamInfo(streamId));
                            stream = this->GetStream(streamId);
                            assert(stream != nullptr);
                        }

                        TrackInfo* track = stream->GetTrack(trackId);
                        if (track == nullptr) {
                            stream->AddTrack(TrackInfo(trackId, media));
                            track = stream->GetTrack(trackId);
                            assert(track != nullptr);
                            track->mediaID = mid;
                            track->encodings = encodings;
                        }
                        // Add ssrc
                        track->AddSSRC(ssrc);
                    }
                }
            }

            // Check if ther is a global msid
            // Why this?
            if (md.find("msid") != md.end()) {
                std::vector<std::string> ids = stringUtil::split(md.at("msid"), " ");
                std::string& streamId = ids[0];
                std::string& trackId = ids[1];

                StreamInfo* stream = this->GetStream(streamId);
                if (stream == nullptr) {
                    this->AddStream(StreamInfo(streamId));
                    stream = this->GetStream(streamId);
                }

                TrackInfo* track = stream->GetTrack(trackId);
                if (track == nullptr) {
                    stream->AddTrack(TrackInfo(trackId, media));
                    track = stream->GetTrack(trackId);
                    track->media = mid;
                    track->encodings = encodings;
                }

                for (auto s : sources) {
                    if (s.second.streamID == "") {
                        s.second.streamID = streamId;
                        s.second.trackID = trackId;
                        track->AddSSRC(s.first);
                    }
                }
            }

            for (auto s : sources) {
                auto& ssrc = s.first;
                auto& source = s.second;

                if (source.streamID == "") {
                    std::string streamId = source.cname;
                    std::string trackId = mid;

                    source.streamID = streamId;
                    source.trackID = trackId;

                    StreamInfo* stream = this->GetStream(streamId);
                    if (stream == nullptr) {
                        this->AddStream(StreamInfo(streamId));
                        stream = this->GetStream(streamId);
                    }

                    TrackInfo* track = stream->GetTrack(trackId);
                    if (track == nullptr) {
                        stream->AddTrack(TrackInfo(trackId, media));
                        track = stream->GetTrack(trackId);
                        track->mediaID = mid;
                        track->encodings = encodings;
                    }
                    track->AddSSRC(ssrc);
                }
            }

            if (md.find("ssrcGroups") != md.end()) {
                for (auto& ssrcGroupAttr : md.at("ssrcGroups")) {
                    std::vector<std::string> ssrcs = stringUtil::split(omitemptyJson<std::string>(ssrcGroupAttr, "ssrcs"), " ");
                    std::vector<int> ssrcsint;
                    for (auto& ssrcstr : ssrcs) {
                        try {
                            ssrcsint.push_back(std::atoi(ssrcstr.c_str()));
                        }
                        catch (...) {}
                    }
                    if (ssrcsint.empty()) {
                        continue;
                    }

                    auto& source = sources[ssrcsint[0]];
                    StreamInfo* streamInfo = this->GetStream(source.streamID);
                    if (streamInfo != nullptr && streamInfo->GetTrack(source.trackID) != nullptr) {
                        streamInfo->GetTrack(source.trackID)->AddSourceGroup(SourceGroupInfo(omitemptyJson<std::string>(ssrcGroupAttr, "semantics"), ssrcsint));
                    }
                }
            }

            this->AddMedia(mediaInfo);
        }

        return true;
    }

}

