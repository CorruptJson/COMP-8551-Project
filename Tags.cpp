#include"Tags.h"

Tagset CombineTags(std::vector<Tag> tags)
{
    int combined = 0;
    for (int i = 0; i < tags.size(); i++)
    {
        combined |= tags[i];
    }
    return combined;
}

bool DoesLeftContainRight(Tagset left, Tagset right)
{
    int masked = left & right;
    return (masked ^ right) == 0;
}

Tag GetCollisionTag(Tag tag)
{
    static Tagset collisionTagMask = CombineTags({Tag::ENEMY,Tag::PLAYER,Tag::PLATFORM,Tag::BULLET});
    return (Tag)(tag & collisionTagMask);
}
