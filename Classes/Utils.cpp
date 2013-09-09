#include "Utils.h"

CCRect Utils::getRect(CCNode* pNode) 
{ 
	CCRect rc; 
	rc.origin = pNode->getPosition(); 
	rc.size = pNode->getContentSize(); 
	rc.origin.x -= rc.size.width*0.5; 
	rc.origin.y -= rc.size.height*0.5; 
	return rc; 
} 
int Utils::randomBetweenInt(int max, int min)   
{   
	time_t t;   
	srand((unsigned int)time(&t));//随机数种子   
	return rand() % (max - min) + min;   
}  
float Utils::randomBetweenFloat(float min, float max)    
{   
	return (2.0f*CCRANDOM_0_1() / 0xFFFFFFFFu) * (max - min) + min;   
}  
