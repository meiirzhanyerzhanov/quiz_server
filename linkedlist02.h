#ifndef LINKED_LIST_02
#define LINKED_LIST_02



typedef struct NodeTag {
    char *data;
    struct NodeTag *next;
} Node;

Node *Node_create();
void Node_destroy(Node *node);

typedef struct ListTag {
    struct NodeTag *first;
} List;

List *List_create();
void List_destroy(List *list);

void List_append(List *list, char *str);
void List_insert(List *list, int index, char *str);

char *List_get(List *list, int index);
int List_find(List *list, char *str);

void List_remove(List *list, int index);
char *List_pop(List *list, int index);

int List_length(List *list);
void List_print(List *list);

#endif
