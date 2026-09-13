#pragma once
#include <pch.h>


using RESTYPE_ID = uint32_t;	// id of the TYPE of resource
namespace ResourceConstants {
	inline constexpr const RESTYPE_ID C_RESTYPE_INVALID_ID{ 0 };
};


struct ResourceTypeMetadata {
public:
	using RESTYPE_INFO = std::type_index;
public:
	ResourceTypeMetadata(
		std::string _typeName,
		RESTYPE_ID _typeId,
		std::type_index _typeInfo
	) :
		m_name				{ _typeName },
		m_resourceTypeID	{ _typeId },
		m_typeInfo			{ _typeInfo }

	{

	}

	const std::string& GetName() const { return m_name; }
	const RESTYPE_ID& GetResourceTypeID() const { return m_resourceTypeID; }
	const RESTYPE_INFO& GetTypeInfo() const { return m_typeInfo; }

	

private:
	std::string m_name;
	RESTYPE_ID m_resourceTypeID;
	std::type_index m_typeInfo;
};