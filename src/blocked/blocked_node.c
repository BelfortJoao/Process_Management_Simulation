#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../error/error.h"

#include "blocked_node.h"


BlockedNode *initializeBlockedNode(int id, Timer timer)
{
    BlockedNode *blockedNode = (BlockedNode *) malloc(sizeof(BlockedNode));

    if (!blockedNode)
    {
        printf(ALLOCATION_ERROR, "blocked node");
        return NULL;
    }

    blockedNode->id = id;
    blockedNode->blockTime = timer;
    blockedNode->next = NULL;

    return blockedNode;
}