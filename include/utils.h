//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_UTILS_H
#define SDPTRANS_UTILS_H


#include <string>

namespace utils {

#define DeleteObj(obj)  {delete obj; obj = nullptr;}

    template<class T> void setPointerData(T*& dest, const T* src) {
        if (src != nullptr) {
            if (dest == nullptr) {
                dest = new T;
            }
            *dest = *src;
        }
        else {
            if (dest != nullptr) {
                delete dest;
                dest = nullptr;
            }
        }
    }

}


#endif //SDPTRANS_UTILS_H
