#pragma once
#include "cocos2d.h"
#include "GameDefine.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
#define BLOCK_PAN_STATUS_INIT 0
#define BLOCK_PAN_STATUS_RUN BLOCK_PAN_STATUS_INIT+1
#define BLOCK_PAN_STATUS_SHOW_END BLOCK_PAN_STATUS_RUN+1
#define BLOCK_PAN_STATUS_SHOW_END_OVER BLOCK_PAN_STATUS_SHOW_END+1

USING_NS_CC;
class GameData
{
private:
	int mScore;
	int highScore;
	int comboUp4Num;
public:
	GameData(void);
	~GameData(void);
	static GameData* shareData();
	void init();
	int getScore();
	void setScore(int score);
	void addScore(int score);

	int getCombo();
	void setCombo(int combo);
	void addCombo(int combo);

	const char* getScoreString();
	int getHighScore();
	const char* getHighScoreString();
	void setHighScore(int score);
};

