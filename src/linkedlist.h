#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#ifndef LINKED_LIST_ITEM_TYPE
#define LINKED_LIST_ITEM_TYPE LinkedListItemTypePlaceholder
typedef struct LinkedListItemTypePlaceholder {
    int item;
} LinkedListItemTypePlaceholder;
#endif // LINKED_LIST_ITEM_TYPE

typedef struct ListNode {
    struct LINKED_LIST_ITEM_TYPE item;
    struct ListNode* nextItem;
} ListNode;

ListNode* LinkedList_rootNode(LINKED_LIST_ITEM_TYPE item);

ListNode* LinkedList_attachNode(ListNode* parentNode, LINKED_LIST_ITEM_TYPE item);

ListNode* _LinkedList_allocateNode();

#endif // LINKEDLIST_H_INCLUDED
