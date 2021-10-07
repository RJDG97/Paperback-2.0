
#pragma once
#define RAPIDJSON_HAS_STDSTRING 1

namespace paperback::serialize
{
    bool WriteAtomic(const rttr::type& t, const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer)
    {
        if (t.is_arithmetic())
        {
            if (t == rttr::type::get<bool>())
                writer.Bool(var.to_bool());
            else if (t == rttr::type::get<char>())
                writer.Bool(var.to_bool());
            else if (t == rttr::type::get<int8_t>())
                writer.Int(var.to_int8());
            else if (t == rttr::type::get<int16_t>())
                writer.Int(var.to_int16());
            else if (t == rttr::type::get<int32_t>())
                writer.Int(var.to_int32());
            else if (t == rttr::type::get<int64_t>())
                writer.Int64(var.to_int64());
            else if (t == rttr::type::get<uint8_t>())
                writer.Uint(var.to_uint8());
            else if (t == rttr::type::get<uint16_t>())
                writer.Uint(var.to_uint16());
            else if (t == rttr::type::get<uint32_t>())
                writer.Uint(var.to_uint32());
            else if (t == rttr::type::get<uint64_t>())
                writer.Uint64(var.to_uint64());
            else if (t == rttr::type::get<float>())
                writer.Double(var.to_double());
            else if (t == rttr::type::get<double>())
                writer.Double(var.to_double());

            return true;
        }
        else if (t.is_enumeration())
        {
            bool ok = false;
            auto result = var.to_string(&ok);
            if (ok)
                writer.String(var.to_string());
            else
            {
                ok = false;
                auto value = var.to_uint64(&ok);
                if (ok)
                    writer.Uint64(value);
                else
                    writer.Null();
            }

            return true;
        }
        else if (t == rttr::type::get<std::string>())
        {
            writer.String(var.to_string());
            return true;
        }

        return false;
    }

    void WriteRecursive(const rttr::instance& obj2, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer)
    {
        writer.StartObject();

        rttr::instance obj = obj2.get_type().get_raw_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;

        auto prop_list = obj.get_derived_type().get_properties();
        for (auto prop : prop_list)
        {
            rttr::variant prop_value = prop.get_value(obj);
            if (!prop_value)
                continue; // cannot serialize, because we cannot retrieve the value

            const auto name = prop.get_name();
            writer.String(name.data(), static_cast<rapidjson::SizeType>(name.length()), false);
            WriteVariant(prop_value, writer);
            PPB_ASSERT_MSG(!WriteVariant(prop_value, writer), "Cannot Serialize Property:");
            //if (!WriteVariant(prop_value, writer))
            //    std::cerr << "cannot serialize property: " << name << std::endl;
        }

        writer.EndObject();
    }


    void Write(rttr::instance object, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer)
    {
        rttr::instance obj = object.get_type().get_raw_type().is_wrapper() ? object.get_wrapped_instance() : object;

        auto prop_list = obj.get_derived_type().get_properties();
        for (auto prop : prop_list)
        {
            rttr::variant prop_value = prop.get_value(obj);
            if (!prop_value)
                continue; // cannot serialize, because we cannot retrieve the value

            const auto name = prop.get_name();
            writer.String(name.data(), static_cast<rapidjson::SizeType>(name.length()), false);
            WriteVariant(prop_value, writer);
            PPB_ASSERT_MSG(!WriteVariant(prop_value, writer), "Cannot Serialize Property");
        }
    }

    void SerializeGuid(rttr::instance obj, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer)
    {
        rttr::instance object = obj.get_type().get_raw_type().is_wrapper() ? obj.get_wrapped_instance() : obj;

        auto prop_list = object.get_derived_type().get_properties();
        for (auto prop : prop_list)
        {
            rttr::variant prop_value = prop.get_value(object);
            if (!prop_value)
                continue; // cannot serialize, because we cannot retrieve the value
            WriteVariant(prop_value, writer);
            PPB_ASSERT_MSG(!WriteVariant(prop_value, writer), "Cannot Serialize Property");
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    void WriteArray(const rttr::variant_sequential_view& view, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer)
    {
        writer.StartArray();

        for (const auto& item : view)
            if (item.is_sequential_container())
                WriteArray(item.create_sequential_view(), writer);
            else
            {
                rttr::variant wrapped_var = item.extract_wrapped_value();
                rttr::type value_type = wrapped_var.get_type();
                if (value_type.is_arithmetic() || value_type == rttr::type::get<std::string>() || value_type.is_enumeration())
                {
                    WriteAtomic(value_type, wrapped_var, writer);
                }
                else // object
                {
                    WriteRecursive(wrapped_var, writer);
                }
            }

        writer.EndArray();
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    void WriteAssociative(const rttr::variant_associative_view& view, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer)
    {
        static const rttr::string_view key_name("key");
        static const rttr::string_view value_name("value");

        writer.StartArray();

        if (view.is_key_only_type())
            for (auto& item : view)
                WriteVariant(item.first, writer);
        else
            for (auto& item : view)
            {
                writer.StartObject();
                writer.String(key_name.data(), static_cast<rapidjson::SizeType>(key_name.length()), false);

                WriteVariant(item.first, writer);

                writer.String(value_name.data(), static_cast<rapidjson::SizeType>(value_name.length()), false);

                WriteVariant(item.second, writer);

                writer.EndObject();
            }

        writer.EndArray();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    bool WriteVariant(const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer)
    {
        auto value_type = var.get_type();
        auto wrapped_type = value_type.is_wrapper() ? value_type.get_wrapped_type() : value_type;
        bool is_wrapper = wrapped_type != value_type;

        if (WriteAtomic(is_wrapper ? wrapped_type : value_type,
            is_wrapper ? var.extract_wrapped_value() : var, writer))
        {
        }
        else if (var.is_sequential_container())
            WriteArray(var.create_sequential_view(), writer);
        else if (var.is_associative_container())
            WriteAssociative(var.create_associative_view(), writer);
        else
        {
            auto child_props = is_wrapper ? wrapped_type.get_properties() : value_type.get_properties();
            if (!child_props.empty())
                WriteRecursive(var, writer);
            else
            {
                bool ok = false;
                auto text = var.to_string(&ok);
                if (!ok)
                {
                    writer.String(text);
                    return false;
                }

                writer.String(text);
            }
        }
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

}