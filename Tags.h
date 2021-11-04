#pragma once

#include"Types.h"

enum Tag
{
    PLAYER,                 // 0 
    ENEMY = 1 << 0,         // 1
    ACTOR = 1 << 1,         // 2
    BULLET = 1 << 2,        // 4
    PLATFORM = 1 << 3,      // 8 
    SCENERY = 1 << 4        // 16
};

Tagset CombineTags(std::vector<Tag> tags);

bool DoesLeftContainRight(Tagset left, Tagset right);

Tag GetMajorTag(Tag tag);

//bool DoesLeftContainRight(Tagset left, Tag right)
//{
//    int masked = left & right;
//    return right ^ left;
//}

