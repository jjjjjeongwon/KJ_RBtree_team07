#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil=(node_t *)calloc(1, sizeof(node_t));

  p->root = nil;
  p->nil = nil;
  nil->color = RBTREE_BLACK;
  
  return p;
}

// 왼쪽 회전하는 함수
// CLRS 13장 참고
void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left;

  if (y->left != t->nil){
    y->left->parent = x;
  }

  y->parent = x->parent;
  if (x->parent == t->nil){
    t->root = y;
  }else if(x == x->parent->left){
    x->parent->left = y;
  }else{
    x->parent->right = y;
  }

  y->left =x;
  x->parent =y;
}

// 오른쪽 회전하는 함수
// CLRS 13장 참고
void right_rotate(rbtree *t, node_t *y){
  node_t *x= y->left;
  y->left = x->right;

  if(x->right != t->nil){
    x->right->parent =y;
  }

  x->parent = y->parent;
  if(y->parent == t->nil){
    t->root = x;
  }else if(y == y->parent->left){
    y->parent->left = x;
  }else{
    y->parent->right = x;
  }

  x->right = y;
  y->parent = x;
}

// 노드 삽입 후 균형 조정
node_t *rbtree_insert_fixup(rbtree *t, node_t *z){
  while(z->parent->color == RBTREE_RED){
    if(z->parent == z->parent->parent->right){
      node_t *y = z->parent->parent->left;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }else{
        if(z == z->parent->left){
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }else{
      node_t *y = z->parent->parent->right;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }else{
        if(z == z->parent->right){
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
  return t->root;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
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
