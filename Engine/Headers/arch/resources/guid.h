#pragma once
#include <random>
#include <cstdint>
#include <format>


namespace GUIDConstants {
	constexpr const GUID C_GUID_INVALID	{};
};

struct GUID {
	uint64_t m_high;
	uint64_t m_low;
	inline bool operator==(const GUID& _other) const {
		return m_high == _other.m_high && m_low == _other.m_low;
	}
	size_t operator()(const GUID& guid) const noexcept {
		size_t h1 = std::hash<uint64_t>{}(guid.m_high);
		size_t h2 = std::hash<uint64_t>{}(guid.m_low);
		return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
	}
};



inline GUID GenerateGUID() {
	static std::random_device rd;
	static std::mt19937_64 rng(rd());
	return{
		rng(),
		rng()
	};
}

inline std::string GUIDToString(const GUID& guid) {
	return std::format(
		"{:016X}{:016X}",
		guid.m_high,
		guid.m_low
	);
}

inline std::string GUIDToString(const GUID& guid) {
	return std::format(
		"{:08X}-{:04X}-{:04X}-{:04X}-{:012X}",
		static_cast<uint32_t>(guid.m_high >> 32),
		static_cast<uint16_t>(guid.m_high >> 16),
		static_cast<uint16_t>(guid.m_high),
		static_cast<uint16_t>(guid.m_low >> 48),
		guid.m_low & 0xFFFFFFFFFFFFULL
	);
}