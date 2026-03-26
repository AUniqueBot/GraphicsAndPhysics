#include<arch/common/component.h>

std::vector<PropertyMD::Property>& Component::GetComponentProperties() {
	static std::vector<PropertyMD::Property> props;
	return props;
}
