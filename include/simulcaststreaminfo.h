//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_SIMULCASTSTREAMINFO_H
#define SDPTRANS_SIMULCASTSTREAMINFO_H


#include <string>

#include "json.hpp"

namespace sdp {

    struct SimulcastStreamInfo {
        std::string	id;
        bool	paused;

        SimulcastStreamInfo() {}
        SimulcastStreamInfo(const std::string ID, bool p) : id(ID), paused(p) {}

        bool IsPaused() const {
            return paused;
        }
    };

    static void to_json(nlohmann::json& j, const SimulcastStreamInfo& d) {
        j = nlohmann::json{ {"scid", d.id},	{"paused", d.paused} };
    }
    static void from_json(const nlohmann::json& j, SimulcastStreamInfo& d) {
        d.id = j.at("scid").get<std::string>();
        d.paused = j.at("paused").get<bool>();
    }
}

#endif //SDPTRANS_SIMULCASTSTREAMINFO_H
