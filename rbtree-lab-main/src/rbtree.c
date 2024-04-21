#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = 1;
  nil->key = -2147483648;
  nil->left = NULL;
  nil->right = NULL;
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

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
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