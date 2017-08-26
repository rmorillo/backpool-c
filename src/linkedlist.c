#include "linkedlist.h"
#include <stdlib.h>

ListNode*  LinkedList_attachNode(ListNode* parentNode, LINKED_LIST_ITEM_TYPE item)
{
    ListNode* newNode = _LinkedList_allocateNode();

    newNode->item = item;
    newNode->nextItem = NULL;
    parentNode->nextItem= newNode;

    return newNode;
}

ListNode* LinkedList_rootNode(LINKED_LIST_ITEM_TYPE item)
{
    ListNode* newNode = _LinkedList_allocateNode();

    newNode->item = item;
    newNode->nextItem = NULL;

    return newNode;
}

ListNode* _LinkedList_allocateNode()
{
    return malloc(sizeof(LINKED_LIST_ITEM_TYPE));
}
