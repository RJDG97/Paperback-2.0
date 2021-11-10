#pragma once
#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/prettywriter.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>

#include <rapidjson/error/en.h>

namespace paperback
{
    class JsonFile
    {
        char* buffer{ nullptr };
        FILE* fp{ nullptr };
        rapidjson::FileWriteStream* wstream{ nullptr };
        rapidjson::FileReadStream* rstream{ nullptr };
        rapidjson::PrettyWriter<rapidjson::FileWriteStream>* writer{ nullptr };
        rapidjson::Document* doc{ nullptr };

    public:
        //-----------------------------------
        //          Serialization
        //-----------------------------------

        PPB_INLINE
        JsonFile& StartWriter(std::string File);

        PPB_INLINE
        JsonFile& EndWriter();

        PPB_INLINE
        JsonFile& StartObject();

        PPB_INLINE
        JsonFile& EndObject();

        PPB_INLINE
        JsonFile& StartArray();

        PPB_INLINE
        JsonFile& EndArray();

        PPB_INLINE
        JsonFile& WriteKey(std::string Str);

        PPB_INLINE
        JsonFile& Write(rttr::instance Obj);

        PPB_INLINE
        JsonFile& WriteGuid(rttr::instance Obj);

        PPB_INLINE
        JsonFile& WriteArray(const rttr::variant_sequential_view& View);

        PPB_INLINE
        JsonFile& WriteAssociativeContainers(const rttr::variant_associative_view& View);

        PPB_INLINE
        JsonFile& ReadObjects(rttr::instance Instance);

        //-----------------------------------
        //         Deserialization
        //-----------------------------------

        PPB_INLINE
        JsonFile& StartReader(std::string File);

        PPB_INLINE
        JsonFile& EndReader();

        PPB_INLINE
        JsonFile& LoadArray(rttr::variant_sequential_view& View, rapidjson::Value& Json_array_value);

        PPB_INLINE
        JsonFile& LoadAssociativeArray(rttr::variant_associative_view& View, rapidjson::Value& Json_array_value);

        PPB_INLINE
        JsonFile& LoadEntities(const char* Archetype);

        PPB_INLINE
        JsonFile& LoadObjects(rttr::instance Instance);


        //extremely general purpose use case
        // for simple jsons consisting of only pairs of data with no spaces
        // and receiving object cannot be registered via rttr due to dynamic nature
        //    e.g. vectors of objects, data to be loaded into object is in vector elements
        PPB_INLINE
        JsonFile& LoadStringPairs(std::stringstream& Stream);
    };
}
