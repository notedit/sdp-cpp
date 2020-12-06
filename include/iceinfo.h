//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_ICEINFO_H
#define SDPTRANS_ICEINFO_H


#include <stdlib.h>
#include <string>

namespace sdp {

    struct ICEInfo {
        std::string	ufrag;
        std::string	password;
        bool	lite = false;
        bool	endOfCandidates = false;

        ICEInfo() {}
        ICEInfo(const std::string& uf, const std::string& pass, bool l, bool can) :
                ufrag(uf),
                password(pass),
                lite(l),
                endOfCandidates(can)
        {}
        ICEInfo(const std::string& uf, const std::string& pass) :
                ufrag(uf),
                password(pass),
                lite(false),
                endOfCandidates(false)
        {}

        static ICEInfo* Generate(bool lite) {
            return new ICEInfo({
                                       std::to_string(rand()),
                                       std::to_string(rand()),
                                       lite,
                                       false });
        }

        bool IsLite() {
            return this->lite;
        }

        bool IsEndOfCandidate() {
            return this->endOfCandidates;
        }
    };
}

#endif //SDPTRANS_ICEINFO_H
