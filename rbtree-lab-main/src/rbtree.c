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

node_t *recursive_cut(node_t *node, node_t *nil) {
  if (node->left == nil && node->right == nil) return node;
  if (node->left != nil) free(recursive_cut(node->left, nil));
  if (node->right != nil) free(recursive_cut(node->right, nil));
  return node;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil) free(recursive_cut(t->root, t->nil));
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
  // printf("\ninsert %d", key);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *cur = t->root;
  // printf("\nfinding %d", key);
  while (cur != t->nil) {
    if (cur->key > key) cur = cur->left;
    else if (cur->key < key) cur = cur->right;
    else {
      // printf("found");
      return cur;
    }
  }
  // printf("!!!");
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  node_t *pre = t->nil;

  while (cur != t->nil) {
    pre = cur;
    cur = cur->left;
  }
  if (pre == t->nil) return t->nil;
  return pre;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  node_t *pre = t->nil;

  while (cur != t->nil) {
    pre = cur;
    cur = cur->right;
  }
  if (pre == t->nil) return t->nil;
  return pre;
}

void rb_delete_fix(rbtree *t, node_t *cur) {
  node_t *sibling;
  while (cur != t->root && cur->color == 1) {
    if (cur == cur->parent->left) {
      sibling = cur->parent->right;
      //case 1
      if (sibling->color == 0) {
        sibling->color = 1;
        cur->parent->color = 0;
        left_rotate(t, cur->parent);
        sibling = cur->parent->right;
      }

      //case 2
      if (sibling->left->color == 1 && sibling->right->color == 1) {
        sibling->color = 0;
        cur = cur->parent;
      }

      //case 3
      else {
        if (sibling->right->color == 1) {
          sibling->left->color = 1;
          sibling->color = 0;
          right_rotate(t, sibling);
          sibling = cur->parent->right;
        }
        sibling->color = cur->parent->color;
        cur->parent->color = 1;
        sibling->right->color = 1;
        left_rotate(t, cur->parent);
        cur = t->root;
      }
    }

    else {
      sibling = cur->parent->left;
      //case 1
      if (sibling->color == 0) {
        sibling->color = 1;
        cur->parent->color = 0;
        right_rotate(t, cur->parent);
        sibling = cur->parent->left;
      }

      //case 2
      if (sibling->left->color == 1 && sibling->right->color == 1) {
        sibling->color = 0;
        cur = cur->parent;
      }

      //case 3
      else {
        if (sibling->left->color == 1) {
          sibling->right->color = 1;
          sibling->color = 0;
          left_rotate(t, sibling);
          sibling = cur->parent->left;
        }
        sibling->color = cur->parent->color;
        cur->parent->color = 1;
        sibling->left->color = 1;
        right_rotate(t, cur->parent);
        cur = t->root;
      }
    }
  }
  cur->color = 1;
}

void rb_transplant(rbtree *t, node_t *par, node_t *chi) {
  if (par->parent == t->nil) t->root = chi;
  else if (par == par->parent->left) par->parent->left = chi;
  else par->parent->right = chi;
  chi->parent = par->parent;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *successor = p;
  node_t *chi;
  color_t suc_o_color = successor->color;
  if (p->left == t->nil) {
    chi = p->right;
    rb_transplant(t, p, p->right);
  }
  else if (p->right == t->nil) {
    chi = p->left;
    rb_transplant(t, p, p->left);
  }
  else {
    rbtree *subtree = (rbtree *)calloc(1, sizeof(rbtree));
    subtree->nil = t->nil;
    subtree->root = p->right;
    successor = rbtree_min(subtree);
    suc_o_color = successor->color;
    chi = successor->right;
    if (successor->parent == p) chi->parent = successor;
    else {
      rb_transplant(t, successor, successor->right);
      successor->right = p->right;
      successor->right->parent = successor;
    }
    rb_transplant(t, p, successor);
    successor->left = p->left;
    successor->left->parent = successor;
    successor->color = p->color;
    free(subtree);
  }
  if (suc_o_color == 1) rb_delete_fix(t, chi);
  free(p);
  return 0;
}

void traversal(node_t *node, node_t *nil, key_t *arr, int *index, int n) {
  if (node->left != nil) traversal(node->left, nil, arr, index, n);
  if (*index < n) {
    arr[*index] = node->key;
    *index +=1;
  }
  if (node->right != nil) traversal(node->right, nil, arr, index, n);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int *index = (int *) malloc(sizeof(int));
  *index = 0;
  if (t->root != t->nil) traversal(t->root, t->nil, arr, index, n);
  free(index);
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

// int main() {
//     // 새로운 레드-블랙 트리 생성
//     rbtree *t = new_rbtree();

//     // 삽입할 노드 키 배열
//     int keys[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};

//     // 트리에 노드 삽입
//     for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
//         rbtree_insert(t, keys[i]);
//     }

//     // 트리 출력
//     printf("Inserted Nodes:\n");
//     print_rbtree(t, t->root, 0);

//     // 삭제할 노드 키 배열
//     int delete_keys[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};

//     // 트리에서 노드 삭제
//     for (int i = 0; i < sizeof(delete_keys) / sizeof(delete_keys[0]); i++) {
//         node_t *node = rbtree_find(t, delete_keys[i]);
//         if (node != NULL) {
//             rbtree_erase(t, node);
//             printf("\n\nAfter deleting node with key %d:\n", delete_keys[i]);
//             print_rbtree(t, t->root, 0);
//         } else {
//             printf("\nNode with key %d not found!\n", delete_keys[i]);
//         }
//     }

//     // 트리 메모리 해제
//     delete_rbtree(t);

//     return 0;
// }