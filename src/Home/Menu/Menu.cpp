#include "Menu.h"
#include <Display/Sprite.h>
#include <Loop/LoopManager.h>
#include <SPIFFS.h>
#include <SD.h>

MenuItem::MenuItem(String text, const GameImage& image, std::function<void()> primary, std::function<void()> secondary) :
					text(std::move(text)), image(image), primary(primary), secondary(secondary){}


Menu::Menu(Sprite* canvas) : canvas(canvas), origin((canvas->width() - width) / 2){

}

Menu::Menu(Sprite* canvas, std::vector<MenuItem>& items) : Menu(canvas) {
	setItems(items);
}

Menu::~Menu(){

}

void Menu::setItems(std::vector<MenuItem>& items){
	this->items = items;

	for(auto& item : this->items){
		item.image.setX(-width);
		item.image.setY(originY);
	}

	repos();
	setOffsetY(64); //used for hiding the menu

	selectedGame = 0;
}

void Menu::setOffsetY(uint8_t y){
	offsetY = y;
	for(auto& item : items){
		item.image.setY(originY + offsetY);
	}
}

void Menu::splash(float f){
	if(items.size() < 4) return;

	getCGame()->setX(f * (float) (origin + width) - width);
	getLGame()->setX(f * (float) (origin + width) - 2 * width - gutter);
	getRGame()->setX(f * (float) (origin + width + gutter - (origin + 2*width + 2*gutter)) + origin + 2*width + 2*gutter);
}

void Menu::load(float f){
	if(items.size() < 4) return;

	getCGame()->setY((f * -7.0f + originY));

	f = 1.0f - f;
	getLGame()->setX(f * (float) (origin + width) - 2 * width - gutter);
	getRGame()->setX(f * (float) (origin + width + gutter - (origin + 2*width + 2*gutter)) + origin + 2*width + 2*gutter);
}

void Menu::finish(float f){
	if(items.size() < 4) return;

	getCGame()->setY((f * (120.0f - originY + 7.0f) + originY - 7.0f));
}

void Menu::reset(){
	selectedGame = 0;
	delta = 0;
	queued = false;

	LoopManager::removeListener(this);

	repos();
}

void Menu::repos(){
	for(auto& item : items){
		item.image.setX(-width);
		item.image.setY(originY);
	}

	if(items.size() < 4) return;

	getLGame()->setX(origin - width - gutter);
	getCGame()->setX(origin);
	getRGame()->setX(origin + width + gutter);
}

uint8_t Menu::prev(){
	if(items.size() < 4) return 0;

	if(scrolling() && direction == PREV){
		multiplier = min(2, multiplier+1);
		queued = true;
		return (selectedGame < 2) ? items.size() - 2 + selectedGame : selectedGame - 2;
	}

	direction = PREV;

	if(scrolling()){ // direction == NEXT
		selectNext();
		delta = (float) width + (float) gutter - delta;
		queued = false;
	}else{
		delta = 1;
		multiplier = 1;
		getLLGame()->setX(origin - 2 * width - 2 * gutter);
		LoopManager::addListener(this);
	}

	return (selectedGame < 1) ? items.size() - 1 : selectedGame - 1;
}

uint8_t Menu::next(){
	if(items.size() < 4) return 0;

	if(scrolling() && direction == NEXT){
		multiplier = min(2, multiplier+1);
		queued = true;
		return (selectedGame + 2) % items.size();
	}

	direction = NEXT;

	if(scrolling()){ // direction == PREV
		selectPrev();
		delta = (float) width + (float) gutter - delta;
		queued = false;
	}else{
		delta = 1;
		multiplier = 1;
		getRRGame()->setX(origin + 2 * width + 2 * gutter);
		LoopManager::addListener(this);
	}

	return (selectedGame + 1) % items.size();
}

void Menu::push(){
	if(items.size() < 4) return;
	canvas->drawRoundRect(origin-1, originY+offsetY-1, width+2, width+2, 2, TFT_BLACK);
	getCGame()->draw();
	getRGame()->draw();
	getLGame()->draw();

	if(scrolling()){
		if(direction == NEXT){
			getRRGame()->draw();
		}else{
			getLLGame()->draw();
		}
	}
}

void Menu::loop(uint micros){
	if(items.size() < 4){
		reset();
		LoopManager::removeListener(this);
		return;
	}

	delta += speed * (micros / 1000000.0f) * (float) multiplier;

	if(direction == PREV){
		getLLGame()->setX(origin - 2 * width - 2 * gutter + delta);
		getLGame()->setX(origin - width - gutter + delta);
		getCGame()->setX(origin + delta);
		getRGame()->setX(origin + width + gutter + delta);
	}else{
		getRRGame()->setX(origin + 2 * width + 2 * gutter - delta);
		getRGame()->setX(origin + width + gutter - delta);
		getCGame()->setX(origin - delta);
		getLGame()->setX(origin - width - gutter - delta);
	}

	if(delta >= (width + gutter)){
		if(direction == NEXT){
			selectNext();
		}else{
			selectPrev();
		}

		if(queued){
			queued = false;
			delta = 1;
			return;
		}

		LoopManager::removeListener(this);
		delta = 0;
		repos();
	}
}

bool Menu::scrolling(){
	return delta != 0;
}

void Menu::selectNext(){
	selectedGame = (selectedGame + 1) % items.size();
}

void Menu::selectPrev(){
	if(selectedGame == 0){
		selectedGame = items.size() - 1;
	}else{
		selectedGame--;
	}
}

GameImage* Menu::getCGame(){
	return &items[selectedGame].image;
}

GameImage* Menu::getLGame(){
	if(selectedGame == 0) return &items.back().image;
	return &items[selectedGame - 1].image;
}

GameImage* Menu::getRGame(){
	return &items[(selectedGame + 1) % items.size()].image;
}

GameImage* Menu::getLLGame(){
	if(selectedGame <= 1) return &items[items.size() - 2 + selectedGame].image;
	return &items[selectedGame - 2].image;
}

GameImage* Menu::getRRGame(){
	return &items[(selectedGame + 2) % items.size()].image;
}

void Menu::setCanvas(Sprite* canvas){
	Menu::canvas = canvas;

	for(auto& item : items){
		if(!item.image) continue;
		item.image.setCanvas(canvas);
	}
}

uint Menu::getSelectedIndex() const{
	return selectedGame;
}
