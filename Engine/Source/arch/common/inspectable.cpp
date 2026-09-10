#include <pch.h>
#include <arch/common/inspectable.h>
#include <arch/resources/res_assetManager.h>
#include <util/util_color.h>


namespace {
    using SetterFunction = std::function<void(void*, const void*)>;
    using Options = std::vector<PropertyMD::Option>;
    struct PropertyProps {
        SetterFunction setter;
        int componentCount;
        const Options& options;
        PropertyMD::PropertyType propType;
    };


    void SetEnum(SetterFunction _setter, const Options& _options, Inspectable* _object, const rapidjson::Value& _value) {
        int val{};
        if (_value.IsInt()) {
            val = _value.GetInt();
        }
        else {
            std::string enumVal = _value.GetString();
            for (const PropertyMD::Option& option : _options) {
                if (option.label == enumVal) {
                    val = option.value;
                    break;
                }
            }
        }
        _setter(_object, &val);
    }

    void SetInt(SetterFunction _setter, int _componentCount, Inspectable* _object, const rapidjson::Value& _value) {
        Inspectable* object = _object;
        switch (_componentCount) {
        case 1: {
            int val{ _value.GetInt() };
            _setter(object, &val);
            break;
        }

        case 2: {
            glm::ivec2 val{ _value[0].GetInt(), _value[1].GetInt() };
            _setter(object, &val);
            break;
        }

        case 3: {
            glm::ivec3 val{ _value[0].GetInt(), _value[1].GetInt(),  _value[2].GetInt() };
            _setter(object, &val);
            break;
        }

        case 4: {
            glm::ivec4 val{ _value[0].GetInt(), _value[1].GetInt(),  _value[2].GetInt(), _value[3].GetInt() };
            _setter(object, &val);
            break;
        }
        }
    }

    void SetFloat(SetterFunction _setter, int _componentCount, Inspectable* _object, const rapidjson::Value& _value) {
        Inspectable* object = _object;
        switch (_componentCount) {

        case 1: {
            float val{ _value.GetFloat() };
            _setter(object, &val);
            break;
        }

        case 2: {
            glm::fvec2 val{ _value[0].GetFloat(), _value[1].GetFloat() };
            _setter(object, &val);
            break;
        }

        case 3: {
            glm::fvec3 val{ _value[0].GetFloat(), _value[1].GetFloat(),  _value[2].GetFloat() };
            _setter(object, &val);
            break;
        }

        case 4: {
            glm::fvec4 val{ _value[0].GetFloat(), _value[1].GetFloat(),  _value[2].GetFloat(), _value[3].GetFloat() };
            _setter(object, &val);
            break;
        }
        }
    }

    void SetDouble(SetterFunction _setter, int _componentCount, Inspectable* _object, const rapidjson::Value& _value) {
        Inspectable* object = _object;
        switch (_componentCount) {

        case 1: {
            double val{ _value.GetDouble() };
            _setter(object, &val);
            break;
        }

        case 2: {
            glm::dvec2 val{ _value[0].GetDouble(), _value[1].GetDouble() };
            _setter(object, &val);
            break;
        }

        case 3: {
            glm::dvec3 val{ _value[0].GetDouble(), _value[1].GetDouble(),  _value[2].GetDouble() };
            _setter(object, &val);
            break;
        }

        case 4: {
            glm::dvec4 val{ _value[0].GetDouble(), _value[1].GetDouble(),  _value[2].GetDouble(), _value[3].GetDouble() };
            _setter(object, &val);
            break;
        }
        }
    }

    void SetString(SetterFunction _setter, Inspectable* _object, const rapidjson::Value& _value) {
        std::string value = _value.GetString();
        _setter(&_object, &value);
    }
    
    void SetBool(SetterFunction _setter, Inspectable* _object, const rapidjson::Value& _value) {
        bool val{ _value.IsString() ? _value.GetString() == "true" : _value.GetBool() };
        _setter(_object, &val);
    }

