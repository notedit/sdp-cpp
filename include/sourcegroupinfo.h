//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_SOURCEGROUPINFO_H
#define SDPTRANS_SOURCEGROUPINFO_H


#include <string>
#include <vector>

namespace sdp {

    struct SourceGroupInfo {
        std::string			semantics;
        std::vector<int>		ssrcs;

        SourceGroupInfo() {}
        SourceGroupInfo(const std::string& sema, const std::vector<int>& ss) :
                semantics(sema),
                ssrcs(ss)
        {}
    };

}

#endif //SDPTRANS_SOURCEGROUPINFO_H
