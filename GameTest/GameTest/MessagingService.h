/*
 *
 * Design inspired by: http://afloatingpoint.blogspot.co.uk/2012/09/gameplay-architecture-part-2-message.html
 */
#pragma once
#include <list>
#include <array>
#include "../MemoryManagement/MemoryPool.h"
#include <functional>

enum MessageType{
	MESSAGE_EMPTY,
	MESSAGE_AUDIO_EFFECT,
	MESSAGE_AUDIO_MUSIC,
	
	MESSAGE_INPUT_PLAYER1,
	MESSAGE_INPUT_PLAYER2,
	MESSAGE_INPUT_PLAYER3,
	MESSAGE_INPUT_PLAYER4,

	MESSAGE_CONTROLLER_RUMBLE,

	MESSAGE_FIRE_PROJECTILE,

	MESSAGE_MAX_NUM_MESSAGES,
};
const unsigned int IMMEDIATE = 0;

struct Message{
	MessageType messageType;
	void* dataPayload;
	unsigned int dataSize;
	unsigned int flags;
	float timeUntillDispatch; //time in ms, counts down by delta time of every update - message dispatched at zero
};




//we use std::function as it allows us to deal with member function pointers way easier
//however comparing std::function instances isn't allowed. 
//we only want to compare the address to check if it is the same so can
//i tried using a hacky solution to find the address but that was a nightmare so
//we use the probably proper solution which is assign a unique id to each callback when
//its registered, and use that to find the callback to delete it
//but then the client is reponsible for storing its id's 
typedef std::function<void(Message&)> MessageCallback;

struct MessageCallbackWrapper {
	MessageCallback callback;
	unsigned int callbackId;
};

const unsigned int MSG_DATA_MAX_BYTES = 32;
typedef unsigned char messagedatasize_t[MSG_DATA_MAX_BYTES];
const unsigned int MESSAGE_QUEUE_CAPACITY = 32;
const unsigned int CALLBACKS_CAPACITY = 8;



class MessagingService
{
public:
	MessagingService();
	~MessagingService();

	/*
	 * use a singleton cus im a terrible person
	 */
	static MessagingService& instance() {
		static MessagingService msgServiceSingleton;
		return msgServiceSingleton;
	}


	//add message to queue and copy the payload data to dynamic memory / heap
	bool pushMessage(Message& msg);


	/*
	 * will update the given callback wrapper with id if succesfully registered
	 */
	bool registerListener(MessageType msgType, MessageCallbackWrapper& callback);

	/*
	 * this is expensive
	 */
	bool deregisterListener(MessageType msgType, int callbackId);

	//call the callbacks associated with this message
	void dispatchMessage(Message& msg);


	//sort then dispatch ready to deliver messages to registered callbacks
	void dispatchMessages();

	//update timeToDispatch for each message
	void update(float msec);

	static bool cmpMsgDispatchTime(Message a, Message b) {
		return a.timeUntillDispatch < b.timeUntillDispatch;
	}

	
	static bool cmpMsgByType(Message a, Message b) {
		return a.messageType < b.messageType;
	}

	//compare primarily by dispatch time and secondary by MessageType 
	static bool cmpMsg(Message a, Message b) {
		return a.timeUntillDispatch < b.timeUntillDispatch ||
			((a.timeUntillDispatch == b.timeUntillDispatch) && (a.messageType < b.messageType));
	}


private:

	//we define the max size of this at runtime.
	//so the memory is only allocated once and we aren't
	//newing memory all over the place
	//the proper way to do this would be to use a custom pool allocator
	//then we could resize it as much as desired
	//i might get around to doing that someday, linked list would probably easier than implementing
	//an std::vector
	
	std::array<Message, MESSAGE_QUEUE_CAPACITY> messageQueue;
	std::array<Message, MESSAGE_QUEUE_CAPACITY> messageDispatchBuffer;
	
	
	//we have to keep track of the *real* tail
	//this will grow and shrink as messages are pushed and dispatched
	unsigned int numMessages;

	//array to hold callbacks
	//we need an array per message type
	//as multiple listeners can register to one message type
	//im not tooo worried about memory for this because its faiirly static... hopefully

	MessageCallbackWrapper messageTypeCallbacks[MESSAGE_MAX_NUM_MESSAGES][CALLBACKS_CAPACITY];
	//yeah this is super obnoxious
	int messageTypeNumCallbacks[CALLBACKS_CAPACITY];
	
	int messageTypeCallbackNextId[MESSAGE_MAX_NUM_MESSAGES];

	//pool allocator
	//the lazy way which uses more memory is to have one pool with a set size 
	//each payload data takes a node. this wastes memory as each payload data has varrying sizes
	//and is unlikely to take up a full node
	//but this i what i'm gunno use for now
	//more complicated way is have a pool for each message type / payload data type
	//then we don't waste memory

	//This pool allocator isn't quite what we want. It grows and shrinks dynamically by allocating new blocks
	//of given blocksize when the current block has run out which we don't want. but if we just set blocksize to the max we need
	//it should be all good
	MemoryPool<messagedatasize_t, sizeof(messagedatasize_t) * MESSAGE_QUEUE_CAPACITY> pool;
};

