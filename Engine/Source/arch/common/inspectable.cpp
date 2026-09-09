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
        ResourceManager& rsMgr = _asMgr.GetResourceManager();
        
        uint64_t resId { ResourceConstants::C_RES_INVALID_ID };
        if (_value.IsString()) {
            // do 
            std::string alias = _value.GetString();
            resId = _asMgr.GetResourceManager().GetResourceFromAlias(alias);
        }
        else if (_value.IsUint64()) {
            resId = _value.GetUint64();
        }

        ResourceHandle handle(rsMgr.GetResourceIdentifier(resId));
        
        _prop.m_set(_object, &handle);
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


    void SetValue(const PropertyMD::Property& _prop, Inspectable* _object, const rapidjson::Value& _value, AssetManager& _asMgr) {
        using namespace PropertyMD;

        if (_prop.m_isEnum) {
            SetEnum(_prop, _object, _value);
            return;
        }





        switch (_prop.m_type) {
        case PropertyType::Color: {
            SetColor(_prop, _object, _value);
            break;
        }
        case PropertyType::Int: {
            SetInt(_prop, _object, _value);
            break;
        }
        case PropertyType::Float: {
            SetFloat(_prop, _object, _value);
            break;
        }
        case PropertyType::Double: {
            SetDouble(_prop, _object, _value);
            break;
        }
        case PropertyType::Boolean: {
            SetBool(_prop, _object, _value);
            break;
        }
        case PropertyType::String: {
            std::string value = _value.GetString();
            _prop.m_set(_object, &value);
            break;
        }
        case PropertyType::Object: {
            // ??
            break;
        }


        case PropertyType::ResourceHandle:
            SetResource(_prop, _object, _value, _asMgr);

            break;
        default:
            break;
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
            if (prop.m_shape == Shape::DynamicList) {
                prop.m_list;
                _data.IsArray();
                               
            }
            else {
                SetValue(prop, this, val, _asMgr);
            }

        }
    }
}