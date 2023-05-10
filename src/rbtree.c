#include "rbtree.h"
#include <stdlib.h>

// 새로운 rbtree를 생성하고
// root, nil 정보를 가진 포인터를 반환하는 함수
rbtree *new_rbtree(void)
{
  // 트리의 root, nil 정보를 담을 포인터 변수 p
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // nil노드도 노드이므로, 노드 만큼의 사이즈로 메모리를 할당해준다.
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));

  p->nil = nil_node;            // 포인터 변수와 nil노드를 이어준다.
  p->root = p->nil;             // root, nil노드를 이어준다. (아직 삽입된 노드가 없을 때의 초기 ver.)
  p->nil->color = RBTREE_BLACK; // nil노드는 항상 Black이므로, 색상을 지정해준다.

  return p; //새로운 rbtree 정보를 가진 포인터 변수 p를 반환한다.
}

// delete_rbtree의 보조 함수
// 재귀적으로 postorder로 순회하며 노드를 삭제한다.
void postorder_del(rbtree *t, node_t *root)
{
  if (root == t->nil) // 종료조건
    return;
  postorder_del(t, root->left);  // left
  postorder_del(t, root->right); // right
  free(root);                    // middle
}

// rbtree 전체 노드를 삭제하는 함수
void delete_rbtree(rbtree *t)
{
  postorder_del(t, t->root); // 모든 노드를 순회하며 삭제하고,
  free(t->nil);              // nil노드와
  free(t);                   // tree의 포인터 t까지 삭제해준다.
}

// parameter로 들어온 노드를 기준으로
// left rotation해주는 함수
void left_rotate(rbtree *t, node_t *x)
{
  // 우선 x의 오른쪽 자식, y를 설정한다.
  node_t *y = x->right;
  x->right = y->left;

  // β(y의 왼쪽 자식)가 존재한다면
  if (y->left != t->nil)
  {
    // β의 부모를 y에서 x로 변경해준다.
    y->left->parent = x;
  }

  // left rotate 시 부모 자식 관계에 변화를 반영한다.
  // y의 부모를 x에서 x의 부모로 변경해준다.
  y->parent = x->parent;
  // 이 때 x->parent == t->nil 이면,
  if (x->parent == t->nil)
  {
    // 새로운 루트는 y가 된다.
    t->root = y;
  }
  // x의 부모가 nil이 아니고, x가 그 부모의 왼쪽 자식이었다면
  else if (x == x->parent->left)
  {
    // x의 부모의 새로운 왼쪽 자식을 y로 연결한다.
    x->parent->left = y;
  }
  // x가 그 부모의 오른쪽 자식이었다면
  else
  {
    // x의 부모의 새로운 오른쪽 자식을 y로 연결한다.
    x->parent->right = y;
  }

  y->left = x;   // y의 왼쪽 자식을 β에서 x로 변경해 연결한다.
  x->parent = y; // x의 부모를 y로 변경한다.
}

// parameter로 들어온 노드를 기준으로
// right rotation해주는 함수
void right_rotate(rbtree *t, node_t *y)
{
  // 우선 y의 왼쪽 자식, x를 설정한다.
  node_t *x = y->left;
  y->left = x->right;

  // β(x의 오른쪽 자식)가 존재한다면
  if (x->right != t->nil)
  {
    // β의 부모를 x에서 y로 변경해준다.
    x->right->parent = y;
  }

  // left rotate 시 부모 자식 관계에 변화를 반영한다.
  // x의 부모는 y에서 y의 부모로 변경한다.
  x->parent = y->parent;
  // 이 때 y->parent == t->nil 이면,
  if (y->parent == t->nil)
  {
    // 새로운 루트는 x가 된다.
    t->root = x;
  }
  // y의 부모가 nil 이 아니고, y가 그 부모의 왼쪽 자식이었다면
  else if (y == y->parent->left)
  {
    // y의 부모의 새로운 왼쪽 자식을 x로 연결한다
    y->parent->left = x;
  }
  // y가 그 부모의 오른쪽 자식이었다면
  else
  {
    // y의 부모의 새로운 왼쪽 자식을 x로 연결한다.
    y->parent->right = x;
  }

  x->right = y;  // x의 왼쪽 자식을 β에서 y로 변경해 연결한다.
  y->parent = x; // y의 부모를 x로 변경한다.
}

