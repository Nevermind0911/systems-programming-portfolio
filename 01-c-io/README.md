# Ввод-вывод на Си — база данных точек с интрузивным списком

Консольное приложение на C17, которое загружает список 2D-точек с диска, затем сохраняет или выводит их. Демонстрирует ручное управление памятью, обобщённый интрузивный связный список и работу с текстовыми и бинарными файлами через стандартную библиотеку Си.

## Сборка

```
make
```

Собирает бинарник `lab-05_c-io`. Требуется GCC; компилируется с флагами `-Wall -Werror -Wextra -pedantic`.

```
make clean   # удалить артефакты сборки
```

## Использование

```
./lab-05_c-io (loadtext | loadbin) <infile> <действие...>
```

| Параметр | Описание |
|---|---|
| `loadtext` / `loadbin` | Читать входной файл как текстовый или бинарный |
| `<infile>` | Путь к входному файлу |
| `savetext <outfile>` | Сохранить точки в текстовый файл |
| `savebin <outfile>` | Сохранить точки в бинарный файл |
| `print <fmt>` | Вывести каждую точку через `printf`-строку формата (два спецификатора `%d`) |
| `count` | Вывести количество точек |

### Примеры

```sh
$ cat points.txt
1 2
4 3
5 5

$ ./lab-05_c-io loadtext points.txt count
3

$ ./lab-05_c-io loadtext points.txt print "[%d, %d] "
[1, 2] [4, 3] [5, 5]

$ ./lab-05_c-io loadtext points.txt savebin out.bin
$ ./lab-05_c-io loadbin out.bin print "%d %d "
1 2 4 3 5 5
```

## Форматы файлов

**Текстовый** — по одной точке на строку, координаты `x` и `y` разделены одним пробелом:
```
1 2
4 3
```

**Бинарный** — 6 байт на точку; каждая координата занимает 3 байта в формате little-endian:
```
hexdump -C out.bin
00000000  01 00 00 02 00 00 ...
```

## Архитектура

```
include/
  clist.h        — обобщённый интрузивный двусвязный список + apply()
  point_list.h   — структура point со встроенным intrusive_node
src/
  clist.c        — операции над списком: init, add, remove, get_length, apply
  point_list.c   — функции для точек: add_point, remove_point, remove_all_points
  main.c         — разбор аргументов CLI, файловый ввод-вывод, колбэки для print/count/save
```

### Интрузивный список

Список не хранит никаких данных самостоятельно. Каждая структура `point` встраивает `intrusive_node` напрямую:

```c
struct point {
    int x, y;
    struct intrusive_node node;
};
```

Чтобы получить указатель на родительскую структуру из указателя на узел, используется макрос `container_of`, вычисляющий смещение поля:

```c
#define container_of(ptr, type, member) \
    (type*)((char*)(ptr) - offsetof(type, member))
```

Это исключает дополнительные аллокации для самого списка и работает для структур любого типа.

### `apply` — обобщённый обход списка

```c
void apply(struct intrusive_list *list,
           void (*op)(struct intrusive_node *node, void *data),
           void *data);
```

`apply` проходит по списку и вызывает `op` для каждого узла, передавая `data` как непрозрачный указатель контекста. Все четыре операции (`print`, `count`, `savetext`, `savebin`) реализованы как колбэки, передаваемые в `apply` — прямого обхода списка в `main.c` нет.

## Структура проекта

```
01-c-io/
├── include/
│   ├── clist.h
│   └── point_list.h
├── src/
│   ├── clist.c
│   ├── main.c
│   └── point_list.c
├── Makefile
└── README.md
```
