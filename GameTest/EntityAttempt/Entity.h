#pragma once
#pragma once
#include "../GameTest/ContentManager.h"
#include "../Input/InputMapper.h"
#include "../GameTest/MessageReceiver.h"
#include "../Input/InputManager.h"
#include <iostream>

class Entity : MessageReceiver
{
public:
	Entity(){
		registerInput(INPUT_ACTOR_PLAYER1);

		//create callback to this instances onInputMessage
		//this is the callback attached to input messages
		//and this way we don't use virtual function from MessageReciever
		//apperently some people think virtual function calls are the devil
		inputMessageCallback = std::bind(&Entity::onInputMessage, this, std::placeholders::_1);
	};
	~Entity() {};

	void loadContent(ContentManager contentManager);

	void update();


	void onInputMessage(Message& msg){
		//we are going to trust this is always an input message
		InputMsgData* data = static_cast<InputMsgData*>(msg.dataPayload);
		InputActors inputActor = data->actor;

		handleInput(inputActor, data->input);

	}

	bool registerInput(InputActors inputActor){

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

	//gets called when received a message containing the input for a actor
	//the instance has registered to receive input from
	//inputActor specified which actor the input belongs to
	virtual void handleInput(InputActors inputActor, MappedInput* mappedInput){
		
		if(mappedInput->getAction(InputCooked::ACTION_JUMP)){
			std::cout << "handle input jump!";
		}
	}


	//registers with Input manager as a callback
	//calls handleInput virtually
	void inputListener(MappedInput& mappedInput) {
		//handleInput(mappedInput);
	}


protected:
	vector<Entity> children;

private:
	MessageCallback inputMessageCallback;

};

