#pragma once
#include <string>
#include "GameTest/cpptoml.h"
#include "Input/InputManager.h"

const std::string CONFIG_FOLDER_PATH = "../../Config/";
const std::string INPUT_CONFIG_FILE = "input_config.toml";
const std::string INPUT_MAPPING_FILE = "input_mapping.toml";

const std::string TEXTURE_CONFIG_FILE = "texture_config.toml";
const std::string ENTITY_CONFIG_FILE = "entity_config.toml";
class ConfigUtil
{
public:
	ConfigUtil();
	~ConfigUtil();

	void loadInputConfig();
	void loadInputMappings(InputManager& inputManager);
protected:
	//contains input constants mapped to a name
	std::map<std::string, int> inputConstantsMap;

	void addInputConfigMappings(std::shared_ptr<cpptoml::table_array> mappings);
	bool loadInputContext(cpptoml::table& inputContext, InputContext& outInputContext);
};

