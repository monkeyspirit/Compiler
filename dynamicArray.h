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
    int modOid;
    struct node *next; //punta al nodo successivo
} ListNode;

ListNode* addIntNode(ListNode *list, int line);
ListNode* addCharNode(ListNode *list, char line);
ListNode* addModEntryNode(ListNode *list, int line, int modOid);





#endif //COMPILER_DYNAMICARRAY_H
