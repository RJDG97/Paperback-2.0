#pragma once

#include <string>
#include <iostream>
#include <rapidjson/prettywriter.h>
#include <filewritestream.h>
#include <filereadstream.h>
#include <rapidjson/document.h>
#include <rttr/type>

#include "Json/JsonDeserialize.h"

//-----------------------------------
//         Deserialization
//-----------------------------------
namespace paperback
{
    class JsonRead
    {
    public:

        JsonRead& StartReader(std::string File)
        {
            fopen_s(&fp, File.c_str(), "rb");
            PPB_ASSERT(fp == nullptr);
            buffer = new char[65536]; buffer;
            rstream = new rapidjson::FileReadStream(fp, buffer, 65536);
            doc = new rapidjson::Document();
            PPB_ASSERT(doc->ParseStream(*rstream).HasParseError());

            return *this;
        }

        JsonRead& EndReader()
        {
            delete doc;
            delete rstream;
            delete buffer;
            fclose(fp);
            fp = nullptr;

            return *this;
        }

        JsonRead& LoadArray(rttr::variant_sequential_view& View, rapidjson::Value& Json_array_value)
        {
            deserialize::ReadArray(View, Json_array_value);
            return *this;
        }

        JsonRead& LoadAssociativeArray(rttr::variant_associative_view& View, rapidjson::Value& Json_array_value)
        {
            deserialize::ReadAssociative(View, Json_array_value);
            return *this;
        }

        JsonRead& LoadEntities(const char* Archetype)
        {
            rapidjson::Value::MemberIterator it = doc->FindMember(Archetype);
            if (it != doc->MemberEnd())
                deserialize::ReadEntities(it);

            return *this;
        }

        JsonRead& LoadObjects(rttr::instance Instance)
        {
            deserialize::ReadObject(Instance, *doc);
            return *this;
        }
    };
}