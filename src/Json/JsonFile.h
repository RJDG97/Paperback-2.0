#pragma once
#define RAPIDJSON_HAS_STDSTRING 1

#include <string>
#include <iostream>
#include <rapidjson/prettywriter.h>
#include <filewritestream.h>
#include <filereadstream.h>
#include <rapidjson/document.h>
#include <rttr/type>

#include "JsonSerialize/JsonSerialize.h"
#include "JsonSerialize/JsonDeserialize.h"

using namespace rttr;
using namespace rapidjson;

namespace paperback 
{
    class JsonFile
    {
        char* buffer { nullptr };
        FILE* fp { nullptr };
        FileWriteStream* wstream {nullptr};
        FileReadStream* rstream {nullptr};
        PrettyWriter<rapidjson::FileWriteStream>* writer {nullptr};
        Document* doc {nullptr};

        public:
        //-----------------------------------
        //          Serialization
        //-----------------------------------

        JsonFile& StartWriter(std::string file)
        {
            assert(fp == nullptr);
            fopen_s(&fp, file.c_str(), "wb");
            buffer = new char[65536] {};
            wstream = new FileWriteStream(fp, buffer, 65536);
            writer = new PrettyWriter<FileWriteStream>(*wstream);

            return *this;
        }

        JsonFile& EndWriter()
        {
            delete writer;
            delete wstream;
            delete buffer;

            fclose(fp);
            fp = nullptr;

            return *this;
        }

        JsonFile& StartObject()
        {
            writer->StartObject();
            return *this;
        }

        JsonFile& EndObject()
        {
            writer->EndObject();
            return *this;
        }

        JsonFile& StartArray()
        {
            writer->StartArray();
            return *this;
        }

        JsonFile& EndArray()
        {
            writer->EndArray();
            return *this;
        }

        JsonFile& WriteKey(std::string str)
        {
           writer->String( str );
           return *this;
        }

        JsonFile& Write(instance obj)
        {
            serialize::Write(obj, *writer);
            return *this;
        }

        JsonFile& WriteArray(const variant_sequential_view& view)
        {
            serialize::WriteArray(view, *writer);
        }

        JsonFile& WriteAssociativeContainers(const variant_associative_view& view)
        {
            serialize::Write(view, *writer);
            return *this;
        }

        //-----------------------------------
        //         Deserialization
        //-----------------------------------

        JsonFile& StartReader(std::string file)
        {
            assert(fp == nullptr);
            fopen_s(&fp, file.c_str(), "rb");
            buffer = new char[65536]; buffer;
            rstream = new FileReadStream(fp, buffer, 65536);
            doc = new Document();
            assert(!doc->ParseStream(*rstream).HasParseError());

            return *this;
        }

        JsonFile& EndReader()
        {
            delete doc;
            delete rstream;
            delete buffer;
            fclose(fp);
            fp = nullptr;

            return *this;
        }

        JsonFile& LoadArray(variant_sequential_view& view, Value& json_array_value)
        {
            deserialize::ReadArray(view, json_array_value);
            return *this;
        }

        JsonFile& LoadAssociativeArray(variant_associative_view &view, Value& json_array_value)
        {
            deserialize::ReadAssociative(view, json_array_value);
            return *this;
        }

        JsonFile& LoadEntities(const char* archetype)
        {
            Value::MemberIterator it = doc->FindMember(archetype);
            //if (it != doc->MemberEnd())
            // deserialize::ReadEntities(it);

            return *this;
        }

        JsonFile& LoadObject(instance obj)
        {
            deserialize::ReadObject(obj, *doc);
            return *this;
        }
    };
}