// rbtree 삽입연산 시 조건에 위배되면 고쳐주는 함수
void rbtree_insert_fixup(rbtree *t, node_t *z)
{
  // 각 if, else문에서 사용할
  // 삼촌 노드 하나 미리 선언
  node_t *uncle = NULL;

  // insert할 노드가 root 노드가 아닐 때,
  // (root이면 바로 while문 아래서 Black으로 변경 후 종료)
  // 부모 노드가 red일 때가 문제이므로
  while (z->parent->color == RBTREE_RED)
  {
    // 부모가 할아버지의 왼쪽 자식일 때
    if (z->parent == z->parent->parent->left)
    {
      // uncle은 오른쪽 자식이다.
      uncle = z->parent->parent->right;
      // 1) 부모 삼촌 모두 레드일 때
      if (uncle->color == RBTREE_RED)
      {
        // parent와 uncle 모두 black으로 바꾼다.
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;

        // grandparent를 red로 바꾼 뒤,
        // 포인터를 grandparent로 옮겨서
        // 그의 parent도 red인지 while문으로 다시 한 번 확인하게 한다.
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        // 2) 부모 레드 & 삼촌 블랙 & 내가 right child일 때
        if (z == z->parent->right)
        {
          // 포인터 parent로 올리고, left-rotate
          z = z->parent;
          left_rotate(t, z);
        }
        // 3) 부모 레드 & 삼촌 블랙 & 내가 left child일 때
        // parent는 black으로, grandparent를 red로 change
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;

        // 포인터를 grandparent로 옮겨서 right-rotate
        right_rotate(t, z->parent->parent);
      }
    }
    // 부모가 할아버지의 오른쪽 자식일 때
    // 위와 left, right만 반대이므로 주석 생략
    else
    {
      uncle = z->parent->parent->left;
      if (uncle->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  // insert할 노드가 root 노드일 때
  // 색만 바로 바꿔주면 해결!
  t->root->color = RBTREE_BLACK;
}

// rbtree에 노드를 삽입해주는 함수
node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // 삽입할 key값을 가진 노드를 만들어준다.
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;

  // 자리를 찾아 내려갈 ptr 변수 x, y
  node_t *x = t->root;
  node_t *y = t->nil;

  // 새로운 노드가 들어갈 자리를 찾아준다.
  while (x != t->nil)
  {
    // while문이 매번 새로 돌 때 마다,
    // x가 가리키는 주소 값을 y도 가리키도록 바꿔준다.
    // 마지막에 x가 nil노드에 도달했을 때,
    // y가 직전 노드의 주소를 가지고 있도록 하기 위함이다.
    y = x;
    // 삽입할 값이 작다면 왼쪽으로 간다.
    if (x->key > key)
    {
      x = x->left;
    }
    // 삽입할 값이 크거나 같다면 오른쪽으로 간다.
    else
    {
      x = x->right;
    }
  }

  // 자리를 찾았으니 부모 자식 관계를 이어준다.
  z->parent = y;
  // 만약 들어갈 자리가 nil노드라면,
  if (y == t->nil)
  {
    //삽입될 값이 최초의 루트노드이다.
    t->root = z;
  }
  // nil노드가 아니라면 key값의 크기에 따라 left, right 자리를 정해준다.
  // right child일 경우
  else if (y->key <= key)
  {
    y->right = z;
  }
  // left child일 경우
  else
  {
    y->left = z;
  }

  // 빨간색으로 칠해주고, left, right child에 nil노드를 연결한다.
  z->color = RBTREE_RED;
  z->left = t->nil;
  z->right = t->nil;

  // 새로 삽입된 값에 의해
  // RB tree의 조건이 위배되었을 수 있으므로, 고쳐준다.
  rbtree_insert_fixup(t, z);

  return t->root;
}

// 특정 key값을 가지는 노드의 포인터를 반환하는 함수
node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // key의 위치를 찾아 내려가기 위한 ptr 변수 x 선언
  node_t *x = t->root;

  // x가 가리키는 노드가 nil 노드가 될 때 까지 탐색
  while (x != t->nil)
  {
    // 찾으려는 key값 보다 x->key가 크면
    if (x->key > key)
    {
      // 작은 값을 찾아 left child로 이동
      x = x->left;
    }
    // 찾으려는 key값 보다 x->key가 크면
    else if (x->key < key)
    {
      // 큰 값을 찾아 right child로 이동
      x = x->right;
    }
    else
    { // 키를 찾았으면 리턴
      return x;
    }
  }
  // 찾는 키가 없으면
  return NULL;
}

// 전체 tree의 최솟값 포인터를 반환하는 함수
node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *x = t->root;
  while (x->left != t->nil)
  {
    x = x->left;
  }
  return x;
}

