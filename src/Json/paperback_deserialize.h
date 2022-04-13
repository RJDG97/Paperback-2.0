/**********************************************************************************
*\file         paperback_deserialize.h
*\brief        paperback_deserialize.h
*
*\author	   Adele Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/document.h>

namespace paperback::deserialize
{
    PPB_INLINE
    rttr::variant ExtractBasicType(rapidjson::Value& json_value);

    PPB_INLINE
    void ReadArray(rttr::variant_sequential_view& view, rapidjson::Value& json_array_value);

    PPB_INLINE
    rttr::variant extract_value(rapidjson::Value::MemberIterator& itr, const rttr::type& t);

    PPB_INLINE
    void ReadAssociative(rttr::variant_associative_view& view, rapidjson::Value& json_array_value);

    PPB_INLINE
    void ReadRecursive(rttr::instance obj2, rapidjson::Value& json_object);

    PPB_INLINE
    void ReadObject(rttr::instance obj, rapidjson::Document& doc);

    PPB_INLINE
    void ReadEntities(rapidjson::Value::MemberIterator it, bool ReadPrefabs = false );

    PPB_INLINE
    void ReadEntityInfo(rapidjson::Value::MemberIterator it);

    PPB_INLINE
    void ReadComponents(rapidjson::Value::MemberIterator it, paperback::archetype::instance* NewArchetype, u32 EntityCounter, bool LoadPrefab);

    PPB_INLINE
    void ExtractWrapperType(rttr::variant& Obj, rttr::variant& Extracted);

    //used for general use case
    PPB_INLINE
    void ReadStringPairs(std::stringstream& stream, rapidjson::Value& json_object);
}