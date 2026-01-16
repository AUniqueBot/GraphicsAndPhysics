#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

class RenderTarget;

enum class ColorChannelLayout {
	RED,
	RG,
	RGB,
	RGBA,
};

enum class DepthChannelLayout {
	DEPTH,
	DEPTH_STENCIL,
	NONE
};

constexpr const char* ToString(ColorChannelLayout _v) {
	switch (_v) {
	case ColorChannelLayout::RED:           return "RED";
	case ColorChannelLayout::RG:            return "RG";
	case ColorChannelLayout::RGB:           return "RGB";
	case ColorChannelLayout::RGBA:          return "RGBA";
	}
	return "UNKNOWN";
}

constexpr const char* ToString(DepthChannelLayout _v) {
	switch (_v) {
	case DepthChannelLayout::DEPTH:         return "DEPTH";
	case DepthChannelLayout::DEPTH_STENCIL:	return "DEPTH_STENCIL";
	case DepthChannelLayout::NONE:			return "NONE";
	}
	return "UNKNOWN";
}

enum class TextureFilter {
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
	// onl
	NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
};



enum class ScalarType {
	BYTE = GL_BYTE,
	UBYTE = GL_UNSIGNED_BYTE,
	SHORT = GL_SHORT,
	INT = GL_INT,
	UINT = GL_UNSIGNED_INT,
	HALF_FLOAT = GL_HALF_FLOAT,
	FLOAT = GL_FLOAT
};

constexpr const char* ToString(ScalarType _v) {
	switch (_v) {
	case ScalarType::BYTE:          return "BYTE";
	case ScalarType::UBYTE:         return "UBYTE";
	case ScalarType::SHORT:         return "SHORT";
	case ScalarType::INT:			return "INT";
	case ScalarType::UINT:			return "UINT";
	case ScalarType::HALF_FLOAT:	return "HALF_FLOAT";
	case ScalarType::FLOAT:			return "FLOAT";
	}
	return "UNKNOWN";
}


class RenderTargetAttachment {
public:
	friend class RenderTarget;
public:
	RenderTargetAttachment(
		const std::string& _name,
		const ScalarType& _scalarType,
		const TextureFilter& _filter,
		const GLenum& _wrapBehaviour,
		const bool& _enableDrawToLocation
	) :
		m_name{ _name },
		m_scalarType{ _scalarType },
		m_filter{ _filter },
		m_wrapBehaviour{ _wrapBehaviour }
	{

	}
	ScalarType		m_scalarType;

	TextureFilter m_filter;
	GLenum m_wrapBehaviour;



	std::string m_name;
	uint32_t	m_attachmentIndex	{}; // location number.
	uint32_t	m_textureId			{};
};


class ColorAttachment : public RenderTargetAttachment {

public:
	friend class RenderTarget;
public:
	ColorAttachment(
		const std::string& _name,
		const ColorChannelLayout& _layout,
		const ScalarType& _scalarType,
		const TextureFilter& _filter,
		const GLenum& _wrapBehaviour,
		const bool& _enableDrawToLocation
	) :
		RenderTargetAttachment(_name, _scalarType, _filter, _wrapBehaviour, _enableDrawToLocation),
		m_channelLayout{ _layout }

	{
		
	}
	

public:
	ColorChannelLayout	m_channelLayout;



};

class DepthAttachment : public RenderTargetAttachment{
public:
	friend class RenderTarget;
public:
	DepthAttachment(
		const std::string& _name = "",
		const DepthChannelLayout& _layout = DepthChannelLayout::DEPTH,
		const ScalarType& _scalarType = ScalarType::FLOAT,
		const TextureFilter& _filter = TextureFilter::LINEAR,
		const GLenum& _wrapBehaviour = GL_WRAP_BORDER,
		const bool& _enableDrawToLocation = true
	) :
		RenderTargetAttachment(_name, _scalarType, _filter, _wrapBehaviour, _enableDrawToLocation),
		m_channelLayout{_layout}
	{}

	DepthChannelLayout m_channelLayout;
};




struct GLFormat {
	GLenum internalFormat;
	GLenum format;
	GLenum scalarType;
	bool integer; // true if integer type
};

// hash helper for unordered_map key
struct ColorChannelCastData {
	ColorChannelLayout layout;
	ScalarType scalarType;
	bool operator==(const ColorChannelCastData& other) const {
		return layout == other.layout && scalarType == other.scalarType;
	}
};

struct ColorChannelKeyHash {
	std::size_t operator()(const ColorChannelCastData& k) const noexcept {
		return std::hash<int>()(static_cast<int>(k.layout)) ^
			(std::hash<int>()(static_cast<int>(k.scalarType)) << 1);
	}
};

