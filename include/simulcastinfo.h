//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_SIMULCASTINFO_H
#define SDPTRANS_SIMULCASTINFO_H


#include <string>

#include "simulcaststreaminfo.h"
#include "directionway.h"

namespace sdp {

    typedef std::vector<SimulcastStreamInfo>		SimulcastStreamInfos;
    typedef std::vector<SimulcastStreamInfos>	SimulcastStreamInfoss;

    struct SimulcastInfo {
        SimulcastStreamInfoss send;
        SimulcastStreamInfoss recv;

        void AddSimulcastAlternativeStreams(DirectionWay direction, SimulcastStreamInfos streams) {
            if (direction == SEND) {
                send.push_back(streams);
            }
            else {
                recv.push_back(streams);
            }
        }

        void AddSimulcastStream(DirectionWay direction, const SimulcastStreamInfo& stream) {
            SimulcastStreamInfos is;
            is.push_back(stream);
            AddSimulcastAlternativeStreams(direction, is);
        }

        const SimulcastStreamInfoss& GetSimulcastStreams(DirectionWay direction) const {
            return direction == SEND ? send : recv;
        }

    };

}

#endif //SDPTRANS_SIMULCASTINFO_H
