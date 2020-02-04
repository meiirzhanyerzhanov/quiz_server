#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "linkedlist02.h"

int main() {
    List *l = List_create();

    List_append(l, "jim");
    List_append(l, "bob");
    List_append(l, "joe");
    List_append(l, "bean");
    List_append(l, "junior");

    List_insert(l, 3, "HAROLD");
    List_insert(l, List_length(l), "CARL");
    List_insert(l, 0, "MIKE");

    List_remove(l, 2);

    List_print(l);
    printf("Length: %i\n", List_length(l));
    printf("Pop 3: %s\n", List_pop(l, 3));
    printf("Get 3: %s\n", List_get(l, 3));
    printf("Find \"junior\": %i\n", List_find(l, "junior"));

    List_destroy(l);

    return 0;
}


Node *Node_create() {
    Node *node = malloc(sizeof(Node));
    assert(node != NULL);

    node->data = "";
    node->next = NULL;

    return node;
}


void Node_destroy(Node *node) {
    assert(node != NULL);
    free(node);
}


List *List_create() {
    List *list = malloc(sizeof(List));
    assert(list != NULL);

    Node *node = Node_create();
    list->first = node;

    return list;
}


void List_destroy(List *list) {
    assert(list != NULL);

    Node *node = list->first;
    Node *next;
    while (node != NULL) {
        next = node->next;
        free(node);
        node = next;
    }

    free(list);
}


void List_append(List *list, char *str) {
    assert(list != NULL);
    assert(str != NULL);

    Node *node = list->first;
    while (node->next != NULL) {
        node = node->next;
    }

    node->data = str;
    node->next = Node_create();
}


void List_insert(List *list, int index, char *str) {
    assert(list != NULL);
    assert(str !=NULL);
    assert(0 <= index);
    assert(index <= List_length(list));

    if (index == 0) {
        Node *after = list->first;
        list->first = Node_create();
        list->first->data = str;
        list->first->next = after;
    } else if (index == List_length(list)) {
        List_append(list, str);
    } else {
        Node *before = list->first;
        Node *after = list->first->next;
        while (index > 1) {
            index--;
            before = before->next;
            after = after->next;
        }
        before->next = Node_create();
        before->next->data = str;
        before->next->next = after;
    }
}


char *List_get(List *list, int index) {
    assert(list != NULL);
    assert(0 <= index);
    assert(index < List_length(list));

    Node *node = list->first;
    while (index > 0) {
        node = node->next;
        index--;
    }
    return node->data;
}


int List_find(List *list, char *str) {
    assert(list != NULL);
    assert(str != NULL);

    int index = 0;
    Node *node = list->first;
    while (node->next != NULL) {
        if (strlen(str) == strlen(node->data)) {
            int cmp = strcmp(str, node->data);
            if (cmp == 0) {
                return index;
            }
        }
        node = node->next;
        index++;
    }
    return -1;
}


void List_remove(List *list, int index) {
    assert(list != NULL);
    assert(0 <= index);
    assert(index < List_length(list));

    if (index == 0) {
        Node *node = list->first;
        list->first = list->first->next;
        Node_destroy(node);
    } else {
        Node *before = list->first;
        while (index > 1) {
            before = before->next;
            index--;
        }
        Node *node = before->next;
        before->next = before->next->next;
        Node_destroy(node);
    }
}


char *List_pop(List *list, int index) {
    assert(list != NULL);
    assert(0 <= index);
    assert(index < List_length(list));

    if (index == 0) {
        Node *node = list->first;
        list->first = list->first->next;
        char *data = node->data;
        Node_destroy(node);
        return data;
    } else {
        Node *before = list->first;
        while (index > 1) {
            before = before->next;
            index--;
        }
        Node *node = before->next;
        before->next = before->next->next;
        char *data = node->data;
        Node_destroy(node);
        return data;
    }
}


int List_length(List *list) {
    assert(list != NULL);

    Node *node = list->first;
    int length = 0;
    while (node->next != NULL) {
        length++;
        node = node->next;
    }

    return length;
}


void List_print(List *list) {
    assert(list != NULL);

    printf("[");
    Node *node = list->first;
    while (node->next != NULL) {
        printf("%s", node->data);
        node = node->next;
        if (node->next != NULL) {
            printf(", ");
        }
    }
    printf("]\n");
}
