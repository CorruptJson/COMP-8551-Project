#pragma once

// the DeleteTimer component has a timestamp monitored by a sytem that will deleted it when that timestamp frame is reached
// this allows for us to have entities with specified lifetime

struct DeleteTimer
{
    int deletionFrame;
};
