#include "Game5.h"
#include "../GameEngine/Collision/CircleCC.h"
#include "../GameEngine/Collision/RectCC.h"

constexpr const char* Game5::barsIcons[3];
constexpr const char* Game5::circlesIcons[3];
constexpr const char* Game5::circlesIconsPressed[3];
constexpr uint16_t Game5::barsX[3];
constexpr uint16_t Game5::circlesX[3];

std::map<uint, uint8_t> btnBarMap = {{ BTN_LEFT,  0 },
									 { BTN_RIGHT, 1 },
									 { BTN_ENTER, 2 }};

Game5::Game5() : Game("/Games/5", {
		{ barsIcons[0],           {}, true },
		{ barsIcons[1],           {}, true },
		{ barsIcons[2],           {}, true },
		{ circlesIcons[0],        {}, true },
		{ circlesIcons[1],        {}, true },
		{ circlesIcons[2],        {}, true },
		{ circlesIconsPressed[0], {}, true },
		{ circlesIconsPressed[1], {}, true },
		{ circlesIconsPressed[2], {}, true },
}), bottomWall(nullptr, std::make_unique<RectCC>(glm::vec2 { 160, 20 })){
	bottomWall.setPos({ 0, 138 });
}

void Game5::onStart(){
	Input::getInstance()->addListener(this);
}

void Game5::onStop(){
	Input::getInstance()->addListener(this);
}

void Game5::onLoad(){
	for(uint8_t i = 0; i < 3; ++i){
		bars[i] = std::make_shared<GameObject>(std::make_unique<StaticRC>(getFile(barsIcons[i]), PixelDim { 14, 115 }), nullptr);
		bars[i]->setPos({ barsX[i], 0 });
		addObject(bars[i]);

		circles[i] = std::make_shared<GameObject>(std::make_unique<StaticRC>(getFile(circlesIcons[i]), PixelDim { 18, 18 }),
												  std::make_unique<CircleCC>(9, glm::vec2 { 9, 9 }));
		circles[i]->getRenderComponent()->setLayer(1);
		circles[i]->setPos({ circlesX[i], circlesY });
		addObject(circles[i]);
	}
}

void Game5::onLoop(float deltaTime){
	updateNotes(deltaTime);
}

void Game5::onRender(Sprite* canvas){
}

void Game5::buttonPressed(uint i){
	if(!btnBarMap.count(i)) return;

	uint8_t circleIndex = btnBarMap[i];
	if(circlesPressed[circleIndex]) return;

	circlesPressed[circleIndex] = true;
	std::static_pointer_cast<StaticRC>(circles[circleIndex]->getRenderComponent())->setFile(getFile(circlesIconsPressed[circleIndex]));
}

void Game5::buttonReleased(uint i){
	if(!btnBarMap.count(i)) return;

	uint8_t circleIndex = btnBarMap[i];
	if(!circlesPressed[circleIndex]) return;

	circlesPressed[circleIndex] = false;
	std::static_pointer_cast<StaticRC>(circles[circleIndex]->getRenderComponent())->setFile(getFile(circlesIcons[circleIndex]));

}

void Game5::updateNotes(float delta){
	for(uint8_t i = 0; i < 3; ++i){
		for(auto& note : notes[i]){
			note->setPos({ circlesX[i], circles[i]->getPos().y + delta * notesSpeed });
		}
	}
}

void Game5::createNote(uint8_t track){
	uint8_t type = random() % 3;

	auto note = std::make_shared<GameObject>(std::make_unique<StaticRC>(getFile(notesIcons[type]), PixelDim { 8, 9 }),
											 std::make_unique<RectCC>(glm::vec2 { 8, 9 }));

	note->getRenderComponent()->setLayer(2);

	collision.addPair(*note, bottomWall, [this, note, track](){
		removeObject(note);
		notes[track].pop_front();
	});

	notes[track].push_back(note);
}
