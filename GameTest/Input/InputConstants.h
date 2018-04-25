#pragma once
struct InputRaw{
	//our raw input types

	static const int BUTTON_ID = 1000;
	enum Buttons {
		BUTTON_KEY_W = BUTTON_ID,
		BUTTON_KEY_A,
		BUTTON_KEY_S,
		BUTTON_KEY_D,
		BUTTON_KEY_R,
		BUTTON_KEY_M,


		BUTTON_CONTROLLER_BUTTON_A,
		BUTTON_CONTROLLER_BUTTON_B,
		BUTTON_CONTROLLER_BUTTON_X,
		BUTTON_CONTROLLER_BUTTON_Y,
		BUTTON_CONTROLLER_BUTTON_DPAD_UP,
		BUTTON_CONTROLLER_BUTTON_DPAD_DOWN,
		BUTTON_CONTROLLER_BUTTON_DPAD_LEFT,
		BUTTON_CONTROLLER_BUTTON_DPAD_RIGHT,
		
		
		MAX_BUTTON
	};

	static int getButtonIndex (Buttons button) {
		return button - BUTTON_ID;
	}

	static const int AXIS_ID = 2000;
	enum Axes{
		AXIS_PAD_X = AXIS_ID,
		AXIS_PAD_Y,
		
		AXIS_CONTROLLER_AXIS_LEFTX,
		AXIS_CONTROLLER_AXIS_LEFTY,
		
		MAX_AXIS
	};

	static int getAxisIndex(Axes axis) {
		return axis - AXIS_ID;
	}

	static const int END_ID = 3000;


	//helpfull functions

	static bool isButton(const int inputId){
		return inputId >= BUTTON_ID && inputId < AXIS_ID;
	}

	static bool isAxis(const int inputId){
		return inputId >= AXIS_ID && inputId < END_ID;
	}

	constexpr static int getNumButtons() {
		return MAX_BUTTON - BUTTON_ID;
	}


	constexpr static int getNumAxes() {
		return MAX_AXIS - AXIS_ID;
	}
};

struct InputCooked {
	//cooked (mapped) input types

	static const int ACTION_ID = 3000;
	enum Actions
	{
		ACTION_JUMP  = ACTION_ID,
		ACTION_CONTROLLER_X,
		ACTION_TOGGLE_MUSIC,
		ACTION_RESET,
		
		
		MAX_ACTION,
	};


	static const int STATE_ID = 4000;
	enum States
	{
		STATE_CAMERA_MOVE_UP = STATE_ID,
		STATE_CAMERA_MOVE_LEFT,
		STATE_CAMERA_MOVE_DOWN,
		STATE_CAMERA_MOVE_RIGHT,

		STATE_PLAYER_MOVE_UP,
		STATE_PLAYER_MOVE_LEFT,
		STATE_PLAYER_MOVE_DOWN,
		STATE_PLAYER_MOVE_RIGHT,

		STATE_CONTROLLER_A,
		STATE_CONTROLLER_X,
		
		MAX_STATE,
	};

	static const int RANGE_ID = 5000;
	enum Ranges
	{
		RANGE_DIRECTION_VERTICAL = RANGE_ID,
		RANGE_DIRECTION_HORIZONTAL,

		RANGE_CONTROLLER_LEFT_X,
		RANGE_CONTROLLER_LEFT_Y,


		MAX_RANGE,
	};
	static const int END_ID = 6000;


	//helpfull functions

	static bool isAction(const int inputId) {
		return inputId >= ACTION_ID && inputId < STATE_ID;
	}

	static bool isState(const int inputId) {
		return inputId >= STATE_ID && inputId < RANGE_ID;
	}

	static bool isRange(const int inputId) {
		return inputId >= RANGE_ID && inputId < END_ID;
	}

	constexpr static int getNumActions() {
		return MAX_ACTION - ACTION_ID;
	}

	constexpr static int getNumStates(){
		return MAX_STATE - STATE_ID;
	}

	constexpr static int getNumRanges(){
		return MAX_RANGE - RANGE_ID;
	}

};