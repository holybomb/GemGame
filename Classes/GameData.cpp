#include "GameData.h"
#include "cocos2d.h"
USING_NS_CC;
static GameData* gameData = NULL;

GameData::GameData(void)
{
}
void GameData::init()
{
	mScore = 0;
	highScore = 0;
	comboUp4Num = 0;
}
GameData* GameData::shareData()
{
	if ( !gameData )
	{
		gameData = new GameData();
		gameData->init();
	}

	return gameData;
}


GameData::~GameData(void)
{
	delete gameData;
}

int GameData::getScore()
{
	return mScore;
}

void GameData::setScore( int score )
{
	mScore = score;
	setHighScore(score);
}

void GameData::addScore( int score )
{
	mScore+=score;
	setScore(mScore);
}

const char* GameData::getScoreString()
{
	return (CCString::createWithFormat("%d",mScore)->getCString());
}

int GameData::getHighScore()
{
	return highScore;
}

const char* GameData::getHighScoreString()
{
	return (CCString::createWithFormat("%d",highScore)->getCString());
}

void GameData::setHighScore( int score )
{
	if (highScore<score)
	{
		highScore = score;
	}
}

int GameData::getCombo()
{
	return comboUp4Num;
}

void GameData::setCombo( int combo )
{
	comboUp4Num = combo;
}

void GameData::addCombo( int combo )
{
	setCombo(getCombo()+combo);
}
