#include "cocos2d.h"
USING_NS_CC;
#define RESOURCE_PATH_GAME(__FILE__) CCString::createWithFormat("%s%s","/gamescene/",__FILE__)->getCString()
#define RESOURCE_PATH_CRYSTRAL(__FILE__) CCString::createWithFormat("%s%s","/crystrals/",__FILE__)->getCString()
#define RESOURCE_PATH_MENU(__FILE__) CCString::createWithFormat("%s%s","/menuscene/",__FILE__)->getCString()
#define RESOURCE_PATH_PARTICLES(__FILE__) CCString::createWithFormat("%s%s","/particles/",__FILE__)->getCString()
#define RESOURCE_PATH_FONT(__FILE__) CCString::createWithFormat("%s%s","/fonts/",__FILE__)->getCString()

#define DESIGN_SCREEN_SIZE_W 640
#define DESIGN_SCREEN_SIZE_H 960

#define BLOCK_PAN_SIZE_W 6
#define BLOCK_PAN_SIZE_H 7

#define BLOCK_COMBO_NUM 4
#define BOMB_COMBO_NUM 3