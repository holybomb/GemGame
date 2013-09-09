#include "GameData.h"
#include "cocos2d.h"
USING_NS_CC;
static GameData* data = NULL;

GameData::GameData(void)
{
}
void GameData::init()
{
	mScore = 0;
	highScore = 0;
}
GameData* GameData::shareData()
{
	if ( !data )
	{
		data = new GameData();
		data->init();
	}

	return data;
}


GameData::~GameData(void)
{
	delete data;
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
