#pragma once
#include <cstdint>


using GPURES_ID = uint64_t;
inline constexpr const GPURES_ID C_GPURES_INVALID_ID	{ 0u };

enum class GPUDatatype {
	Texture,
	Buffer,
	IndexBuffer,
	VAO,
	FrameBuffer,
	Mesh,
	UNKNOWN
};

struct GPUResourceHandle {
	GPURES_ID m_id{ C_GPURES_INVALID_ID };
	GPUDatatype m_type{ GPUDatatype::UNKNOWN };
	inline bool IsValid() const {
		return m_id != C_GPURES_INVALID_ID && m_type != GPUDatatype::UNKNOWN;
	}
	void Reset() {
		m_id = C_GPURES_INVALID_ID;
		m_type = GPUDatatype::UNKNOWN;
	}
};

