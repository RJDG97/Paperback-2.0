#pragma once
#define RAPIDJSON_HAS_STDSTRING 1
//change spacing & remove ref before monke beat mi

#include <string>
#include <iostream>
#include <rapidjson/prettywriter.h>
#include <filewritestream.h>
#include <filereadstream.h>
#include <rapidjson/document.h>
#include <rttr/type>

#include "Json/JsonSerialize.h"
#include "Json/JsonDeserialize.h"

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

        JsonFile& StartWriter(std::string File)
        {
            fopen_s(&fp, File.c_str(), "wb");
            PPB_ASSERT(fp == nullptr);
            buffer = new char[65536] {};
            wstream = new rapidjson::FileWriteStream(fp, buffer, 65536);
            writer = new rapidjson::PrettyWriter<rapidjson::FileWriteStream>(*wstream);

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

        JsonFile& WriteKey( std::string Str )
        {
           writer->String( Str );
           return *this;
        }

        JsonFile& Write( rttr::instance Obj )
        {
            serialize::Write( Obj, *writer );
            return *this;
        }

        JsonFile& WriteGuid(rttr::instance Obj)
        {
            serialize::SerializeGuid(Obj, *writer);
            return *this;
        }

        JsonFile& WriteArray( const rttr::variant_sequential_view& View )
        {
            serialize::WriteArray( View, *writer );
        }

        JsonFile& WriteAssociativeContainers( const rttr::variant_associative_view& View )
        {
            serialize::Write( View, *writer );
            return *this;
        }

        //-----------------------------------
        //         Deserialization
        //-----------------------------------

        JsonFile& StartReader(std::string File)
        {
            fopen_s(&fp, File.c_str(), "rb");
            PPB_ASSERT(fp == nullptr);
            buffer = new char[65536]; buffer;
            rstream = new rapidjson::FileReadStream(fp, buffer, 65536);
            doc = new rapidjson::Document();
            PPB_ASSERT(doc->ParseStream(*rstream).HasParseError());

            //if (doc->ParseStream(*rstream).HasParseError())
            //{
            //    std::cout << GetParseError_En(doc->GetParseError()) << std::endl;
            //}
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

        JsonFile& LoadArray(rttr::variant_sequential_view& View, rapidjson::Value& Json_array_value)
        {
            deserialize::ReadArray(View, Json_array_value);
            return *this;
        }

        JsonFile& LoadAssociativeArray(rttr::variant_associative_view &View, rapidjson::Value& Json_array_value)
        {
            deserialize::ReadAssociative(View, Json_array_value);
            return *this;
        }

        JsonFile& LoadEntities(const char* Archetype)
        {
            rapidjson::Value::MemberIterator it = doc->FindMember(Archetype);
            if ( it != doc->MemberEnd() )
                deserialize::ReadEntities(it);

            return *this;
        }

        JsonFile& LoadObjects( rttr::instance Instance )
        {
            deserialize::ReadObject(Instance, *doc);
            return *this;
        }
    };
}
