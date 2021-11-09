#pragma once
#define RAPIDJSON_HAS_STDSTRING 1

namespace paperback
{
    //-----------------------------------
    //          Serialization
    //-----------------------------------

    inline JsonFile& JsonFile::StartWriter( std::string File )
    {
        fp = { nullptr };

        fopen_s(&fp, File.c_str(), "wb");
        PPB_ASSERT(fp == nullptr);
        buffer = new char[65536]{};
        wstream = new rapidjson::FileWriteStream(fp, buffer, 65536);
        writer = new rapidjson::PrettyWriter<rapidjson::FileWriteStream>(*wstream);

        return *this;
    }

    inline JsonFile& JsonFile::EndWriter()
    {
        delete writer;
        delete wstream;
        delete buffer;
        fclose(fp);
        fp = nullptr;

        return *this;
    }

    inline JsonFile& JsonFile::StartObject()
    {
        writer->StartObject();
        return *this;
    }

    inline JsonFile& JsonFile::EndObject()
    {
        writer->EndObject();
        return *this;
    }

    inline JsonFile& JsonFile::StartArray()
    {
        writer->StartArray();
        return *this;
    }

    inline JsonFile& JsonFile::EndArray()
    {
        writer->EndArray();
        return *this;
    }

    inline JsonFile& JsonFile::WriteKey( std::string Str )
    {
        writer->String(Str);
        return *this;
    }

    inline JsonFile& JsonFile::Write( rttr::instance Obj )
    {
        serialize::Write(Obj, *writer);
        return *this;
    }

    inline JsonFile& JsonFile::WriteGuid( rttr::instance Obj )
    {
        serialize::SerializeGuid(Obj, *writer);
        return *this;
    }

    inline JsonFile& JsonFile::WriteArray( const rttr::variant_sequential_view& View )
    {
        serialize::WriteArray(View, *writer);
    }

    inline JsonFile& JsonFile::WriteAssociativeContainers( const rttr::variant_associative_view& View )
    {
        serialize::Write(View, *writer);
        return *this;
    }

    inline JsonFile& JsonFile::ReadObjects(rttr::instance Instance)
    {
        serialize::WriteObject( Instance, *writer );
        return *this;
    }

    //-----------------------------------
    //         Deserialization
    //-----------------------------------

    inline JsonFile& JsonFile::StartReader(std::string File)
    {
        fopen_s(&fp, File.c_str(), "rb");
        PPB_ASSERT(fp == nullptr);
        buffer = new char[65536]; buffer;
        rstream = new rapidjson::FileReadStream(fp, buffer, 65536);
        doc = new rapidjson::Document();
        if (doc->ParseStream(*rstream).HasParseError()) ERROR_LOG("Cannot open file");

        return *this;
    }

    inline JsonFile& JsonFile::EndReader()
    {
        delete doc;
        delete rstream;
        delete buffer;
        fclose(fp);
        fp = nullptr;

        return *this;
    }

    inline JsonFile& JsonFile::LoadArray(rttr::variant_sequential_view& View, rapidjson::Value& Json_array_value)
    {
        deserialize::ReadArray(View, Json_array_value);
        return *this;
    }

    inline JsonFile& JsonFile::LoadAssociativeArray(rttr::variant_associative_view& View, rapidjson::Value& Json_array_value)
    {
        deserialize::ReadAssociative(View, Json_array_value);
        return *this;
    }

    inline JsonFile& JsonFile::LoadEntities(const char* Archetype)
    {
        rapidjson::Value::MemberIterator it = doc->FindMember(Archetype);
        if (it != doc->MemberEnd())
            if (Archetype == "Entities") //Load Entities + Prefabs + Archetype Mgr
                deserialize::ReadEntities(it);
            else if (Archetype == "All Entity Info") // Load Entity Infos
                deserialize::ReadEntityInfo(it);

        return *this;
    }

    inline JsonFile& JsonFile::LoadObjects(rttr::instance Instance)
    {
        deserialize::ReadObject(Instance, *doc); // reading of normal json whose type is registered with rttr
        return *this;
    }
}
