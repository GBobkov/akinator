#ifndef AKINATOR_H
#define AKINATOR_H

#include "tree.h"

enum MODE_VARIATIONS
{
    MODE_EXIT       =0,
    MODE_GAME       =1,
    MODE_SEARCH    =2,
    MODE_DIFFERENCE =4
};

int Play_Akinator(void);

#endif