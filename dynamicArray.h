//
// Created by maria on 12/07/19.
//

#ifndef COMPILER_DYNAMICARRAY_H
#define COMPILER_DYNAMICARRAY_H

#include "def.h"

typedef union {
    int ival;
    char cval;
} ArrayVal;

typedef struct node{
    ArrayVal value; //contiene il valore
    struct node *next; //punta al nodo successivo
} ListNode;

ListNode* addIntNode(ListNode *list, int line);
ListNode* addCharNode(ListNode *list, char line);





#endif //COMPILER_DYNAMICARRAY_H
