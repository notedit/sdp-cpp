//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_DTLSINFO_H
#define SDPTRANS_DTLSINFO_H


#include <string>
#include "setup.h"

namespace sdp {

    struct DTLSInfo {
        Setup       setup;
        std::string      hash;
        std::string      fingerprint;
    };
}


#endif //SDPTRANS_DTLSINFO_H
