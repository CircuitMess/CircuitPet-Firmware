#include "State.h"
#include <Arduino.h>

State::State(State* parent) : parent(parent){

}

void State::start(){
	if(started) return;
	started = true;
	onStart();
}

void State::stop(){
	if(!started) return;
	started = false;
	onStop();
}

void State::push(State* parent){
	parent->stop();
	this->parent = parent;
	start();
}

void State::pop(){
	auto volatile p = parent;
	if(parent == nullptr) return;
	stop();
	delete this;
	p->start();
}

bool State::isStarted() const{
	return started;
}

State* State::getParent(){
	return parent;
}
