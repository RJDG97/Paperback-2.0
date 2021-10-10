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
    bool WriteAtomic(const rttr::type& t, const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    void WriteRecursive(const rttr::instance& obj2, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    void Write(rttr::instance object, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    void SerializeGuid(rttr::instance obj, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    void WriteArray(const rttr::variant_sequential_view& view, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    void WriteAssociative(const rttr::variant_associative_view& view, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    bool WriteVariant(const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

    void WriteObject(rttr::instance obj, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer);

}