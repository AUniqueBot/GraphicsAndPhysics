#pragma once
#include <pch.h>


struct ConfigSection {
	std::unordered_map<std::string, std::string> props;

	template <typename T>
	inline T Get(std::string _attributeName) const;

	template <>
	inline std::string Get(std::string _attributeName) const {
		return props.at(_attributeName);
	}

	template <>
	inline int Get(std::string _attributeName) const {
		return std::stoi(props.at(_attributeName));
	}
	template <>
	inline unsigned long Get(std::string _attributeName) const {
		return std::stoul(props.at(_attributeName));
	}

	template <>
	inline float Get(std::string _attributeName) const {
		return std::stof(props.at(_attributeName));
	}


	template <>
	inline bool Get(std::string _attributeName) const {
		std::string res = (props.at(_attributeName));
		std::transform(res.begin(), res.end(), res.begin(), [](char& c) {
			return std::tolower(c);
			}
		);
		return res == "true";
	}
};

using ConfigData = std::unordered_map<std::string, ConfigSection>;


ConfigData ParseConfigFile(std::filesystem::path _path);
void SaveConfigData(const ConfigData& _data, std::filesystem::path _path);