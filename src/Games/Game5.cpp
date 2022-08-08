#include "Game5.h"
#include "../GameEngine/Collision/CircleCC.h"
#include "../GameEngine/Collision/RectCC.h"
#include "../GameEngine/Rendering/SpriteRC.h"
#include "../GameEngine/Rendering/StaticRC.h"
#include <Util/GIF.h>

constexpr const char* Game5::barsIcons[4];
constexpr const char* Game5::circlesIcons[3];
constexpr const char* Game5::circlesIconsPressed[3];
constexpr uint16_t Game5::barsX[3];
constexpr uint16_t Game5::circlesX[3];
constexpr const char* Game5::notesIcons[3];
constexpr const char* Game5::danceGIFs[4];

std::map<uint, uint8_t> btnBarMap = {{ BTN_LEFT,  0 },
									 { BTN_RIGHT, 1 },
									 { BTN_ENTER, 2 }};

Game5::Game5() : Game("/Games/5", {
		{ barsIcons[0],           {}, true },
		{ barsIcons[1],           {}, true },
		{ barsIcons[2],           {}, true },
		{ barsIcons[3],           {}, true },
		{ circlesIcons[0],        {}, true },
		{ circlesIcons[1],        {}, true },
		{ circlesIcons[2],        {}, true },
		{ circlesIconsPressed[0], {}, true },
		{ circlesIconsPressed[1], {}, true },
		{ circlesIconsPressed[2], {}, true },
		{ notesIcons[0],          {}, true },
		{ notesIcons[1],          {}, true },
		{ notesIcons[2],          {}, true },
		{ "/bg.raw",          {}, true },
		{ danceGIFs[0],          {}, false },
		{ danceGIFs[1],          {}, false },
		{ danceGIFs[2],          {}, false },
		{ danceGIFs[3],          {}, false },
		{ "/fail.gif",            {}, false },
		{ "/idle.gif",            {}, true },
		{ "/win.gif",            {}, false },
		{ "/starFull.raw",            {}, true },
		{ "/starEmpty.raw",            {}, true },
		RES_HEART}),
				 bottomWall(nullptr, std::make_unique<RectCC>(glm::vec2 { 160, 20 })),
				 scoreBar(std::make_shared<GameObject>(std::make_unique<SpriteRC>(PixelDim { 7, 110 }), nullptr)){

	bottomWall.setPos({ 0, 138 });
	addObject(std::make_shared<GameObject>(bottomWall));

	scoreBar->setPos({ 146, 14 });
	scoreBarSprite = std::static_pointer_cast<SpriteRC>(scoreBar->getRenderComponent())->getSprite();
	scoreBarSprite->clear(TFT_BLACK);
	scoreBarSprite->drawRect(0, 0, scoreBarSprite->width(), scoreBarSprite->height(), TFT_WHITE);
	addObject(scoreBar);
}

void Game5::onStart(){
	Input::getInstance()->addListener(this);
	duckRC->start();
}

void Game5::onStop(){
	Input::getInstance()->removeListener(this);
	duckRC->stop();
}

void Game5::onLoad(){
	auto bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/bg.raw"), PixelDim{160, 128}), nullptr);
	bg->getRenderComponent()->setLayer(-1);
	addObject(bg);

	scoreStar = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/starEmpty.raw"), PixelDim{17, 13}), nullptr);
	scoreStar->setPos({141, 4});
	scoreStar->getRenderComponent()->setLayer(1);
	addObject(scoreStar);

	hearts = std::make_unique<Hearts>(getFile(FILE_HEART));
	hearts->getGO()->setPos({ 113, 4 });
	addObject(hearts->getGO());

	for(uint8_t i = 0; i < 3; ++i){
		bars[i] = std::make_shared<GameObject>(std::make_unique<StaticRC>(getFile(barsIcons[i]), PixelDim { 14, 115 }), nullptr);
		bars[i]->setPos({ barsX[i], barsY });
		addObject(bars[i]);

		circles[i] = std::make_shared<GameObject>(std::make_unique<StaticRC>(getFile(circlesIcons[i]), PixelDim { 18, 18 }),
												  std::make_unique<CircleCC>(9, glm::vec2 { 9, 9 }));
		circles[i]->getRenderComponent()->setLayer(1);
		circles[i]->setPos({ circlesX[i], circlesY });
		addObject(circles[i]);
	}

	duck = std::make_shared<GameObject>(std::make_unique<AnimRC>(getFile("/idle.gif")), nullptr);
	duck->setPos({ 65, 50 });
	duckRC = std::static_pointer_cast<AnimRC>(duck->getRenderComponent());
	addObject(duck);
}

