#include "State.h"

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
	if(parent == nullptr) return;
	stop();
	parent->start();
	delete this;
}
