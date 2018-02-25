#pragma once
struct InputRaw{
	//our raw input types

	static const int BUTTON_ID = 1000;
	enum Buttons{
		BUTTON_KEY_W = BUTTON_ID,
		BUTTON_KEY_A,
		BUTTON_KEY_S,
		BUTTON_KEY_D,
		
		
		MAX_BUTTON
	};

	static const int AXIS_ID = 2000;
	enum Axes{
		AXIS_PAD_X = AXIS_ID,
		AXIS_PAD_Y,
		
		
		MAX_AXIS
	};
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
		
		
		MAX_ACTION,
	};

	static const int STATE_ID = 4000;
	enum States
	{
		STATE_MOVE_UP = STATE_ID,
		STATE_MOVE_LEFT,
		STATE_MOVE_DOWN,
		STATE_MOVE_RIGHT,

		
		MAX_STATE,
	};

	static const int RANGE_ID = 5000;
	enum Ranges
	{
		RANGE_DIRECTION_VERTICAL = RANGE_ID,
		RANGE_DIRECTION_HORIZONTAL,


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