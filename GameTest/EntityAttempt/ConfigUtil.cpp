#include "ConfigUtil.h"
#include <iostream>


ConfigUtil::ConfigUtil()
{
}


ConfigUtil::~ConfigUtil()
{
}

void ConfigUtil::loadInputConfig() {
	auto config = cpptoml::parse_file(CONFIG_FOLDER_PATH + INPUT_CONFIG_FILE);

	addInputConfigMappings((config->get_table_array("buttons")));
	addInputConfigMappings(config->get_table_array("axis"));
	

	addInputConfigMappings(config->get_table_array("actions"));
	addInputConfigMappings(config->get_table_array("states"));
	addInputConfigMappings(config->get_table_array("ranges"));

}

void ConfigUtil::loadInputMappings(InputManager& inputManager) {
	auto configMappings = cpptoml::parse_file(CONFIG_FOLDER_PATH + INPUT_MAPPING_FILE);
	
	//the mapping file contains an entry for each input actor,
	//the entry specifies which input contexts they use
	auto inputActorEntries = configMappings->get_table_array("input_actors");

	
	//so we loop through all input actor entries and attach the specified input contexts to them
	for(const auto& inputActorEntry : *inputActorEntries){

		auto actorId = inputActorEntry->get_as<int>("actor_id");
		//check actor id is valid
		if (!actorId || !InputManager::isInputActor(*actorId)) {
			//either entry did not contain "actor_id" key or id is an invalid input actor
			//so just ignore this entry
			continue;
		}
		InputActors inputActor = InputManager::asInputActor(*actorId);

		std::cout << "\nactor entry: " << *actorId << "\n";
		//array of context keys, each key should point to a value in the config mapping file...
		auto inputActorContexts = inputActorEntry->get_array_of<std::string>("contexts");
		for (const auto& contextKey : *inputActorContexts) {
			auto contextTable = configMappings->get_table_qualified(contextKey);
			if (contextTable) {
				//key points to a valid table in mapping file
				
				//check it has a valid name
				//(alright really the input actor entry should specify the name attached to each
				//context, not the context itself...)
				auto name = contextTable->get_as<std::string>("name");
				if (!name || name->empty()) {
					//failed to load inpout context, no name
					continue;
				}

				std::cout << "context: " << *name << "\n";

				//use table to create an input context
				InputContext context(true);
				bool success = loadInputContext(*contextTable, context);

				if(success) {
					//atach context to inputActor with name
					inputManager.addInputContext(inputActor, *name, context);
				}
				

			}
		}

		inputManager.activateActor(inputActor);
	}
}

void ConfigUtil::addInputConfigMappings(std::shared_ptr<cpptoml::table_array> mappings) {
	if(!mappings){
		return;
	}
	for (const auto& table : *mappings) {
		const auto id = table->get_as<int>("id");
		const auto name = table->get_as<std::string>("name");

		if((id && name) && (!name->empty())){
			inputConstantsMap.insert(std::pair<std::string, int>(*name, *id));
		}
		
	}
}

bool ConfigUtil::loadInputContext(cpptoml::table& inputContextTable, InputContext& outInputContext) {
	
	auto mappings = inputContextTable.get_table_array("mappings");
	if(!mappings){
		return false;
	}

	for (const auto& table : *mappings) {
		const auto raw = table->get_as<std::string>("raw");
		const auto cooked = table->get_as<std::string>("cooked");
		if (raw && cooked) {
			//mapping contains expected keys
			auto rawInputIt = inputConstantsMap.find(*raw);
			auto cookedInputIt = inputConstantsMap.find(*cooked);

			//could add fun stuff like sensitivity and deadzone here

			if(rawInputIt != inputConstantsMap.end() && cookedInputIt != inputConstantsMap.end()){
				//raw and cooked input names map to input ids
				//now we try adding them to input manager

				//if these ids aren't valid, or make an invalid mapping
				//addMapping will complain but just ignore them
				outInputContext.addMapping(rawInputIt->second, cookedInputIt->second);
				
			}
		}


	}

	auto deadzone = inputContextTable.get_as<double>("deadzone");
	if (deadzone) {
		outInputContext.addDeadzone(*deadzone);
	}

	return true;

}
