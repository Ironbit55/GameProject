#include "MessagingService.h"



MessagingService::MessagingService(){
	//start all callback ids at 1
	//so we can use 0 as invalid id
	for (int i = 0; i < MESSAGE_MAX_NUM_MESSAGES; ++i){
		messageTypeCallbackNextId[i] = 1;
	}
}


MessagingService::~MessagingService(){
	//pool will deconstruct itself fine

}

bool MessagingService::pushMessage(Message& msg){
	if(msg.messageType == MESSAGE_MAX_NUM_MESSAGES || msg.messageType == MESSAGE_EMPTY){
		//we don't deliver "MESSAGE_EMPTY" messages so no point allowing them to be pushed
		return false;
	}
	if(msg.dataPayload != nullptr){
		if(msg.dataSize == 0 || msg.dataSize > MSG_DATA_MAX_BYTES){
			//if we are given a data payload, the size has to be less than the max 
			//and also not zero or we can't put it in queue
			return false;
		}
	}
	
	if(numMessages > MESSAGE_QUEUE_CAPACITY){
		//no space in queue :(
		return false;
	}

	//all good lets add the message to queue
	//we are allowing messages with no payload... dunno if this is a good idea.

	//if we wanted to support immediete messages could call callback here and
	//not bother adding to queue

	//copy msg into queue. now messaging service is resposible for its life span
	//asigns the array element to the message pointed to by the reference msg 
	messageQueue[numMessages] = msg;

	//we copy the data payload from wherever it is at the moment on the stack
	//into our memory pool
	//now the original data can go out of scope or whatever now we have
	//a copy it doesn't matter
	if (msg.dataPayload != nullptr) { //if the message has no data, don't bother allocating it space in the pool
		void *msgDataDest = pool.allocate();
		void* msgDataSource = msg.dataPayload;

		//if the given size is wrong we're gunno have a bad time
		memcpy(msgDataDest, msgDataSource, msg.dataSize);
		//make sure the message saved on queue now points to the allocated data
		messageQueue[numMessages].dataPayload = msgDataDest;
	}
	numMessages++;

	return true;
}

bool MessagingService::registerListener(MessageType msgType, MessageCallbackWrapper& callback){
	if(messageTypeNumCallbacks[msgType] >= CALLBACKS_CAPACITY){
		return false;
	}
	callback.callbackId = messageTypeCallbackNextId[msgType];
	messageTypeCallbackNextId[msgType]++;
	messageTypeCallbacks[msgType][messageTypeNumCallbacks[msgType]] = callback;
	messageTypeNumCallbacks[msgType]++;
	return true;
}

bool MessagingService::deregisterListener(MessageType msgType, int callbackId){
	for (int i = 0; i < messageTypeNumCallbacks[msgType]; i++){
		if(messageTypeCallbacks[msgType][i].callbackId == callbackId){
			//found the callback so remove it

			//the question is how though this is awkward because
			//of the horrible way we are storing callbacks
			//set callback to nullptr and then we are left with a hole that won't be filled
			//yaay, so solution is shift elements after the callback we want to delete down one spot
			//OR since order doesn't matter (I hope) we can just copy callback at tail to fill the hole

			//delete callback creating hole in array
			messageTypeCallbacks[msgType][i].callback = nullptr;
			int numCallbacks = messageTypeNumCallbacks[msgType];
			//if we only had one callback we don't need to worry
			if(numCallbacks > 1){
				//fill hole with tail
				messageTypeCallbacks[msgType][i] = messageTypeCallbacks[msgType][numCallbacks];
				//delete tail
				messageTypeCallbacks[msgType][numCallbacks].callback = nullptr;
			}

			//we now have one less callback in array
			messageTypeNumCallbacks[msgType]--;

			return true;
		}
	}

	//couldn't find the callback
	return false;
}

void MessagingService::dispatchMessage(Message& msg){

	for (int i = 0; i < messageTypeNumCallbacks[msg.messageType]; i++){
		//perform the callback
		if (messageTypeCallbacks[msg.messageType][i].callback != nullptr) {
			(messageTypeCallbacks[msg.messageType][i].callback)(msg);
		}
	}
}


void MessagingService::dispatchMessages(){
	//sorted so smallest dispatch time at end of array
	std::sort(messageQueue.begin(), messageQueue.begin() + numMessages, cmpMsg);
	int dispatchListCount = 0;

	//need to create a buffer of messages to dispatch so that callbacks triggered
	//by messages can add messages to the queue without disrupting the dispatch

	for (int i = 0; i < numMessages; ++i){
		if(messageQueue[i].timeUntillDispatch < 0 + FLT_EPSILON){
			//copy messages to dispath into dispatch buffer
			//this is from o to max
			messageDispatchBuffer[i] = messageQueue[i];

			//delete / deallocate the message from queue
			//this isn't actually neccesary as message will get ovverriten below
			messageQueue[i] = Message();
			dispatchListCount++;
		}else{
			//as array is sorted when we hit a time untill dispatch greater than 0
			//we know msgs further down are also going to be greater than 0
			//so don't bother searching any further
			break;
		}
	}
	
	//go through copy of queue and dispatch messages
	//new messages can be added to the main queue during this with no problems
	for (int i = 0; i < dispatchListCount; ++i){

		if (messageDispatchBuffer[i].messageType != MESSAGE_EMPTY) {
			dispatchMessage(messageDispatchBuffer[i]);

			//dealocate message data if it had any
			if (messageDispatchBuffer[i].dataPayload != nullptr) {
				pool.deallocate((messagedatasize_t*)messageDispatchBuffer[i].dataPayload);
			}
		}
	}


	//move undispatched messages to front of queue
	//this ovverites the dispatched messages
	for (int i = dispatchListCount; i < numMessages; ++i){
		messageQueue[i - dispatchListCount] = messageQueue[i];
	}

	//we have less messages as we dispatched some
	numMessages -= dispatchListCount;
}

void MessagingService::update(float msec){
	for (int i = 0; i < numMessages; ++i){
		messageQueue[i].timeUntillDispatch -= msec;
	}
	//for (int i = numMessages; i > 0; --i) {
	//	messageQueue[i].timeUntillDispatch -= msec;
	//}
}
