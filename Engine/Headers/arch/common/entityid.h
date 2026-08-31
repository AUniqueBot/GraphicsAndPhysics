#pragma once
#include <chrono>
#include <math.h>
#include <pch.h>

using EntityIDType = uint32_t;
namespace EntityConstants {
	inline constexpr const EntityIDType C_ENTITYID_INVALID = 0;
};

// ----------------------------------------------------------------------

class EntityID {
public:
	EntityID() : m_id{ static_cast<EntityIDType>(std::chrono::system_clock::now().time_since_epoch().count()) } {
		std::stringstream ss;
		ss << typeid(EntityID).name() << "::Constructor(): ID - " << m_id << std::endl;
		LOG_INFO(ss.str());
	};
	EntityID(EntityIDType _id) : m_id{ _id } {};
	EntityID(const EntityID&) = default;
	EntityID& operator=(const EntityID& _other) = default;


	bool operator<(const EntityID& other)	const { return m_id < other.m_id; }
	bool operator>(const EntityID& other)	const { return m_id > other.m_id; }
	bool operator==(const EntityID& other)	const { return m_id == other.m_id; }
	bool operator==(EntityIDType other)		const { return m_id == other; }
	explicit operator EntityIDType()		const { return m_id; }


	inline bool IsValid() const { return m_id != EntityConstants::C_ENTITYID_INVALID; };

	EntityIDType GetID() const { return m_id; }

	friend std::ostream& operator<<(std::ostream&, const EntityID&);

	// cut up the id, first bit should be an active bit.
	inline static EntityIDType ENTITYID_INVALID { 0 };
private:
	EntityIDType m_id{};
};


namespace std {
	template <>
	struct hash<EntityID> {
		std::size_t operator()(const EntityID& id) const noexcept {
			return std::hash<EntityIDType>{}(id.GetID());
		}
	};
}