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

cocos2d::CCRect Utils::getRect( CCNode* pNode,CCNode* pBase )
{
	CCPoint winPoint = pBase->convertToNodeSpace(pNode->getPosition());
	CCPoint winAnchor = pNode->getAnchorPoint();
	winPoint.x = winPoint.x-pNode->getContentSize().width*winAnchor.x;
	winPoint.y = winPoint.y-pNode->getContentSize().height*winAnchor.y;
	return CCRectMake(winPoint.x,winPoint.y, pNode->getContentSize().width, pNode->getContentSize().height);
}

int Utils::randomBetweenInt(int max, int min)   
{   
	time_t t;   
	srand((unsigned int)time(&t));//���������   
	return rand() % (max - min) + min;   
}  
float Utils::randomBetweenFloat(float min, float max)    
{   
	return (2.0f*CCRANDOM_0_1() / 0xFFFFFFFFu) * (max - min) + min;   
}  
