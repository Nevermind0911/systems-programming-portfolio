#include "point_list.h"

void count( __attribute__((unused)) struct intrusive_node *node, void *data) {
  (*(int *)data)++;
}

void print(struct intrusive_node *node, void *data) {
  struct point *element = container_of(node, struct point, node);
  printf((char *) data, element->x, element->y);
}

void save_text(struct intrusive_node *node, void *data) {
  FILE *file = (FILE *) data;
  struct point *element = container_of(node, struct point, node);
  fprintf(file, "%d %d\n", element->x, element->y);
}

void save_bin(struct intrusive_node *node, void *data) {
  FILE *file = (FILE *) data;
  struct point *element = container_of(node, struct point, node);
  fwrite(&element->x, sizeof(int), 1, file);
  fwrite(&element->y, sizeof(int), 1, file);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("\n");
  }
  struct intrusive_list list;
  init_list(&list);
  int x,y;
  if (strcmp(argv[1], "loadtext") == 0) {
    FILE *ftxtin = fopen(argv[2], "r");
    if (ftxtin != NULL) {
      while(fscanf(ftxtin, "%d %d", &x, &y) == 2) {
        add_point(&list, x, y);
      }
    }
    fclose(ftxtin);
  } else {
    FILE *fbinin = fopen(argv[2], "rb");
      if (fbinin != NULL) {
          unsigned char buff[6];
          while (fread(buff, sizeof(unsigned char), 6, fbinin) == 6) {
              int x, y;
              x = (buff[0] | (buff[1] << 8) | (buff[2] << 16));
              y = (buff[3] | (buff[4] << 8) | (buff[5] << 16));
              add_point(&list, x, y);
          }
      }
      fclose(fbinin);
    }
  if (strcmp(argv[3], "count") == 0) {
    int cnt = 0;
    apply(&list, count, &cnt);
    printf("%d\n", cnt);
  } else if (strcmp(argv[3], "print") == 0) {
    apply(&list, print, argv[4]);
    printf("\n");
  } else if (strcmp(argv[3], "savetext") == 0) {
    FILE *ftxtout = fopen(argv[4], "w");
    apply(&list, save_text, ftxtout);
    fclose(ftxtout);
  } else {
    FILE *fbinout = fopen(argv[4], "wb");
    apply(&list, save_bin, fbinout);
    fclose(fbinout);
  }
  remove_all_points(&list);
}
