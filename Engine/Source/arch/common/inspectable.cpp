#include <pch.h>
#include <arch/common/inspectable.h>
#include <arch/resources/res_assetManager.h>
#include <util/util_color.h>


namespace {
	
    void SetEnum(const PropertyMD::Property& _prop, Inspectable* _object, const rapidjson::Value& _value) {
        int val{};

        if (_value.IsInt()) {
            val = _value.GetInt();
        }
        else {
            std::string enumVal = _value.GetString();
            for (const PropertyMD::Option& option : _prop.m_options) {
                if (option.label == enumVal) {
                    val = option.value;
                    break;
                }
            }
        }
        _prop.m_set(_object, &val);
    }

    void SetInt(const PropertyMD::Property& _prop, Inspectable* _object, const rapidjson::Value& _value) {
        const PropertyMD::Property& prop = _prop;
        Inspectable* object = _object;
        switch (_prop.m_componentCount) {

        case 1: {
            int val{ _value.GetInt() };
            prop.m_set(object, &val);

            break;
        }

        case 2: {
            glm::ivec2 val{ _value[0].GetInt(), _value[1].GetInt()};
            prop.m_set(object, &val);
            break;
        }

        case 3: {
            glm::ivec3 val{ _value[0].GetInt(), _value[1].GetInt(),  _value[2].GetInt() };
            prop.m_set(object, &val);
            break;
        }

        case 4: {
            glm::ivec4 val{ _value[0].GetInt(), _value[1].GetInt(),  _value[2].GetInt(), _value[3].GetInt() };
            prop.m_set(object, &val);
            break;
        }
        }
    }
    
    void SetFloat(const PropertyMD::Property& _prop, Inspectable* _object, const rapidjson::Value& _value) {
        const PropertyMD::Property& prop = _prop;
        Inspectable* object = _object;
        switch (_prop.m_componentCount) {

        case 1: {
            float val{ _value.GetFloat() };
            prop.m_set(object, &val);

            break;
        }

        case 2: {
            glm::fvec2 val{ _value[0].GetFloat(), _value[1].GetFloat() };
            prop.m_set(object, &val);
            break;
        }

        case 3: {
            glm::fvec3 val{ _value[0].GetFloat(), _value[1].GetFloat(),  _value[2].GetFloat() };
            prop.m_set(object, &val);
            break;
        }

        case 4: {
            glm::fvec4 val{ _value[0].GetFloat(), _value[1].GetFloat(),  _value[2].GetFloat(), _value[3].GetFloat() };
            prop.m_set(object, &val);
            break;
        }
        }
    }
    
    void SetDouble(const PropertyMD::Property& _prop, Inspectable* _object, const rapidjson::Value& _value) {
        const  PropertyMD::Property& prop = _prop;
        Inspectable* object = _object;
        switch (_prop.m_componentCount) {

        case 1: {
            double val{ _value.GetDouble() };
            prop.m_set(object, &val);

            break;
        }

        case 2: {
            glm::dvec2 val{ _value[0].GetDouble(), _value[1].GetDouble() };
            prop.m_set(object, &val);
            break;
        }

        case 3: {
            glm::dvec3 val{ _value[0].GetDouble(), _value[1].GetDouble(),  _value[2].GetDouble() };
            prop.m_set(object, &val);
            break;
        }

        case 4: {
            glm::dvec4 val{ _value[0].GetDouble(), _value[1].GetDouble(),  _value[2].GetDouble(), _value[3].GetDouble() };
            prop.m_set(object, &val);
            break;
        }
        }
    }
    
    void SetString(const PropertyMD::Property& _prop, Inspectable* _object, const rapidjson::Value& _value) {
        std::string value = _value.GetString();
        _prop.m_set(&_object, &value);
    }
    
    void SetBool(const PropertyMD::Property& _prop, Inspectable* _object, const rapidjson::Value& _value) {
        bool val{ _value.IsString() ? _value.GetString() == "true" : _value.GetBool() };
        _prop.m_set(_object, &val);
    }

    void SetResource(const PropertyMD::Property& _prop, Inspectable* _object, const rapidjson::Value& _value, AssetManager& _asMgr) {
        // it cannot use a string.
        // always a value.
        uint64_t resId { ResourceConstants::C_RES_INVALID_ID };
        if (_value.IsString()) {
            // do 
            std::string alias = _value.GetString();
            resId = _asMgr.GetResourceManager().GetResourceFromAlias(alias );
        }
        else {
            resId = _value.GetUint64();
        }



        _prop.m_set(_object, &resId);
    }
    
    void SetColor(const PropertyMD::Property& _prop, Inspectable* _object, const rapidjson::Value& _value) {
        const PropertyMD::Property& prop = _prop;
        // color is set as vectors and not unsigned.
        Inspectable* object = _object;
        if (prop.m_componentCount == 3) {
            glm::vec3 val = _value.IsString() ? 
                Color::HexToVec3F(std::stoul(_value.GetString(), nullptr, 16)) :
                glm::vec3(_value[0].GetFloat(), _value[1].GetFloat(), _value[2].GetFloat());
            
            prop.m_set(object, &val);
        }
        else {
            glm::vec4 val = _value.IsString() ?
                Color::HexToVec4F(std::stoul(_value.GetString(), nullptr, 16)) :
                glm::vec4(_value[0].GetFloat(), _value[1].GetFloat(), _value[2].GetFloat(), _value[3].GetFloat());
            prop.m_set(object, &val);
        }
    }
}


void Inspectable::Deserialize(const Serialization::JSONFile& _data, AssetManager& _asMgr) {
    using namespace PropertyMD;
    // the ability to override is to allow you to deserialize custom data.
    auto& props = GetProperties();
    
    for (const PropertyMD::Property& prop : props) {
        std::string attrName = prop.m_name;
        if (_data.HasMember(attrName.c_str())) {
            // deserialize members here.
            PropertyMD::PropertyType type = prop.m_type;

            const rapidjson::Value& val = _data.GetMember(attrName.c_str());

            if (prop.m_isEnum) {
                SetEnum(prop, this, val);
                continue;
            }

            switch (prop.m_type) {
            case PropertyType::Color: {
                SetColor(prop, this, val);
                break;
            }
            case PropertyType::Int: {
                SetInt(prop, this, val);
                break;
            }
            case PropertyType::Float: {
                SetFloat(prop, this, val);
                break;
            }
            case PropertyType::Double: {
                SetDouble(prop, this, val);
                break;
            }
            case PropertyType::Boolean: {
                SetBool(prop, this, val);
                break;
            }
            case PropertyType::String: {
                std::string value = val.GetString();
                prop.m_set(this, &value);
                break;
            }
            case PropertyType::Object: {
                // ??
                break;
            }

            case PropertyType::Resource: {
                // we are
                SetResource(prop, this, val, _asMgr);
                break;
            }

            case PropertyType::ResourceHandle:

                break;
            default:
                break;
            }




        }
    }
}