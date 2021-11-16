#pragma once

#include"Types.h"

enum Tag
{
    PLAYER,  
    ENEMY,   
    ACTOR,   
    BULLET,  
    PLATFORM,
    SCENERY,
    STAR,
    FIRE,
    ENEMYSPAWNER,
    PLAYERSPAWNER

};

//Tagset CombineTags(std::vector<Tag> tags);
//
//bool DoesLeftContainRight(Tagset left, Tagset right);
//
//Tag GetMajorTag(Tag tag);

//bool DoesLeftContainRight(Tagset left, Tag right)
//{
//    int masked = left & right;
//    return right ^ left;
//}