    void SetResource(SetterFunction _setter, Inspectable* _object, const rapidjson::Value& _value, AssetManager& _asMgr) {
        // it cannot use a string.
        // always a value.
        ResourceManager& rsMgr = _asMgr.GetResourceManager();
        uint64_t resId{ ResourceConstants::C_RES_INVALID_ID };
        if (_value.IsString()) {
            // do 
            std::string alias = _value.GetString();
            resId = _asMgr.GetResourceManager().GetResourceFromAlias(alias);
        }
        else if (_value.IsUint64()) {
            resId = _value.GetUint64();
        }
        ResourceHandle handle(rsMgr.GetResourceIdentifier(resId));
        _setter(_object, &handle);
    }

    

    void SetColor(SetterFunction _setter, int _componentCount, Inspectable* _object, const rapidjson::Value& _value) {
        // color is set as vectors and not unsigned.
        Inspectable* object = _object;
        if (_componentCount == 3) {
            glm::vec3 val = _value.IsString() ?
                Color::HexToVec3F(std::stoul(_value.GetString(), nullptr, 16)) :
                glm::vec3(_value[0].GetFloat(), _value[1].GetFloat(), _value[2].GetFloat());

            _setter(object, &val);
        }
        else {
            glm::vec4 val = _value.IsString() ?
                Color::HexToVec4F(std::stoul(_value.GetString(), nullptr, 16)) :
                glm::vec4(_value[0].GetFloat(), _value[1].GetFloat(), _value[2].GetFloat(), _value[3].GetFloat());
            _setter(object, &val);
        }
    }

    void SetValue(PropertyProps _props, AssetManager& _asMgr, Inspectable* _object, const rapidjson::Value& _value) {
        SetterFunction setter = _props.setter;
        int compCount = _props.componentCount;
        const Options& options = _props.options;
        using namespace PropertyMD;
        const PropertyType& type = _props.propType;


        if (!options.empty()) {
            SetEnum(setter, options, _object, _value);
            return;
        }
        switch (type) {
        case PropertyType::Color: {
            SetColor(setter, compCount, _object, _value);
            break;
        }
        case PropertyType::Int: {
            SetInt(setter, compCount, _object, _value);
            break;
        }
        case PropertyType::Float: {
            SetFloat(setter, compCount, _object, _value);
            break;
        }
        case PropertyType::Double: {
            SetDouble(setter, compCount, _object, _value);
            break;
        }
        case PropertyType::Boolean: {
            SetBool(setter, _object, _value);
            break;
        }
        case PropertyType::String: {
            SetString(setter, _object, _value);
            break;
        }
        case PropertyType::Object: {
            // ??
            break;
        }
        case PropertyType::ResourceHandle:
            SetResource(setter, _object, _value, _asMgr);

            break;
        default:
            break;
        }


    }

    void SetValue(const PropertyMD::Property& _prop, Inspectable* _object, const rapidjson::Value& _value, AssetManager& _asMgr) {
        using namespace PropertyMD;

        PropertyProps props{
            _prop.m_set,
            _prop.m_componentCount,
            _prop.m_options,
            _prop.m_type
        };
        SetValue(props, _asMgr, _object, _value);
    }

    // do something here.

    void SetList(
        const PropertyMD::Property& _prop, 
        Inspectable* _object, 
        const rapidjson::Value& _val, 
        AssetManager& _asMgr
    ) {
        std::function<void(void*, void*)> addFunction = _prop.m_list.m_addFunction;
        if (!addFunction) {
            LOG_ERROR("Add function not set.");
            return;
        }
        ResourceManager& rsm{ _asMgr.GetResourceManager() };
        for (const rapidjson::Value* itr = _val.Begin(); itr != _val.End(); ++itr) {


            if (itr->IsUint64()) {
                RES_ID resid{ itr->GetUint64() };
                ResourceHandle handle (rsm.GetResourceIdentifier(resid));
                addFunction(_object, &handle);
            }
            else {
                // we need to get the object.
                // but what is the type?
            }


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
            if (prop.m_list.m_valid) {
                // for now we'll use the resource ids.
                SetList(prop, this, val, _asMgr);
            }
            else {
                SetValue(prop, this, val, _asMgr);
            }

        }
    } 
}