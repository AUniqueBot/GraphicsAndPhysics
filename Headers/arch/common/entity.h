#pragma once

#include <bitset>
#include <arch/components/componentList.h>
#include <chrono>
#include <math.h>
#include <entt/entt.hpp>m


class EntityID;
class EntityRegistry;

namespace ch = std::chrono;

// hashing.
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

class EntityID {
public:
	EntityID() : m_id { static_cast<unsigned long>(s_TpHash(ch::system_clock::now())) } {};
	EntityID(unsigned long _id) : m_id{ _id } {};
	EntityID(const EntityID&) = default;
	EntityID& operator=(const EntityID&) = default;


	bool operator<(const EntityID& other) { return m_id < other.m_id; }
	bool operator>(const EntityID& other) { return m_id > other.m_id; }
	bool operator==(const EntityID& other) { return m_id == other.m_id; }

	inline bool IsValid() const { return m_id != ENTITYID_INVALID; };

	unsigned long GetID() const { return m_id; }


private:
	static constexpr unsigned long ENTITYID_INVALID = 0; // Define invalid ID
	
	unsigned long m_id{};
};


// make EntityID hashable.
namespace std {
	template <>
	struct hash<EntityID> {
		std::size_t operator()(const EntityID& id) const noexcept {
			return std::hash<unsigned long>()(id.GetID());
		}
	};
}



// ----------------------------------------------------------------------

class Entity {

public:
	
	EntityID GetID() const		{ return m_id; };
	bool IsValid() const { return m_id.IsValid(); };


	template <typename T>
	void AddComponent() {};
	template <typename T>
	void RemoveComponent() {};

private:


	EntityRegistry* m_registry					{};
	EntityID m_id								{ 0 };
	std::bitset<ComponentType::COUNT> m_flags;

};