void Game5::onLoop(float deltaTime){
	switch(state){
		case Running:
			beatTimer += deltaTime;
			if(beatTimer >= beatInterval){
				beatTimer = 0;
				uint8_t randNotes = rand() % 8;
				for(int i = 0; i < 3; ++i){
					if(randNotes & (1 << i)){
						createNote(i);
					}
				}
			}

			updateNotes(deltaTime);

			updateTracks(deltaTime);
			break;

		case DoneAnim:
			if(fail[0] || fail[1] || fail[2]){
				updateTracks(deltaTime);
			}else{
				hideBars(deltaTime);
				updateNotes(deltaTime);
			}

			break;

		case DonePause:
			hideBars(deltaTime);
			updateNotes(deltaTime);

			gameDoneTimer += deltaTime;
			if(gameDoneTimer >= gameDonePause){
				pop();
			}
			break;

		default:
			break;
	}

}

void Game5::onRender(Sprite* canvas){
}

void Game5::buttonPressed(uint i){
	if(i == BTN_BACK){
		pop();
		return;
	}

	if(state != Running) return;

	if(!btnBarMap.count(i)) return;

	uint8_t circleIndex = btnBarMap[i];

	noteHit(circleIndex);

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
			note->setPos({ note->getPos().x, note->getPos().y + delta * notesSpeed });
		}
	}
}

void Game5::createNote(uint8_t track){
	uint8_t type = rand() % 3;

	auto note = std::make_shared<GameObject>(std::make_unique<StaticRC>(getFile(notesIcons[type]), PixelDim { 8, 9 }),
											 std::make_unique<RectCC>(glm::vec2 { 8, 9 }));

	note->getRenderComponent()->setLayer(2);
	note->setPos({ barsX[track] + 3, -9 });

	collision.addPair(*note, bottomWall, [this, track](){
		removeObject(notes[track].front());
		notes[track].pop_front();
	});

	notes[track].push_back(note);
	addObject(note);
}

void Game5::noteHit(uint8_t track){
	float diff;
	if(notes[track].empty()){
		diff = noteTolerance + 1; //incorrect note hit even when no notes are present on track
	}else{
		diff = abs(notePerfectY - notes[track].front()->getPos().y);
	}


	if(diff <= noteTolerance){
		score += notePoints + (int)(diff * perfectBonus / noteTolerance);

		adjustTempo();
		adjustScoreBar();

		removeObject(notes[track].front());
		notes[track].pop_front();
		if(score >= goal){
			gameDone(true);
			return;
		}

		uint8_t i = rand() % 4;
		duckRC->setAnim(getFile(danceGIFs[i]));

		duckRC->setLoopMode(GIF::LoopMode::Infinite);
		duckRC->setLoopDoneCallback([&](uint32_t){
			duckRC->setAnim((getFile("/idle.gif")));
		});

	}else{
		life--;
		hearts->setLives(life);

		fail[track] = true;
		failTime[track] = 0;

		if(life <= 0){
			gameDone(false);
			return;
		}
	}
}

void Game5::adjustTempo(){
	float factor = 1.0f + 0.75f * ((float)(min(score, goal)) / (float)goal);
	beatInterval = defaultBeatInterval / factor;
	notesSpeed = defaultNotesSpeed * factor;
}

void Game5::gameDone(bool success){
	if(success){
		duckRC->setAnim(getFile("/win.gif"));
		std::static_pointer_cast<StaticRC>(scoreStar->getRenderComponent())->setFile(getFile("/starFull.raw"));
	}else{
		duckRC->setAnim(getFile("/fail.gif"));
	}

	state = DoneAnim;

	duckRC->setLoopMode(GIF::LoopMode::Single);
	duckRC->setLoopDoneCallback([this](uint32_t){
		state = DonePause;
		gameDoneTimer = 0;
		duckRC->stop();
	});
}

void Game5::adjustScoreBar(){
	int infill = (int)((float)(scoreBarSprite->height() - 2) * ((float)(min(score, goal)) / (float)goal));

	scoreBarSprite->fillRect(1, scoreBarSprite->height() - 1 - infill, scoreBarSprite->width() - 2, infill, C_RGB(246, 242, 65));
}

void Game5::hideBars(float deltaTime){
	for(int i = 0; i < 3; i++){
		auto pos = bars[i]->getPos();
		pos.y -= 80.0f * deltaTime;
		bars[i]->setPos(pos);

		pos = circles[i]->getPos();
		pos.y -= 80.0f * deltaTime;
		circles[i]->setPos(pos);
	}
}

void Game5::updateTracks(float delta){
	for(uint8_t i = 0; i < 3; i++){
		if(fail[i]){
			failTime[i] += delta;

			auto rc = std::static_pointer_cast<StaticRC>(bars[i]->getRenderComponent());
			if((int)(failTime[i] / failBlinkDuration) % 2 == 0){
				rc->setFile(getFile(barsIcons[3]));
			}else{
				rc->setFile(getFile(barsIcons[i]));
			}

			if(failTime[i] >= failDuration){
				failTime[i] = 0;
				fail[i] = false;
				rc->setFile(getFile(barsIcons[i]));
			}
		}
	}
}
