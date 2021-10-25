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
    void ReadEntities(rapidjson::Value::MemberIterator it);
}