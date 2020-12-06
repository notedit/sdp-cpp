//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_JSONSERIALIZE_H
#define SDPTRANS_JSONSERIALIZE_H

#include "json.hpp"


#define mem_def_(op, type, mem)				mem_def_##op(type, mem, #mem)
#define mem_def(op, type, mem, name)		mem_def_##op(type, mem, name)
#define mem_def_def(type, mem, name)		type mem;
#define mem_def_copy(type, mem, name)		mem = other.mem;
#define mem_def_init(type, mem, name)
#define mem_def_delete(type, mem, name)
#define mem_def_from(type, mem, name)		try {d.mem = j.at(name).get<type>();} catch (const std::exception& ) {}
#define mem_def_to(type, mem, name)			j[name] = d.mem;

#define mem_ptr_def(op, type, mem, name)	mem_ptr_def_##op(type, mem, name)
#define mem_ptr_def_def(type, mem, name)	type* mem;
#define mem_ptr_def_copy(type, mem, name)	utils::setPointerData(mem, other.mem);
#define mem_ptr_def_init(type, mem, name)	mem = nullptr; assert(mem == nullptr);
#define mem_ptr_def_delete(type, mem, name)	delete mem; mem = nullptr;
#define mem_ptr_def_from(type, mem, name)	try {d.mem = new type(j.at(name).get<type>());} catch (const std::exception& ) {}
#define mem_ptr_def_to(type, mem, name)		if (d.mem!=nullptr) j[name] = *d.mem;

#define struct_def(struName)\
struct struName {	\
	mem_def_##struName(def)\
};\
serialize_def(struName)

#define struct_comp_def(struName)\
struct struName {	\
	mem_def_##struName(def)\
	struName() { \
		mem_def_##struName(init)\
	}\
	struName(const struName &other) {\
		mem_def_##struName(init)\
		*this = other;\
	}\
	struName& operator=(const struName& other) {\
		mem_def_##struName(copy)\
		return *this;\
	}\
	~struName(void) {\
		mem_def_##struName(delete)\
	}\
};\
serialize_def(struName)

#define serialize_def(struName)\
static void to_json(nlohmann::json& j, const struName& d) {\
	mem_def_##struName(to);\
}\
static void from_json(const nlohmann::json& j, struName& d) {\
	mem_def_##struName(from);\
}


#endif //SDPTRANS_JSONSERIALIZE_H
