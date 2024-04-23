#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = 1;
  nil->key = -2147483648;
  nil->left = nil;
  nil->right = nil;
  nil->parent = NULL;
  p->nil = nil;
  p->root = nil;
  return p;
}

node_t *recursive_cut(node_t *node) {
  if (node->left->parent == NULL && node->right->parent == NULL) return node;
  if (node->left->parent != NULL) free(recursive_cut(node->left));
  if (node->right->parent != NULL) free(recursive_cut(node->right));
  return node;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(recursive_cut(t->root));
  free(t->nil);
  free(t);
}

void left_rotate(rbtree *t, node_t *cur) {
  if (cur == cur->parent->left) cur->parent->left = cur->right;
  else if (cur->parent == t->nil) t->root = cur->right;
  else cur->parent->right = cur->right;
  cur->right->parent = cur->parent;
  cur->parent = cur->right;
  cur->right = cur->parent->left;
  if (cur->right != t->nil) cur->right->parent = cur;
  cur->parent->left = cur;
}

void right_rotate(rbtree *t, node_t *cur) {
  if (cur == cur->parent->right) cur->parent->right = cur->left;
  else if (cur->parent == t->nil) t->root = cur->left;
  else cur->parent->left = cur->left;
  cur->left->parent = cur->parent;
  cur->parent = cur->left;
  cur->left = cur->parent->right;
  if (cur->left != t->nil) cur->left->parent = cur;
  cur->parent->right = cur;
}

void rb_insert_fix(rbtree *t, node_t *cur) {
  node_t *uncle;
  while (cur->parent->color == 0)
  {
    if (cur->parent == cur->parent->parent->left) {
      uncle = cur->parent->parent->right;
      if (uncle->color == 0) {
        cur->parent->color = 1;
        uncle->color = 1;
        cur->parent->parent->color = 0;
        cur = cur->parent->parent;
      }
      else {
        if (cur == cur->parent->right) {
        cur = cur->parent;
        left_rotate(t, cur);
        }
      cur->parent->color = 1;
      cur->parent->parent->color = 0;
      right_rotate(t,cur->parent->parent);
      }
    }

    else {
      uncle = cur->parent->parent->left;
      if (uncle->color == 0) {
        cur->parent->color = 1;
        uncle->color = 1;
        cur->parent->parent->color = 0;
        cur = cur->parent->parent;
      }
      else {
        if (cur == cur->parent->left) {
        cur = cur->parent;
        right_rotate(t, cur);
        }
      cur->parent->color = 1;
      cur->parent->parent->color = 0;
      left_rotate(t,cur->parent->parent);
      }
    }
    t->root->color = 1;
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  // new_node 초기화: 색깔 red, 좌우자식 nil
  node_t *new_node = (node_t *) calloc(1, sizeof(node_t));
  new_node->color = 0;
  new_node->key = key;
  new_node->left = t->nil;
  new_node->right = t->nil;

  if (t->root == t->nil) {
    new_node->color = 1;
    new_node->parent = t->nil;
    t->root = new_node;
    return t->root;
  }

  // 삽입할 위치 찾아 들어가기
  node_t *parent = t->nil;
  node_t *cur = t->root;
  while (cur != t->nil) {
    parent = cur;
    if (new_node->key < cur->key) cur = cur->left;
    else cur = cur->right;
  }
  
  // 삽입: 찾은 부모 등록, 부모 입장에서 자식 등록
  new_node->parent = parent;
  if (parent->key > new_node->key) parent->left = new_node;
  else parent->right = new_node;

  // 문제 확인 및 수정
  rb_insert_fix(t, new_node);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}


// 트리를 출력하는 함수
void print_rbtree(rbtree *t, node_t *node, int space)
{
  if (node == t->nil)
    return;

  space += 10;
  print_rbtree(t, node->right, space);

  printf("\n");
  for (int i = 10; i < space; i++)
    printf(" ");
  printf("%d(%s)\n", node->key, node->color == RBTREE_RED ? "R" : "B");

  print_rbtree(t, node->left, space);
}

int main()
{
  rbtree *t = new_rbtree(); // 레드-블랙 트리 생성 함수
  int key;

  printf("노드를 삽입하려면 키 값을 입력하세요 (음수를 입력하면 종료):\n");
  while (scanf("%d", &key) && key >= 0)
  {
    rbtree_insert(t, key);
    print_rbtree(t, t->root, 0);
  }

  // 트리 메모리 해제
  delete_rbtree(t);

  return 0;
}