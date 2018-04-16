#pragma once
#include "InputMapper.h"
#include "../GameTest/MessagingService.h"

/*
 * Author: Ed Curran (b15021040)
 * Class: InputManager
 * Description: Manages collecting and mapping raw input on behalf of specified input actors
 * 
 * raw input can be pushed to the input manager and marked as belonging to a particular input actor
 * Each actor then maps its raw input into an InputMapped instance
 * Can then access the mapped input belonging to a particular actor 
 * callbacks can be registered to receive the mapped input from a particular actor (or combination of actors if i get round to that)
 * 
 * In reality this is basically just a wrapper around a collection of InputMappers
 * with each InputMapper corrosponding to an Actor
 * but I may change the implementation underneath pretty heavily so this is a pretty verbose wrapper because
 * 
 * E N C A P S U L A T I O N
 * N
 * C
 * A
 * P
 * S
 * U
 * L
 * A
 * T
 * I
 * N
 * 
 * As at this level we are exclusively working with our engine defined raw input
 * it means the input manager / mapping system is independent of any particular input library / collection method
 * 
 * Extend this class to facilitate collecting a wide range of input
 * eg could be extended to take sdlKey inputs
 * or nclgl keyboard 
 * 
 *  
 * 
 */
enum InputActors{
	INPUT_ACTOR_PLAYER1,
	INPUT_ACTOR_PLAYER2,
	INPUT_ACTOR_PLAYER3,
	INPUT_ACTOR_PLAYER4,
	INPUT_ACTOR_MAX,
};

//const std::map<InputActors, MessageType> ACTOR_TO_MESSAGE_MAP = {
//	{ INPUT_ACTOR_PLAYER1, MESSAGE_INPUT_PLAYER1 },
//	{ INPUT_ACTOR_PLAYER2, MESSAGE_INPUT_PLAYER2 },
//	{ INPUT_ACTOR_PLAYER3, MESSAGE_INPUT_PLAYER3 },
//	{ INPUT_ACTOR_PLAYER4, MESSAGE_INPUT_PLAYER4 }
//};

const std::map<MessageType, InputActors> MESSAGE_TO_ACTOR_MAP = {
	{ MESSAGE_INPUT_PLAYER1, INPUT_ACTOR_PLAYER1 },
	{ MESSAGE_INPUT_PLAYER2, INPUT_ACTOR_PLAYER2 },
	{ MESSAGE_INPUT_PLAYER3, INPUT_ACTOR_PLAYER3 },
	{ MESSAGE_INPUT_PLAYER4, INPUT_ACTOR_PLAYER4 }
};


struct InputMsgData {
	//InputManager manages the memory of the mapped inputs
	//so just use a pointer to it
	InputActors actor;
	MappedInput* input;
};

class InputManager
{
public:
	InputManager();
	~InputManager();

	void loadFromFile(std::string fileName);

	void clearRawInput();
	void swapRawInput();
	void clearMappedInput();

	// define our input callback type to call on dispatch
	typedef void(*InputCallback)(MappedInput& inputs);

	//each actor (input mapper) can still have multiple input contexts
	//This might end up being overkill but
	//actors are a seperate concept from input contexts
	//eg actor 1 may have weapon a equiped and actor 2 weapon b
	//and so have different input contexts

	void addInputContext(InputActors actor, std::string contextName, InputContext context);

	void activateActor(InputActors actor);
	void deactivateActor(InputActors actor);
	/*
	 * Send raw input to actors
	 */

	void setButtonState(InputActors actor, InputRaw::Buttons button, bool buttonDown, bool buttonWasDown);
	void addButton(InputActors actor, InputRaw::Buttons button, bool buttonDown);
	void addButtonDown(InputActors actor, InputRaw::Buttons button);
	void addButtonUp(InputActors actor, InputRaw::Buttons button);
	void addAxisValue(InputActors actor, InputRaw::Axes axis, float value);

	/*
	 * get resultant cooked input from actor
	 */
	MappedInput& getMappedInput(InputActors actor);

	/*
	 * map raw input to cooked input
	 */
	void performMapping();

	/*
	 * I don't really know how this is gunno work yet
	 * TODO: figure out how we're gunno implement this
	 */
	void registerListener(InputActors actor, InputCallback callback);

protected:
	InputMapper inputMappers[INPUT_ACTOR_MAX];

	//i want these to be in a contigous block
	//not mixed in with InputMapper objects
	//so inputmanager is responsible for managing these
	//and makes calls to the relevent mapped to fill them up
	MappedInput mappedInputs[INPUT_ACTOR_MAX];
	bool inputActorsActive[INPUT_ACTOR_MAX];

	//std::vector<InputCallback> inputListeners;
	std::map<InputCallback, InputActors> inputListeners;
};