inline const std::unordered_map<ColorChannelCastData, GLFormat, ColorChannelKeyHash> C_COLOR_FORMAT_LOOKUPTABLE = {

	// =========================
	// RED
	// =========================
	{{ColorChannelLayout::RED, ScalarType::BYTE},        {GL_R8,     GL_RED,         GL_BYTE,		   false}},
	{{ColorChannelLayout::RED, ScalarType::UBYTE},       {GL_R8,     GL_RED,         GL_UNSIGNED_BYTE, false}},
	{{ColorChannelLayout::RED, ScalarType::SHORT},       {GL_R16,    GL_RED,         GL_SHORT,         false}},
	{{ColorChannelLayout::RED, ScalarType::HALF_FLOAT},  {GL_R16F,   GL_RED,         GL_HALF_FLOAT,    false}},
	{{ColorChannelLayout::RED, ScalarType::FLOAT},       {GL_R32F,   GL_RED,         GL_FLOAT,         false}},
	{{ColorChannelLayout::RED, ScalarType::INT},         {GL_R32I,   GL_RED_INTEGER, GL_INT,           true}},
	{{ColorChannelLayout::RED, ScalarType::UINT},        {GL_R32UI,  GL_RED_INTEGER, GL_UNSIGNED_INT,  true}},

	// =========================
	// RG
	// =========================
	{{ColorChannelLayout::RG, ScalarType::BYTE},         {GL_RG8,     GL_RG,         GL_UNSIGNED_BYTE, false}},
	{{ColorChannelLayout::RG, ScalarType::SHORT},        {GL_RG16,    GL_RG,         GL_SHORT,         false}},
	{{ColorChannelLayout::RG, ScalarType::HALF_FLOAT},   {GL_RG16F,   GL_RG,         GL_HALF_FLOAT,    false}},
	{{ColorChannelLayout::RG, ScalarType::FLOAT},        {GL_RG32F,   GL_RG,         GL_FLOAT,         false}},
	{{ColorChannelLayout::RG, ScalarType::INT},          {GL_RG32I,   GL_RG_INTEGER, GL_INT,           true}},
	{{ColorChannelLayout::RG, ScalarType::UINT},         {GL_RG32UI,  GL_RG_INTEGER, GL_UNSIGNED_INT,  true}},

	// =========================
	// RGB
	// =========================
	{{ColorChannelLayout::RGB, ScalarType::BYTE},        {GL_RGB8,    GL_RGB,        GL_UNSIGNED_BYTE, false}},
	{{ColorChannelLayout::RGB, ScalarType::SHORT},       {GL_RGB16,   GL_RGB,        GL_SHORT,         false}},
	{{ColorChannelLayout::RGB, ScalarType::HALF_FLOAT},  {GL_RGB16F,  GL_RGB,        GL_HALF_FLOAT,    false}},
	{{ColorChannelLayout::RGB, ScalarType::FLOAT},       {GL_RGB32F,  GL_RGB,        GL_FLOAT,         false}},
	{{ColorChannelLayout::RGB, ScalarType::INT},         {GL_RGB32I,  GL_RGB_INTEGER,GL_INT,           true}},
	{{ColorChannelLayout::RGB, ScalarType::UINT},        {GL_RGB32UI, GL_RGB_INTEGER,GL_UNSIGNED_INT,  true}},

	// =========================
	// RGBA
	// =========================
	{{ColorChannelLayout::RGBA, ScalarType::BYTE},       {GL_RGBA8,    GL_RGBA,        GL_UNSIGNED_BYTE, false}},
	{{ColorChannelLayout::RGBA, ScalarType::SHORT},      {GL_RGBA16,   GL_RGBA,        GL_SHORT,         false}},
	{{ColorChannelLayout::RGBA, ScalarType::HALF_FLOAT}, {GL_RGBA16F,  GL_RGBA,        GL_HALF_FLOAT,    false}},
	{{ColorChannelLayout::RGBA, ScalarType::FLOAT},      {GL_RGBA32F,  GL_RGBA,        GL_FLOAT,         false}},
	{{ColorChannelLayout::RGBA, ScalarType::INT},        {GL_RGBA32I,  GL_RGBA_INTEGER,GL_INT,           true}},
	{{ColorChannelLayout::RGBA, ScalarType::UINT},       {GL_RGBA32UI, GL_RGBA_INTEGER,GL_UNSIGNED_INT,  true}},


};

struct DepthChannelCastData {
	DepthChannelLayout layout;
	ScalarType scalarType;
	bool operator==(const DepthChannelCastData& other) const {
		return layout == other.layout && scalarType == other.scalarType;
	}
};

struct DepthChannelKeyHash {
	std::size_t operator()(const DepthChannelCastData& k) const noexcept {
		return std::hash<int>()(static_cast<int>(k.layout)) ^
			(std::hash<int>()(static_cast<int>(k.scalarType)) << 1);
	}
};

// =========================
// DEPTH
// =========================
//{{ChannelLayout::DEPTH, ScalarType::UINT},      {GL_DEPTH_COMPONENT24,  GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, false}},
//{{ChannelLayout::DEPTH, ScalarType::FLOAT},     {GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT,        false}},

// =========================
// DEPTH + STENCIL
// =========================
//{{ChannelLayout::DEPTH_STENCIL, ScalarType::UINT},
	//{GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, false}},


inline const std::unordered_map<DepthChannelCastData, GLFormat, DepthChannelKeyHash> C_DEPTH_FORMAT_LOOKUPTABLE = {

	// =========================
	// DEPTH
	// =========================
	{{DepthChannelLayout::DEPTH, ScalarType::UINT},      {GL_DEPTH_COMPONENT24,  GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, false}},
	{{DepthChannelLayout::DEPTH, ScalarType::FLOAT},     {GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT,        false}},

	// =========================
	// DEPTH + STENCIL
	// =========================
	{{DepthChannelLayout::DEPTH_STENCIL, ScalarType::UINT}, {GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, false}},



};