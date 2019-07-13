//
// Created by maria on 12/07/19.
//

#include <stddef.h>
#include <stdlib.h>
#include "dynamicArray.h"

ListNode* addIntNode(ListNode *list, int line){
    ListNode *newnode;

    newnode = malloc(sizeof(ListNode));
    newnode->value.ival=line;
    newnode->next = list;

    return newnode;
}

ListNode* addCharNode(ListNode *list, char line){
    ListNode *newnode;

    newnode = malloc(sizeof(ListNode));
    newnode->value.cval=line;
    newnode->next = list;

    return newnode;
}

ListNode* addModEntryNode(ListNode *list, int line, int oid){

    ListNode *newnode;

    newnode = malloc(sizeof(ListNode));
    newnode->value.ival=line;
    newnode->modOid=oid;
    newnode->next = list;

    return newnode;

}