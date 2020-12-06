//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_CANDIDATE_H
#define SDPTRANS_CANDIDATE_H


#include "jsonserialize.h"
#include <string>

namespace sdp {

#define mem_def_CandidateInfo(op)\
	mem_def_(op, std::string,	foundation)\
	mem_def_(op, int,		component)\
	mem_def_(op, std::string,	transport)\
	mem_def_(op, int,		priority)\
	mem_def_(op, std::string,	ip)\
	mem_def_(op, int,		port)\
	mem_def_(op, std::string,	type)\
	mem_def_(op, std::string, 	raddr)\
	mem_def_(op, int, 		rport)

    struct CandidateInfo {
//        std::string foundation;
//        int component;
//        std::string transport;
//        int priority;
//        std::string ip;
//        int port;
//        std::string type;
//        std::string raddr;
//        int rport;

        mem_def_CandidateInfo(def)

        CandidateInfo() :
                component(0),
                priority(0),
                port(0),
                rport(0) {}

        CandidateInfo(const std::string &found, int compID, const std::string &transport,
                      int prio, const std::string &addr, int port, const std::string &ct, const std::string &rAddr, int rPort)
                : foundation(found),
                  component(compID),
                  transport(transport),
                  priority(prio),
                  ip(addr),
                  port(port),
                  type(ct),
                  raddr(rAddr),
                  rport(rPort) {}
    };
    serialize_def(CandidateInfo)
}


#endif //SDPTRANS_CANDIDATE_H
