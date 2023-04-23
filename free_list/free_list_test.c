#include "free_list.h"
#include <assert.h>
#include <stdio.h>

void is_empty_test1();
void is_empty_test2();
void is_empty_test3();

void insert_first_test1();
void insert_first_test2();

void remove_test1();
void remove_test2();
void remove_test3();
void remove_test4();
void remove_test5();

int main() {
  is_empty_test1();
  is_empty_test2();
  is_empty_test3();

  printf("=================\n");

  insert_first_test1();
  insert_first_test2();

  printf("=================\n");

  remove_test1();
  remove_test2();
  remove_test3();
  remove_test4();
  remove_test5();

  return 0;
}

void is_empty_test1() {
  printf("is_empty_test1\n");

  fl_list_t list;
  fl_init(&list);

  assert(fl_is_empty(&list) == true);
}

void is_empty_test2() {
  printf("is_empty_test2\n");

  fl_list_t list;
  fl_mcb_t node;
  list.head = &node;

  assert(fl_is_empty(&list) == false);
}

void is_empty_test3() {
  printf("is_empty_test3\n");

  assert(fl_is_empty(NULL) == true);
}

void insert_first_test1() {
  printf("inset_first_test1\n");

  fl_list_t list;
  fl_init(&list);

  fl_mcb_t node;
  fl_insert_first(&list, &node);

  assert(list.head == &node);
  assert(list.last == &node);
}

void insert_first_test2() {
  printf("inset_first_test2\n");

  fl_list_t list;
  fl_init(&list);

  fl_mcb_t node1;
  fl_insert_first(&list, &node1);

  fl_mcb_t node2;
  fl_insert_first(&list, &node2);

  assert(list.head == &node2);
  assert(list.last == &node1);

  assert(node2.next == &node1);
  assert(node2.prev == NULL);

  assert(node1.prev == &node2);
}

void remove_test1() {
  printf("remove_test1\n");

  fl_list_t list;
  fl_init(&list);

  fl_mcb_t node1;
  fl_remove(&list, &node1);

  assert(list.head == NULL);
  assert(list.last == NULL);
}

void remove_test2() {
  printf("remove_test2\n");

  fl_list_t list;
  fl_init(&list);

  fl_mcb_t node;
  fl_insert_first(&list, &node);

  fl_remove(&list, &node);

  assert(list.head == NULL);
  assert(list.last == NULL);

  assert(node.next == NULL);
  assert(node.prev == NULL);
}

void remove_test3() {
  printf("remove_test3\n");

  fl_list_t list;
  fl_init(&list);

  fl_mcb_t node1;
  fl_mcb_t node2;

  fl_insert_first(&list, &node1);
  fl_insert_first(&list, &node2);

  fl_remove(&list, &node1);

  assert(list.head == &node2);
  assert(list.last == &node2);

  assert(node2.next == NULL);
  assert(node2.prev == NULL);

  assert(node1.next == NULL);
  assert(node1.prev == NULL);
}

void remove_test4() {
  printf("remove_test4\n");

  fl_list_t list;
  fl_init(&list);

  fl_mcb_t node1;
  fl_mcb_t node2;

  fl_insert_first(&list, &node1);
  fl_insert_first(&list, &node2);

  fl_remove(&list, &node2);

  assert(list.head == &node1);
  assert(list.last == &node1);

  assert(node1.next == NULL);
  assert(node1.prev == NULL);

  assert(node2.next == NULL);
  assert(node2.prev == NULL);
}

void remove_test5() {
  printf("remove_test5\n");

  fl_list_t list;
  fl_init(&list);

  fl_mcb_t node1;
  fl_mcb_t node2;
  fl_mcb_t node3;

  fl_insert_first(&list, &node1);
  fl_insert_first(&list, &node2);
  fl_insert_first(&list, &node3);

  fl_remove(&list, &node2);

  assert(list.head == &node3);
  assert(list.last == &node1);

  assert(node3.next == &node1);
  assert(node3.prev == NULL);

  assert(node1.next == NULL);
  assert(node1.prev == &node3);

  assert(node2.next == NULL);
  assert(node2.prev == NULL);
}