/*
 * Author: Edward Curarn
 * Class: MessageReceiver
 * 
 * Used to register callbacks to be called on messages of certain types by the messaging service
 * 
 * Derived classes must remember to call deleteAllListeners() on destruct
 * this isn't done virtually because it breaks my horrible entity system
 */

#pragma once
#include <unordered_map>
#include "MessagingService.h"

class MessageReceiver
{
public:
	MessageReceiver(){
		baseMessageCallback = std::bind(&MessageReceiver::onreceiveMessage, this, std::placeholders::_1);
	};
	//deregister callbacks
	virtual ~MessageReceiver(){
		deleteAllListeners();
	}
protected:

	MessageCallback baseMessageCallback;
	/*
	 * add listener to map and register with messaging service
	 */
	bool addListener(MessageType msgType, MessageCallback callback){
		MessageCallbackWrapper callbackWrapper;
		callbackWrapper.callback = callback;
		callbackWrapper.callbackId = 0;
		
		if(MessagingService::instance().registerListener(msgType, callbackWrapper)){
			//succesfully registered
			//add returned id to map 
			if((listeners.insert(std::make_pair(msgType, callbackWrapper))).second){
				//insertion succesfull
				return true;
			}
			//listener for this message has already been registered by this receiver

		}


		return false;
	}

	/*
	* add listener to map and register with messaging service
	*/
	bool addListener(MessageType msgType) {
		return addListener(msgType, baseMessageCallback);
	}

	/*
	 * remove from map and derigister from messaging service
	 */
	bool deleteListener(MessageType msgType){
		bool success = false;
		auto it = listeners.find(msgType);
		if(it != listeners.end()){
			//listener exists in map so try and deregister it
			success = MessagingService::instance().deregisterListener(msgType, it->second.callbackId);
			//either success or not registered anyway so remove from local map
			listeners.erase(it);
		}

		return false;
	}

	void deleteAllListeners(){
		//deregister all 
		for(auto listener : listeners) { //well this is some fun syntax
			MessagingService::instance().deregisterListener(listener.first, listener.second.callbackId);
		}
		//clear map
		listeners.clear();
	}

	/*
	 * 	kepp listener in receivers map, but deregister from messaging service
	 * 	so will no longer receive messages
	 * 	use this as just a little optimisation to save memory allocations
	 */
	bool disableListener(MessageType msgType){
		bool success = false;
		auto it = listeners.find(msgType);
		if (it != listeners.end() && it->second.callbackId != 0) { //id = 0 means listener isn't registered
			//listener exists in map
			success = MessagingService::instance().deregisterListener(msgType, it->second.callbackId);
			//flag id as invalid by setting to 0
			it->second.callbackId = 0;

			return success;
		}
		return false;
	}
	/*
	 * register already existing listener with messaging service
	 * to resume recieving messages
	 * this will ovverwrite the id 
	 */
	bool enableListener(MessageType msgType){
		auto it = listeners.find(msgType);
		bool success = false;
		if (it != listeners.end() && it->second.callbackId == 0){
			success = MessagingService::instance().registerListener(msgType, it->second);
		}

		return success;

	}

	virtual void onreceiveMessage(Message& msg) {};


	//not actually neccesary really but maybe helpfull
	void pushMessage(Message msg){
		MessagingService::instance().pushMessage(msg);
	}

private:
	//yeah this is gunno allocate memory dynamically here and there but not much of it and rarely
	//can only have one listener per message type, we need to keep track of the id of registered
	//listeners so we can deregister them, this could just map the int id instead of the wrapper
	//but i thought this may be usefull in the future if we change implementation of the wrapper or something
	std::unordered_map<MessageType, MessageCallbackWrapper> listeners;
};

