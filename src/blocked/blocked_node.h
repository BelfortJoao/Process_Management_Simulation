#ifndef SRC_BLOCKED_NODE_H
#define SRC_BLOCKED_NODE_H

#include <stdbool.h>
#include "../manager/timer.h"


typedef struct BlockedNode
{
    int id;
    Timer blockTime;
    struct BlockedNode *next;
} BlockedNode;


BlockedNode *initializeBlockedNode(int id, Timer timer);


#endif //SRC_BLOCKED_NODE_H