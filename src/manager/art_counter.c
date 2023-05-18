#include <stdlib.h>
#include "../error/error.h"
#include "art_counter.h"

ArtCounter *initializeArtCounter()
{
    ArtCounter *artCounter = (ArtCounter *) malloc(sizeof(ArtCounter));

    artCounter->totalTime = 0;
    artCounter->numberOfFinishedProcesses = 0;

    if (!artCounter
    )
    {
        printf(ALLOCATION_ERROR, "average response time counter");
        return NULL;
    }

    return artCounter;
}

void addFinishedProcess (ArtCounter *artCounter, int processInitTime, int currTime)
{
    artCounter->numberOfFinishedProcesses++;
    artCounter->totalTime += (currTime - processInitTime);
}

double calcAverageResponseTime(ArtCounter *artCounter)
{
    if(artCounter->numberOfFinishedProcesses == 0) return 0;
    else return artCounter->totalTime / (double) artCounter->numberOfFinishedProcesses;
}

void freeArtCounter(ArtCounter *artCounter)
{
    artCounter->totalTime = 0;
    artCounter->numberOfFinishedProcesses = 0;
}