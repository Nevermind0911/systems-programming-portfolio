#include "clist.h"


void init_list(struct intrusive_list *list) {
  list->head = NULL;
  list->length = 0;
}

void add_node(struct intrusive_list *list, struct intrusive_node *node) {
  if (list->head == NULL) {
    list->head = node;
    node->prev = NULL;
    node->next = NULL;
    list->length++;
    return;
  }
  struct intrusive_node *next_element = list->head;
  
  while (next_element->next != NULL) {
    next_element = next_element->next;
  }
  next_element->next = node;
  node->next = NULL;
  node->prev = next_element;
  list->length++;
}

void remove_node(struct intrusive_list *list, struct intrusive_node *node) {
  if (node == list->head) {
    list->head = node->next;
    if (list->head != NULL) {
      list->head->prev = NULL;
    }
    list->length--;
    return;
  }
  node->prev->next = node->next;
  if (node->next != NULL) {
    node->next->prev = node->prev;
  }
  list->length--;
}

int get_length(struct intrusive_list *list) {
  return list->length;
}

void apply(struct intrusive_list *list, void (*op)(struct intrusive_node *node, void *data), void *data) {
  for (struct intrusive_node *node = list->head; node != NULL; node = node->next) {
    op(node, data);
  }
}

