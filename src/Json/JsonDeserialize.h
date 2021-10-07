#pragma once
#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/document.h>


namespace paperback::deserialize 
{
    rttr::variant ExtractBasicType(rapidjson::Value& json_value);

    void ReadArray(rttr::variant_sequential_view& view, rapidjson::Value& json_array_value);

    rttr::variant extract_value(rapidjson::Value::MemberIterator& itr, const rttr::type& t);

    void ReadAssociative(rttr::variant_associative_view& view, rapidjson::Value& json_array_value);

    void ReadRecursive(rttr::instance obj2, rapidjson::Value& json_object);

    void ReadObject( rttr::instance obj, rapidjson::Document& doc);

    void ReadEntities( rapidjson::Value::MemberIterator it) ;
}