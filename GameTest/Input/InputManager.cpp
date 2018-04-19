#include "InputManager.h"


InputManager::InputManager(){
}


InputManager::~InputManager()
{
}

void InputManager::loadMappings(){
	//should load from file
	//loadMappingsFromFile("");


}

void InputManager::clearRawInput(){
	for (int actor = 0; actor < InputActors::INPUT_ACTOR_MAX; actor++) {
		inputMappers[actor].clear();
	}
}

void InputManager::swapRawInput(){
	for (int actor = 0; actor < InputActors::INPUT_ACTOR_MAX; actor++) {
		inputMappers[actor].swap();
	}
}

void InputManager::clearMappedInput(){
	for (int actor = 0; actor < InputActors::INPUT_ACTOR_MAX; actor++) {
		mappedInputs[actor].clear();
	}
}

void InputManager::addInputContext(InputActors actor, std::string contextName, InputContext context){
	inputMappers[actor].addInputContext(contextName, context);
}

void InputManager::setButtonState(InputActors actor, InputRaw::Buttons button, bool buttonDown, bool buttonWasDown){
	inputMappers[actor].setButtonState(button, buttonDown, buttonWasDown);
}

void InputManager::addButton(InputActors actor, InputRaw::Buttons button, bool buttonDown) {
	inputMappers[actor].addButton(button, buttonDown);
}

void InputManager::addButtonDown(InputActors actor, InputRaw::Buttons button){
	inputMappers[actor].addButtonDown(button);
}

void InputManager::addButtonUp(InputActors actor, InputRaw::Buttons button){
	inputMappers[actor].addButtonUp(button);
}

void InputManager::addAxisValue(InputActors actor, InputRaw::Axes axis, float value){
	inputMappers[actor].addAxisValue(axis, value);
}

void InputManager::activateActor(InputActors actor){
	inputActorsActive[actor] = true;
}

void InputManager::deactivateActor(InputActors actor){
	inputActorsActive[actor] = false;
}



MappedInput& InputManager::getMappedInput(InputActors actor){
	return mappedInputs[actor];
}

void InputManager::performMapping(){
	for (int actor = 0; actor < InputActors::INPUT_ACTOR_MAX; actor++){
		if(inputActorsActive[actor]){
			inputMappers[actor].dispatch(mappedInputs[actor]);
		}
	}

	//send message for each mapped input

	Message msg;
	msg.messageType = MESSAGE_INPUT_PLAYER1;
	msg.timeUntillDispatch = 0;
	
	InputMsgData msgData;
	//yeah this is a bit sketchy
	msgData.input = &mappedInputs[INPUT_ACTOR_PLAYER1];
	msgData.actor = INPUT_ACTOR_PLAYER1;
	msg.dataPayload = &msgData;
	msg.dataSize = sizeof(InputMsgData);

	//map from input actor to message type
	//send message with mapped type
	for (auto i = ACTOR_TO_MESSAGE_MAP.begin(); i != ACTOR_TO_MESSAGE_MAP.end(); ++i) {
		
		InputActors inputActor = i->first;
		MessageType inputMessageType = i->second;

		if (!inputActorsActive[inputActor] || mappedInputs[inputActor].isEmpty()) {
			//don't need to send message if this actor isn't active
			//don't need to send input message containing empty input
			continue;
		}

		MessageType msgType = inputMessageType;
		//msg data input should point to this actors mapped input
		msgData.input = &mappedInputs[inputActor];
		msgData.actor = inputActor;

		MessagingService::instance().pushMessage(msg);
	}

}

void InputManager::registerListener(InputActors actor, InputCallback callback){
	inputListeners.insert(std::make_pair(callback, actor));
}