// 전체 tree의 최댓값 포인터를 반환하는 함수
node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  node_t *x = t->root;
  while (x->right != t->nil)
  {
    x = x->right;
  }
  return x;
}

// subtree의 최솟값 포인터를 반환하는 함수
node_t *subtree_min(const rbtree *t, node_t *z)
{
  node_t *ptr = z;
  while (ptr->left != t->nil)
  {
    ptr = ptr->left;
  }
  return ptr;
}

// 부모 자식 관계를 재설정해주는 함수
void rb_transplant(rbtree *t, node_t *u, node_t *v)
{
  // 지우려는 노드가 루트노드였으면 자식노드를 루트로 설정한다.
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  // 지우려는 노드가 왼쪽 자식이면, 부모의 왼쪽 자식노드를 왼쪽자식으로 연결한다.
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  // 지우려는 노드가 오른쪽 자식이면, 부모의 오른쪽 자식노드를 오른쪽자식으로 연결한다.
  else
  {
    u->parent->right = v;
  }
  // 지우려는 노드의 자식의 부모를, 지우려는 노드의 부모로 연결한다.
  v->parent = u->parent;
}

// 삭제연산으로 인해 rb tree 조건이 위반된 경우
// 조건을 만족하도록 조정해주는 함수
void rb_erase_fixup(rbtree *t, node_t *x)
{
  // x는 y(successor)의 원래 위치로 이동하는 노드이다.
  // x의 형제노드 bro
  node_t *bro = t->nil;

  // x가 루트노드가 아니고, doubly black일 때
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    // x가 왼쪽 자식이면
    if (x == x->parent->left)
    {
      // 형제노드는 오른쪽 자식이다.
      bro = x->parent->right;
      // doubly black 의 형제가 레드이면 case 1을 적용해서 해결.
      if (bro->color == RBTREE_RED)
      {
        bro->color = RBTREE_BLACK;     // 형제를 블랙으로 만든 뒤
        x->parent->color = RBTREE_RED; // 부모를 RED로 바꾸고
        left_rotate(t, x->parent);     // 부모를 기준으로 rotation 한다.
        bro = x->parent->right;        // 새로운 형제를 연결해준다.
      }
      // 형제와 그의 자식 모두 블랙일 때 case 2 적용해서 해결.
      if (bro->left->color == RBTREE_BLACK && bro->right->color == RBTREE_BLACK)
      {

        bro->color = RBTREE_RED; // doubly black과 그 형제의 블랙을 모아 부모에게 전달한 뒤,
        x = x->parent;           // 부모가 extra black을 해결하도록 위임한다.
      }
      // 형제가 블랙일 때
      else
      {
        // 형제의 오른쪽 자식 블랙, 왼쪽 자식 레드일 때 case 3을 적용해서 해결.
        if (bro->right->color == RBTREE_BLACK)
        {

          bro->left->color = RBTREE_BLACK; // 형제와 왼쪽 자식의 색을 교환하고,
          bro->color = RBTREE_RED;
          right_rotate(t, bro);   // 형제를 기준으로 rotation 하고
          bro = x->parent->right; // 새로운 형제를 연결해준다.
        }
        // 형제 블랙, 그의 오른쪽 자식은 레드일 때 case 4를 적용해서 해결
        bro->color = x->parent->color; // 형제와 부모의 색을 교환해준 뒤
        x->parent->color = RBTREE_BLACK;
        bro->right->color = RBTREE_BLACK; // 형제의 자식에게 extra black을 준다.
        left_rotate(t, x->parent);        // 부모를 기준으로 rotation 하고
        x = t->root;                      // x 포인터를 트리의 루트로 향하게 한다. (문제 해결 완료 && while문 종료!)
      }
    }
    // x가 오른쪽 자식이면
    // if문과 left, right만 바꾸면 동일
    else
    {
      bro = x->parent->left;
      if (bro->color == RBTREE_RED)
      {
        bro->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        bro = x->parent->left;
      }
      if (bro->right->color == RBTREE_BLACK && bro->left->color == RBTREE_BLACK)
      {
        bro->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (bro->left->color == RBTREE_BLACK)
        {
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
  // 모든 조건을 만족시켰거나, x가 루트였고 red인 경우,
  // 블랙으로 변경하여 fixup을 마무리한다.
  x->color = RBTREE_BLACK;
}

// 특정 노드를 삭제하는 함수
int rbtree_erase(rbtree *t, node_t *z)
{
  // x는 y(successor)의 원래 위치로 이동하는 노드.
  // node y는 트리에서 삭제된 노드,
  // OR 트리에서 이동한 노드를 가리킴.
  node_t *x = NULL;
  node_t *y = z;
  color_t y_origin = y->color; // y의 색이 변경될 수 있으므로 변경 전 색을 y-original-color에 저장

  // z가 하나 이하의 자식을 가지면, 자식과 부모를 바로 연결하고 삭제한다.
  // 1) z의 오른쪽 노드에 자식이 있을 수 있을 때
  if (z->left == t->nil)
  {
    x = z->right;
    rb_transplant(t, z, z->right);
  }
  // 2) z의 왼쪽 노드에 자식이 있을 수 있을 때
  else if (z->right == t->nil)
  {
    x = z->left;
    rb_transplant(t, z, z->left);
  }
  // 3) 지우려는 노드의 왼쪽, 오른쪽 자식 모두 있을 때
  else
  {
    // y는 지우려는 노드의 successor를 가리키도록 한다.
    y = subtree_min(t, z->right);
    y_origin = y->color; // y_original_color를 successor y의 색으로 변경한다.
    x = y->right;        // x는 successor y의 오른쪽 자식을 가리키도록 한다.
    // 3-1) successor y가 직속 자식에 바로 있을 때
    if (y->parent == z)
    {
      x->parent = y; // x의 부모로 y를 이어준다.
    }
    // 3-2) successor y가 직속 자식에 바로 있지 않을 때
    else
    {
      // 'y의 오른쪽 자식 <-> y의 부모'간 부자 관계 설정을 다시 한다.
      // successor를 옮기기 전 자식노드들의 새로운 부모를 연결해야 하기 때문이다.
      rb_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    // 'y <-> z의 부모'간 부자 관계 설정을 다시 한다.
    rb_transplant(t, z, y);
    // y가 이제 z의 부모의 새로운 자식이 되었으니, z의 왼쪽 자식을 넘겨 받는다.
    y->left = z->left;
    y->left->parent = y;
    // y가 z자리로 갔으니 color를 z의 컬러로 변경한다.
    y->color = z->color;
  }
  // 삭제한 color가 Doubly Black이면
  // 2, 4, 5번 조건을 위반할 수 있으므로 고치러 보낸다.
  if (y_origin == RBTREE_BLACK)
  {
    rb_erase_fixup(t, x);
  }
  free(z);
  return 0;
}

// rbtree_to_array의 보조함수
// inorder순으로 순회하며 arr값을 변경한다.
int rb_inorder(node_t *root, key_t *res, const rbtree *t, int i)
{
  if (root == t->nil) // 종료 조건
    return i;

  i = rb_inorder(root->left, res, t, i);
  res[i] = root->key;
  i = rb_inorder(root->right, res, t, i + 1);
  return i;
}

// rbtree를 key값 순으로 변경하는 함수
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  rb_inorder(t->root, arr, t, 0);
  return 0;
}