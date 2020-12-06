//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_CODECINFO_H
#define SDPTRANS_CODECINFO_H

#include <string>
#include <map>
#include <vector>

#include "stringUtil.h"
#include "rtcpfeedbackinfo.h"


namespace sdp {

    struct CodecInfo {
        std::string				codec;
        int					ctype;
        int					rtx;
        std::map<std::string, std::string>	params;
        std::vector<RtcpFeedbackInfo>	rtcpfbs;

        CodecInfo() {}
        CodecInfo(const std::string& cc, int ct) : codec(cc), ctype(ct), rtx(0) {}


        bool HasParam(const std::string& key) const {
            return params.find(key) != params.end();
        }

        std::string GetParam(const std::string& key) const {
            auto it = params.find(key);
            return it == params.end() ? "" : it->second;
        }

        void AddParam(const std::string& key, const std::string& value) {
            params[key] = value;
        }

        void AddParams(const std::map<std::string, std::string>& ps) {
            for (auto p : ps) {
                this->params[p.first] = p.second;
            }
        }

        bool HasRTX() const {
            return this->rtx != 0;
        }

        void AddRTCPFeedback(const RtcpFeedbackInfo& rtcpfb) {
            this->rtcpfbs.push_back(rtcpfb);
        }

    };

    static std::map<std::string, CodecInfo> CodecMapFromNames(const std::vector<std::string>& names, bool rtx, const std::vector<RtcpFeedbackInfo>& rtcpFbs)
    {
        std::map<std::string, CodecInfo> codecInfos;
        int basePt = 96;

        for (auto& name : names) {
            int pt;
            std::vector<std::string> params = stringUtil::split(name, ";");
            std::string codecName = stringUtil::trim(stringUtil::tolower(params[0]));
            if (codecName == "pcmu") {
                pt = 0;
            }
            else if (codecName == "pcma") {
                pt = 8;
            }
            else {
                basePt++;
                pt = basePt;
            }

            CodecInfo codecInfo(codecName, pt);
            if (rtx && codecName != "ulpfec" && codecName != "flexfec-03" && codecName != "red") {
                basePt++;
                codecInfo.rtx = basePt;
            }

            for (auto& rtcpfb : rtcpFbs) {
                codecInfo.AddRTCPFeedback(rtcpfb);
            }


            if (params.size() > 1) {
                params.erase(params.begin());
                for (auto& param : params) {
                    std::vector<std::string> values = stringUtil::split(param, "=");
                    if (values.size() < 2) {
                        continue;
                    }
                    codecInfo.AddParam(values[0], values[1]);
                }
            }

            codecInfos[codecName] = codecInfo;
        }

        return codecInfos;
    }
}


#endif //SDPTRANS_CODECINFO_H
