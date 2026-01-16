#pragma once
#include <chrono>
#include <math.h>



// ----------------------------------------------------------------------

class EntityID {
public:
	EntityID() : m_id{ static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()) } {
		std::stringstream ss;
		ss << typeid(EntityID).name() << "::Constructor(): ID - " << m_id << std::endl;
		LOG_INFO(ss.str());
	};
	EntityID(unsigned _id) : m_id{ _id } {};
	EntityID(const EntityID&) = default;
	EntityID& operator=(const EntityID& _other) = default;


	bool operator<(const EntityID& other)	const { return m_id < other.m_id; }
	bool operator>(const EntityID& other)	const { return m_id > other.m_id; }
	bool operator==(const EntityID& other)	const { return m_id == other.m_id; }
	bool operator==(unsigned long other)	const { return m_id == other; }

	inline bool IsValid() const { return m_id != ENTITYID_INVALID; };

	unsigned GetID() const { return m_id; }

	friend std::ostream& operator<<(std::ostream&, const EntityID&);

	// cut up the id, first bit should be an active bit.

	static constexpr unsigned ENTITYID_INVALID = 0; // Define invalid ID
private:
	unsigned long m_id{};
};


namespace std {
	template <>
	struct hash<EntityID> {
		std::size_t operator()(const EntityID& id) const noexcept {
			return std::hash<unsigned>{}(id.GetID());
		}
	};
}