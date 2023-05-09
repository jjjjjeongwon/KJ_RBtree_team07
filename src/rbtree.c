#include "rbtree.h"
#include <stdlib.h>

// 새로운 rbtree 생성
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->root = p->nil;
  p->nil->color = RBTREE_BLACK;

  return p;
}

// 후위 순회하면서 노드 삭제
void del_postorder(node_t *root, node_t *nil){
  if(root == NULL || root == nil){
    return;
  }
  del_postorder(root->left, nil);
  del_postorder(root->right, nil);
  free(root);
}

void delete_rbtree(rbtree *t) {
  del_postorder(t->root, t->nil);
  free(t->nil);
  free(t);
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
void rbtree_insert_fixup(rbtree *t, node_t *z){
  // red가 연속으로 나오지 않을 때 까지 실행.
  while(z->parent->color == RBTREE_RED){
    if(z->parent == z->parent->parent->right){ // 삽입 노드의 부모가 삽입 노드의 조상의 오른쪽 자식이면
      node_t *y = z->parent->parent->left; // 삽입 노드의 조상의 왼쪽 자식으로 삼촌 노드 지정
      if(y->color == RBTREE_RED){ // 부모의 색과 삼촌 노드의 색이 red면
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }else{ // 삼촌 노드의 색이 black이고
        if(z == z->parent->left){ // 삽입 노드의 부모의 왼쪽 자식이 삽입노드이면
          z = z->parent;
          right_rotate(t, z); // 부모 노드를 기준으로 오른쪽 회전
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }else{ // 삽입 노드의 부모가 삽입 노드의 조상의 왼쪽 자식이면
      node_t *y = z->parent->parent->right; // 삽입 노드의 조상의 오른쪽 자식을 삼촌 노드로 지정
      if(y->color == RBTREE_RED){ // 부모의 색과 삼촌 노드의 색이 red이면
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }else{ // 삼촌 노드의 색상이 black이고
        if(z == z->parent->right){ // 삽입 노드의 부모의 오른쪽 자식이 삽입 노드이면
          z = z->parent;
          left_rotate(t, z); // 부모를 기준으로 왼쪽으로 회전
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}


// 노드 삽입 함수
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *z = (node_t *)calloc(1,sizeof(node_t)); // 새로 삽입된 노드에게 메모리 할당
  z->key = key;

  node_t *y = t->nil; 
  node_t *x = t->root;

  while(x != t->nil){ //root노드가 nil이 아니면
    y=x;
    if(z->key < x->key){
      x = x->left;
    }else{
      x = x->right;
    }
  }

  z->parent = y; // 새로 삽입된 노드의 부모를 y로 할당
  if(y == t->nil){ // y가 nil일 경우 새로 삽입된 노드를 root로
    t->root = z;
  }else if(z->key < y->key){
    y->left = z;
  }else{
    y->right = z;
  }

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  rbtree_insert_fixup(t, z);
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *x = t->root;

  while(x != t->nil){
    if(x->key > key){
      x = x->left;
    }else if(x->key < key){
      x = x->right;
    }else{
      break;
    }
  }

  if(x == t->nil){
    return NULL;
  }else{
    return x;
  }
}

node_t* delete_min(node_t *n, node_t *nil){
  node_t *min = n;
  while(n != nil){
    min = n;
    n = n->left;
  }

  return min;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *min;
  min = delete_min(t->root, t->nil);
  return min;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;
  node_t *max = t->root;

  while(x != t->nil){
    max = x;
    x = x->right;
  }
  return max;
}

void rbtree_erase_fixup(rbtree *t, node_t *x){
  while(x != t->root && x->color == RBTREE_BLACK){
    if(x == x->parent->left){
      node_t *bro = x->parent->right;
      if(bro->color == RBTREE_RED){
        bro->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        bro = x->parent->right;
      }
      if(bro->left->color == RBTREE_BLACK && bro->right->color == RBTREE_BLACK){
        bro->color = RBTREE_RED;
        x = x->parent;
      }else{
        if(bro->right->color == RBTREE_BLACK){
          bro->left->color = RBTREE_BLACK;
          bro->color = RBTREE_RED;
          right_rotate(t, bro);
          bro = x->parent->right;
        }
        bro->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        bro->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }else{
      node_t *bro = x->parent->left;
      if(bro->color == RBTREE_RED){
        bro->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        bro = x->parent->left;
      }
      if(bro->left->color == RBTREE_BLACK && bro->right->color == RBTREE_BLACK){
        bro->color = RBTREE_RED;
        x = x->parent;
      }else{
        if(bro->left->color == RBTREE_BLACK){
          bro->right->color = RBTREE_BLACK;
          bro->color = RBTREE_RED;
          left_rotate(t, bro);
          bro = x->parent->left;
        }
        bro->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        bro->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil){
    t->root = v;
  }else if(u == u->parent->left){
    u->parent->left = v;
  }else{
    u->parent->right = v;
  }
  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *current = p;
  color_t current_color = current->color;
  node_t *x;

  if(p->left == t->nil){ // 삭제할 노드의 왼쪽자식이 nil일때
    x = p->right; // 삭제할 노드와 그 오른쪽 자식의 부모들을 바꿈
    rb_transplant(t, p, p->right);
  }else if(p->right == t->nil){ // 삭제할 노드의 오른쪽자식이 nil일때
    x = p->left; // 삭제할 노드와 그 왼쪽 자식의 부모들을 바꿈.
    rb_transplant(t, p, p->left);
  }else{ // 삭제할 노드가 자식을 2개 가지고 있다면
    current = delete_min(p->right, t->nil); // successor찾기
    current_color = current->color; // successor의 색 저장
    x = current->right;
    if(current->parent == p){ // 삭제할 노드가 successor의 부모와 같다면
      x->parent = current; // current를 current의 오른쪽 부모로 할당.
    }else{
      rb_transplant(t, current, current->right);
      current->right = p->right;
      current->right->parent = current;
    }
    rb_transplant(t, p, current);
    current->left = p->left;
    current->left->parent = current;
    current->color = p->color;
  }
  free(p);
  p = NULL;

  if(current_color == RBTREE_BLACK){
    rbtree_erase_fixup(t, x);
  }
  t->nil->parent = NULL;
  t->nil->left = NULL;
  t->nil->right = NULL;
  t->nil->color = RBTREE_BLACK;

  return 0;
}

void inordered_arr(node_t *root, node_t *nil, key_t *arr, int *index){
  if(root == nil){
    return;
  }
  inordered_arr(root->left, nil, arr, index);
  arr[(*index)++] = root->key;
  inordered_arr(root->right, nil, arr, index);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int *index = calloc(1,sizeof(int));
  inordered_arr(t->root, t->nil, arr, index);
  free(index);
  return 0;
}
