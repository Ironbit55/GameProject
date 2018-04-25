#pragma once

#include "../Input/InputMapper.h"
#include "../GameTest/MessageReceiver.h"
#include "../Input/InputManager.h"
#include <iostream>
class InputReceiver : public MessageReceiver
{
public:
	InputReceiver() {
		inputMessageCallback = std::bind(&InputReceiver::onInputMessage, this, std::placeholders::_1);
		//inputMessageCallback = [this](Message& msg) {this->onInputMessage(msg); };
	}
	~InputReceiver();

	void onInputMessage(Message& msg) {
		//we are going to trust this is always an input message
		InputMsgData* data = static_cast<InputMsgData*>(msg.dataPayload);
		InputActors inputActor = data->actor;

		handleInput(inputActor, data->input);
	}

	bool registerInput(InputActors inputActor) {

		//find message type mapped to 
		auto i = ACTOR_TO_MESSAGE_MAP.find(inputActor);
		if (i != ACTOR_TO_MESSAGE_MAP.end()) {
			//add listener to this message type
			return addListener(i->second, inputMessageCallback);;
		}

		return false;

	}


	bool disableInput(InputActors inputActor) {
		//find message type mapped to 
		auto i = ACTOR_TO_MESSAGE_MAP.find(inputActor);
		if (i != ACTOR_TO_MESSAGE_MAP.end()) {
			//add listener to this message type
			return this->disableListener(i->second);
		}
		return false;
	}

	void rebindCallback(){
		inputMessageCallback = [this](Message& msg) {this->onInputMessage(msg); };
		MessageReceiver::rebindCallback();
	}

	void unbindCallback() {
		inputMessageCallback = MessageCallback();
		MessageReceiver::unbindCallback();
		//memset(&inputMessageCallback, 0, sizeof(inputMessageCallback));
	}

	//gets called when received a message containing the input for a actor
	//the instance has registered to receive input from
	//inputActor specified which actor the input belongs to
	virtual void handleInput(InputActors inputActor, MappedInput* mappedInput) {}

private:
	MessageCallback inputMessageCallback;
};

