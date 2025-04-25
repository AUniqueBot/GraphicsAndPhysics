#pragma once
#include <chrono>
#include <math.h>


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
static std::hash<ch::time_point<ch::system_clock>> s_TpHash{};
static std::hash<ch::time_point<ch::file_clock>> s_FpHash{};

// ----------------------------------------------------------------------

class EntityID {
public:
	EntityID() : m_id{ static_cast<unsigned long>(s_TpHash(ch::system_clock::now())) } {};
	EntityID(unsigned long _id) : m_id{ _id } {};
	EntityID(const EntityID&) = default;
	EntityID& operator=(const EntityID&) = default;


	bool operator<(const EntityID& other)	const { return m_id < other.m_id; }
	bool operator>(const EntityID& other)	const { return m_id > other.m_id; }
	bool operator==(const EntityID& other)	const { return m_id == other.m_id; }
	bool operator==(unsigned long other)	const { return m_id == other; }

	inline bool IsValid() const { return m_id != ENTITYID_INVALID; };

	unsigned long GetID() const { return m_id; }


	// cut up the id, first bit should be an active bit.

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
