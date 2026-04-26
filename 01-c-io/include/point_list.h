#include "clist.h"

struct point {
  int x, y;
  struct intrusive_node node;
};

void  add_point(struct intrusive_list *list, int x, int y);
void remove_point(struct intrusive_list *list, int x, int y);
void show_all_points(struct intrusive_list *list);
void remove_all_points(struct intrusive_list *list);

