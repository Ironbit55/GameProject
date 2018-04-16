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
	};
	~Entity() {};

	void loadContent(ContentManager contentManager);

	void update();

	void onreceiveMessage(Message& msg) override{

		//check whether this is an input message
		//by seeing if the type of message matches any
		//mesage types defined in message actor map
		auto i = MESSAGE_TO_ACTOR_MAP.find(msg.messageType);
		if(i != MESSAGE_TO_ACTOR_MAP.end()){
			
			InputActors inputActor = i->second;

			InputMsgData* data = static_cast<InputMsgData*>(msg.dataPayload);
			handleInput(inputActor, data->input);
		}
	}

	bool registerInput(InputActors inputActor){

		//check this input actor is mapped to a message
		//inputActor is on the wrong side of the map but register input
		//gets called less than onreceiveMessage which needs it this way round
		for (auto i = MESSAGE_TO_ACTOR_MAP.begin(); i != MESSAGE_TO_ACTOR_MAP.end(); ++i) {
			if (inputActor == i->second) {
				//it is mapped so register for message actor is mapped to
				return addListener(i->first);
			}
		}

		return false;
		
	}

	
	bool disableInput(InputActors inputActor) {

		for (auto i = MESSAGE_TO_ACTOR_MAP.begin(); i != MESSAGE_TO_ACTOR_MAP.end(); ++i) {
			if (inputActor == i->second) {
				//input actor has message mapping

				//may not be registered for this kind of message
				//but will just return false and do nothing in that case so its fine
				return this->disableListener(i->first);
			}
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

};

