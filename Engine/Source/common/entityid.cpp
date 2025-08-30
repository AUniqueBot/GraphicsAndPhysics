#include <pch.h>
#include <arch/common/entityid.h>

std::ostream& operator<<(std::ostream& os, const EntityID& _id) {
	os << "ID: [" << _id.m_id << "]";
	return os;
}
