/**********************************************************************************
*\file         paperback_serialize.h
*\brief        paperback_serialize.h
*
*\author	   Adele Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/prettywriter.h>
#include <rapidjson/filewritestream.h>
#include <rttr/type>
#include <iostream>
#include <cstdint>
#include "Components/component_includes.h"

namespace paperback::serialize
{
    PPB_INLINE
    bool WriteAtomic(const rttr::type& t, const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    PPB_INLINE
    void WriteRecursive(const rttr::instance& obj2, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    PPB_INLINE
    void Write(rttr::instance object, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    PPB_INLINE
    void SerializeGuid(rttr::instance obj, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    PPB_INLINE
    void WriteArray(const rttr::variant_sequential_view& view, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    PPB_INLINE
    void WriteAssociative(const rttr::variant_associative_view& view, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    PPB_INLINE
    bool WriteVariant(const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    PPB_INLINE
    void WriteObject(rttr::instance obj, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

}