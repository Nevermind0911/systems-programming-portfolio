#include "point_list.h"



void  add_point(struct intrusive_list *list, int x, int y) {
  struct point *current_point = (struct point *) malloc(sizeof(struct point));
  current_point->x = x;
  current_point->y = y;
  current_point->node.next = NULL;
  current_point->node.prev = NULL;
  add_node(list, &current_point->node);
}

void remove_point(struct intrusive_list *list, int x, int y) {
  struct intrusive_node *current = list->head;
  while (current != NULL) {
    struct point *element = container_of(current, struct point, node);
    if (element->x == x && element->y == y) {
      struct intrusive_node *new_node = current->next;
      remove_node(list, &element->node);
      free(element);
      current = new_node;
    } else {
      current = current->next;
    }
  }
}

void show_all_points(struct intrusive_list *list) {
  struct intrusive_node *current = list->head;
  for (size_t i = 0; i<list->length;i++) {
    struct point *element = container_of(current, struct point, node);
    printf("(%d %d)", element->x, element->y);
    current = current->next;
    if (current != NULL) {
      printf(" ");
    }
  }
  printf("\n");
}

void remove_all_points(struct intrusive_list *list) {
  struct intrusive_node *current = list->head;
  while (current != NULL) {
    struct intrusive_node *new_node = current->next;
    struct point *element = container_of(current, struct point, node);
    remove_node(list, &element->node);
    free(element);
    current = new_node;
  }
}

