#pragma once
#define RAPIDJSON_HAS_STDSTRING 1

namespace paperback::deserialize
{
    rttr::variant ExtractBasicType(rapidjson::Value& json_value)
    {
        switch (json_value.GetType())
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

    rttr::variant extract_value(rapidjson::Value::MemberIterator& itr, const rttr::type& t)
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
            switch (json_value.GetType())
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

    void ReadObject( rttr::instance obj, rapidjson::Document& doc )
    {
        ReadRecursive(obj, doc);
    }

    void ReadEntityInfo(rapidjson::Value::MemberIterator it)
    {
        for (rapidjson::Value::ValueIterator itr = it->value.Begin(); itr != it->value.End(); itr++)
        {
            u32 InfoCount{ };
            auto& EntityInfoList = PPB.GetEntityInfoList();

            for (rapidjson::Value::MemberIterator mitr = itr->MemberBegin(); mitr != itr->MemberEnd(); mitr++)
            {                
                rttr::type type = rttr::type::get_by_name(mitr->name.GetString());
                rttr::variant obj = type.get_constructor().invoke();

                if (!obj.is_type<paperback::component::temp_guid>())
                    ReadRecursive(obj, mitr->value);

                if (obj.is_type<paperback::entity::TempInfo>())
                {
                    //check Guid value if 0 assign Archetype::instance* as nullptr
                    EntityInfoList[InfoCount].m_pArchetype = obj.get_value<paperback::entity::TempInfo>().ArchetypeGuid ? &PPB.GetArchetype(obj.get_value<paperback::entity::TempInfo>().ArchetypeGuid) : nullptr;
                    EntityInfoList[InfoCount].m_PoolDetails = obj.get_value<paperback::entity::TempInfo>().PoolDetails;
                    EntityInfoList[InfoCount++].m_Validation = obj.get_value<paperback::entity::TempInfo>().Validation;
                }
            }
        }
    }

    void ReadComponents( rapidjson::Value::MemberIterator it, paperback::archetype::instance* NewArchetype, u32 EntityCounter)
    {
        if (NewArchetype)
        {
            rttr::type type = rttr::type::get_by_name(it->name.GetString());
            rttr::variant obj = type.get_constructor().invoke();

            auto& Value = it->value;

            if (!obj.is_type<paperback::component::temp_guid>())
                ReadRecursive(obj, Value);

            if (obj.is_type<transform>())
                NewArchetype->GetComponent<transform>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<transform>();

            if (obj.is_type<scale>())
                NewArchetype->GetComponent<scale>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<scale>();

            if (obj.is_type<rotation>())
                NewArchetype->GetComponent<rotation>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<rotation>();

            if (obj.is_type<mesh>())
                NewArchetype->GetComponent<mesh>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<mesh>();

            if (obj.is_type<component::entity>())
                NewArchetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<paperback::component::entity>();

            if (obj.is_type<sound>())
                NewArchetype->GetComponent<sound>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<sound>();

            if (obj.is_type<timer>())
                NewArchetype->GetComponent<timer>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<timer>();

            if (obj.is_type<entityscript>())
                NewArchetype->GetComponent<entityscript>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<entityscript>();

            if (obj.is_type<boundingbox>())
                NewArchetype->GetComponent<boundingbox>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<boundingbox>();

            if (obj.is_type<sphere>())
                NewArchetype->GetComponent<sphere>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<sphere>();

            if (obj.is_type<rigidbody>())
                NewArchetype->GetComponent<rigidbody>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<rigidbody>();

            if (obj.is_type<rigidforce>())
                NewArchetype->GetComponent<rigidforce>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<rigidforce>();

            if (obj.is_type<animator>())
                NewArchetype->GetComponent<animator>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<animator>();

            if (obj.is_type<child>())
                NewArchetype->GetComponent<child>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<child>();

            if (obj.is_type<parent>())
                NewArchetype->GetComponent<parent>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<parent>();

            if (obj.is_type<prefab>())
                NewArchetype->GetComponent<prefab>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<prefab>();

            if (obj.is_type<reference_prefab>())
                NewArchetype->GetComponent<reference_prefab>(paperback::vm::PoolDetails{ 0, EntityCounter }) = obj.get_value<reference_prefab>();
        }
    }

    void ReadEntities( rapidjson::Value::MemberIterator it )
    {
        using NewComponentInfoList = std::array<const component::info*, settings::max_components_per_entity_v>;
        std::string TempName{};
        for (rapidjson::Value::ValueIterator itr = it->value.Begin(); itr != it->value.End(); itr++)
        {
            for (rapidjson::Value::MemberIterator mitr = itr->MemberBegin(); mitr != itr->MemberEnd(); mitr++)
            {
                NewComponentInfoList CList{};
                paperback::u32 Counter = 0, EntityCounter = 0;
                paperback::archetype::instance* NewArchetype = nullptr;
                tools::bits ArchetypeSignature;

                TempName = mitr->name.GetString(); //Archetype Manager Name Appears in this layer

                if (mitr->value.IsArray() && !TempName.empty())
                {
                    for (rapidjson::Value::ValueIterator vitr = mitr->value.Begin(); vitr != mitr->value.End(); vitr++)
                    {
                        if (NewArchetype)
                        {
                            NewArchetype->AccessGuard([&]()
                                {
                                    NewArchetype->CreateEntity();
                                    NewArchetype->SetName(TempName);

                                    for (rapidjson::Value::MemberIterator Mitr = vitr->MemberBegin(); Mitr != vitr->MemberEnd(); Mitr++)
                                        ReadComponents(Mitr, NewArchetype, EntityCounter);

                                    EntityCounter++;
                                }
                            );
                        }
                        else
                        {   
                            // Load Each Archetype's Components GUID
                            for (rapidjson::Value::MemberIterator Mitr = vitr->MemberBegin(); Mitr != vitr->MemberEnd(); Mitr++)
                            {
                                if ( Mitr->value.IsArray() )
                                {
                                    rttr::variant obj = rttr::type::get_by_name(Mitr->name.GetString()).get_constructor().invoke();

                                    if (obj.is_type<paperback::component::temp_guid>())
                                    {
                                        for (rapidjson::Value::ValueIterator Vitr = Mitr->value.Begin(); Vitr != Mitr->value.End(); Vitr++)
                                        {
                                            paperback::component::type::guid TempGuid{ Vitr->GetUint64() };

                                            CList[Counter] = PPB.FindComponentInfo(TempGuid);
                                            ArchetypeSignature.Set(CList[Counter++]->m_UID);
                                        }

                                        NewArchetype = &PPB.GetOrCreateArchetype(ArchetypeSignature); // Replace with Create Prefab
                                        PPB_ASSERT_MSG(NewArchetype == nullptr, "Archetype Failed to Init");
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                   // Shld enter here first since Archetype Manager stuff isnt in an array + First item in the json
                   //Access the Archetype Manager Data Stuff

                   rttr::type type = rttr::type::get_by_name(mitr->name.GetString());
                   rttr::variant obj = type.get_constructor().invoke();

                   if (!obj.is_type<paperback::component::temp_guid>())
                       ReadRecursive(obj, mitr->value);

                   if (obj.is_type<paperback::archetype::TempMgr>())
                   {
                       PPB.SetEntityHead(obj.get_value<paperback::archetype::TempMgr>().EntityHead);
                   }
                }
            }
        }
    }
}