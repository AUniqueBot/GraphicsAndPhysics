#pragma once
#include <bitset>
#include <arch/components/componentList.h>
#include <chrono>

class EntityID;
const EntityID C_INVALID { 0x000000 };


namespace ch = std::chrono;


template<>
struct std::hash<ch::system_clock::time_point> {
	std::size_t operator()(ch::system_clock::time_point const& d) const noexcept {
		return d.time_since_epoch().count();
	}
};

template<>
struct std::hash<ch::file_clock::time_point> {
	std::size_t operator()(ch::file_clock::time_point const& d) const noexcept {
		return d.time_since_epoch().count();
	}
};

static std::hash<std::string> s_StrHash{};
static std::hash<ch::time_point<ch::system_clock>> s_TpHash	{};
static std::hash<ch::time_point<ch::file_clock>> s_FpHash	{};
// ----------------------------------------------------------------------

class Entity {

public:
	
	EntityID GetID() const		{ return m_id; };
	bool IsValid() const { return m_id.IsValid(); };

	// needs an ID to refer to.
private:
	EntityID m_id				{ 0 };
	std::bitset<ComponentType::COUNT> m_flags;

};


// ----------------------------------------------------------------------

class EntityID {
public:
	EntityID() : m_id{ s_TpHash(ch::system_clock::now()) } {};
	EntityID(unsigned long _id) : m_id { _id } {};
	EntityID(const EntityID&) = default;

	inline bool IsValid() const { return m_id != 0; };

private:
	unsigned long m_id {};
};

