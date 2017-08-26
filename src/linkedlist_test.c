void TestLinkedList_basic()
{
    LINKED_LIST_ITEM_TYPE root;
    root.item= 1;

    ListNode* rootNode = LinkedList_rootNode(root);

    printf("%d\n",rootNode->item);

    LINKED_LIST_ITEM_TYPE child;
    child.item= 2;

    ListNode* childNode = LinkedList_attachNode(rootNode, child);

    printf("%d\n",rootNode->nextItem->item);

    LINKED_LIST_ITEM_TYPE grandChildNode;
    grandChildNode.item= 3;

    LinkedList_attachNode(childNode, grandChildNode);

    printf("%d\n",rootNode->nextItem->nextItem->item);
}
