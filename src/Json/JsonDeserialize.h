#pragma once
#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/document.h>
#include <rttr/type>
#include <string>

namespace paperback::deserialize 
{
    /////////////////////////////////////////////////////////////////////////////////////////
    //Forward Declaration

    void ReadRecursive(rttr::instance obj2, rapidjson::Value& json_object);

    /////////////////////////////////////////////////////////////////////////////////////////

    rttr::variant ExtractBasicType(rapidjson::Value& json_value)
    {
        switch(json_value.GetType())
        {
            case rapidjson::kStringType:
            {
                return std::string(json_value.GetString());
                break;
            }
            case rapidjson::kNullType:     break;
            case rapidjson::kFalseType:
            case rapidjson::kTrueType:
            {
                return json_value.GetBool();
                break;
            }
            case rapidjson::kNumberType:
            {
                if (json_value.IsInt())
                    return json_value.GetInt();
                else if (json_value.IsDouble())
                    return json_value.GetDouble();
                else if (json_value.IsUint())
                    return json_value.GetUint();
                else if (json_value.IsInt64())
                    return json_value.GetInt64();
                else if (json_value.IsUint64())
                    return json_value.GetUint64();
                break;
            }
            // we handle only the basic types here
            case rapidjson::kObjectType:
            case rapidjson::kArrayType: return rttr::variant();
        }

        return rttr::variant();
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    void ReadArray(rttr::variant_sequential_view& view, rapidjson::Value& json_array_value)
    {
        view.set_size(json_array_value.Size());
        for (rapidjson::SizeType i = 0; i < json_array_value.Size(); ++i)
        {
            auto& json_index_value = json_array_value[i];
            if (json_index_value.IsArray())
            {
                auto sub_array_view = view.get_value(i).create_sequential_view();
                ReadArray(sub_array_view, json_index_value);
            }
            else if (json_index_value.IsObject())
            {
                rttr::variant var_tmp = view.get_value(i);
                rttr::variant wrapped_var = var_tmp.extract_wrapped_value();
                ReadRecursive(wrapped_var, json_index_value);
                view.set_value(i, wrapped_var);
            }
            else
            {
                const rttr::type array_type = view.get_rank_type(i);
                rttr::variant extracted_value = ExtractBasicType(json_index_value);
                if (extracted_value.convert(array_type))
                    view.set_value(i, extracted_value);
            }
        }
    }

    rttr::variant extract_value (rapidjson::Value::MemberIterator& itr, const rttr::type& t)
    {
        auto& json_value = itr->value;
        rttr::variant extracted_value = ExtractBasicType(json_value);
        const bool could_convert = extracted_value.convert(t);
        if (!could_convert)
        {
            if (json_value.IsObject())
            {
                rttr::constructor ctor = t.get_constructor();
                for (auto& item : t.get_constructors())
                    if (item.get_instantiated_type() == t)
                        ctor = item;
                extracted_value = ctor.invoke();
                ReadRecursive(extracted_value, json_value);
            }
        }

        return extracted_value;
    }

    void ReadAssociative(rttr::variant_associative_view& view, rapidjson::Value& json_array_value)
    {
        for (rapidjson::SizeType i = 0; i < json_array_value.Size(); ++i)
        {
            auto& json_index_value = json_array_value[i];
            if (json_index_value.IsObject()) // a key-value associative view
            {
                rapidjson::Value::MemberIterator key_itr = json_index_value.FindMember("key");
                rapidjson::Value::MemberIterator value_itr = json_index_value.FindMember("value");

                if (key_itr != json_index_value.MemberEnd() &&
                    value_itr != json_index_value.MemberEnd())
                {
                    auto key_var = extract_value(key_itr, view.get_key_type());
                    auto value_var = extract_value(value_itr, view.get_value_type());
                    if (key_var && value_var)
                    {
                        view.insert(key_var, value_var);
                    }
                }
            }
            else // a key-only associative view
            {
                rttr::variant extracted_value = ExtractBasicType(json_index_value);
                if (extracted_value && extracted_value.convert(view.get_key_type()))
                    view.insert(extracted_value);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    void ReadRecursive(rttr::instance obj2, rapidjson::Value& json_object)
    {
        rttr::instance obj = obj2.get_type().get_raw_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;
        const auto prop_list = obj.get_derived_type().get_properties();
        for (auto prop : prop_list)
        {
            rapidjson::Value::MemberIterator ret = json_object.FindMember(prop.get_name().data());
            if (ret == json_object.MemberEnd())
                continue;
            const rttr::type value_t = prop.get_type();

            auto& json_value = ret->value;
            switch(json_value.GetType())
            {
                case rapidjson::kArrayType:
                {
                    rttr::variant var;
                    if (value_t.is_sequential_container())
                    {
                        var = prop.get_value(obj);
                        auto view = var.create_sequential_view();
                        ReadArray(view, json_value);
                    }
                    else if (value_t.is_associative_container())
                    {
                        var = prop.get_value(obj);
                        auto associative_view = var.create_associative_view();
                        ReadAssociative(associative_view, json_value);
                    }

                    prop.set_value(obj, var);
                    break;
                }
                case rapidjson::kObjectType:
                {
                    rttr::variant var = prop.get_value(obj);
                    ReadRecursive(var, json_value);
                    prop.set_value(obj, var);
                    break;
                }
                default:
                {
                    rttr::variant extracted_value = ExtractBasicType(json_value);
                    if (extracted_value.convert(value_t)) 
                        prop.set_value(obj, extracted_value);
                }
            }
        }
    }

    void ReadObject( rttr::instance obj, rapidjson::Document& doc)
    {
        ReadRecursive(obj, doc);
    }

    void ReadEntity( rapidjson::Value::MemberIterator it) 
    {
        // to do link to the creating entity
    }

}