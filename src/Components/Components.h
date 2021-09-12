#pragma once

#include <rttr/registration>
#include "Json/JsonSerialize.h"
//entity manager
#include <iostream>

#define REGISTER_COMPONENT(TYPE) static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer, void* ptr)\
{\
    TYPE t = *static_cast<TYPE*>(ptr);\
    serialize::WriteObject(writer, t);\
}\
static void Deserialize(/*entitymanager em, EntityID id,*/ rttr::variant var)\
{\
    TYPE t = var.get_value<TYPE>();\
    //add component
}


