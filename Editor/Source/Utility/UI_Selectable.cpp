#include <Utility/UI_Selectable.h>

bool UI_Selectable::operator==(const UI_Selectable& _other) const {
	return _other.m_type == m_type &&
		((m_id.m_resId == _other.m_id.m_resId) || (m_id.m_entityId == _other.m_id.m_entityId));
}