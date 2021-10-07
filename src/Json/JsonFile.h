#pragma once
#define RAPIDJSON_HAS_STDSTRING 1

#include <string>
#include <iostream>
#include <rapidjson/prettywriter.h>
#include <filewritestream.h>
#include <filereadstream.h>
#include <rapidjson/document.h>
#include <rttr/type>

#include "Json/JsonSerialize.h"

#include <rapidjson/error/en.h>

namespace paperback 
{
    class JsonFile
    {
        char* buffer { nullptr };
        FILE* fp { nullptr };
        rapidjson::FileWriteStream* wstream {nullptr};
        rapidjson::FileReadStream* rstream {nullptr};
        rapidjson::PrettyWriter<rapidjson::FileWriteStream>* writer {nullptr};
        rapidjson::Document* doc {nullptr};

        public:
        //-----------------------------------
        //          Serialization
        //-----------------------------------

        JsonFile& StartWriter(std::string File);

        JsonFile& EndWriter();

        JsonFile& StartObject();

        JsonFile& EndObject();
        JsonFile& StartArray();
        JsonFile& EndArray();
        JsonFile& WriteKey(std::string Str);

        JsonFile& Write(rttr::instance Obj);

        JsonFile& WriteGuid(rttr::instance Obj);

        JsonFile& WriteArray(const rttr::variant_sequential_view& View);

        JsonFile& WriteAssociativeContainers(const rttr::variant_associative_view& View);

        //-----------------------------------
        //         Deserialization
        //-----------------------------------

        JsonFile& StartReader(std::string File);

        JsonFile& EndReader();

        JsonFile& LoadArray(rttr::variant_sequential_view& View, rapidjson::Value& Json_array_value);

        JsonFile& LoadAssociativeArray(rttr::variant_associative_view& View, rapidjson::Value& Json_array_value);

        JsonFile& LoadEntities(const char* Archetype);

        JsonFile& LoadObjects(rttr::instance Instance);

    };
}
