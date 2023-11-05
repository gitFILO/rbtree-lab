#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
void rb_insert_fixup(rbtree *t, node_t *z);
void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  // 안의 노드 먼저 전부 free 해야함
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *x = t->root;
  node_t *y = t->nil;
  if(t->root == NULL){
    node_t *z = (node_t *)malloc(sizeof(node_t));
    z->left = t->nil;
    z->right = t->nil;
    z->color = RBTREE_RED;
    z->key = key;
    t->root = z;
    return z;
  }
  while(x != t->nil){
    y = x;
    if( key < x->key) x = x->left;
    else x = x->right;
  }
  node_t *z = (node_t *)malloc(sizeof(node_t));
  z->key = key;
  z->parent = y;
  if(y == t->nil) t->root = z;
  else if( z->key < y->key) y->left = z;
  else y->right = z;
  
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  rb_insert_fixup(t,z);

  return z;
}
void rb_insert_fixup(rbtree *t, node_t *z){
  while(z->parent->color == RBTREE_RED){
    if(z->parent == z->parent->parent->left){ 
      node_t *y = z->parent->parent->right;
      if(y->color == RBTREE_RED){ //case 1: 부모의 형제도 red인 경우
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color =  RBTREE_RED;
        z = z->parent->parent; // 할아버지에서 다시 확인해야함
      }
      else if (z == z->parent->right){ // case 2: 삽입된 노드가 부모의 오른쪽, 부모는 할아버지의 왼쪽, 부모 형제 black
        z = z->parent;
        left_rotate(t,z); // case 3로 바꿔서 해결
      }
      z->parent->color = RBTREE_BLACK;  // case 3: 삽입된 노드가 부모의 왼쪽, 부모는 red && 할아버지 왼쪽, 부모 형제는 black
      z->parent->parent->color = RBTREE_RED;
      right_rotate(t,z->parent->parent);
    }
    else{ // 중복되는 코드: 리팩토링 대상
      node_t *y = z->parent->parent->left;
      if(y->color == RBTREE_RED){ //case 1: 부모의 형제도 red인 경우
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color =  RBTREE_RED;
        z = z->parent->parent; // 할아버지에서 다시 확인해야함
      }
      else if (z == z->parent->left){ // case 2: 위 함수의 대칭
        z = z->parent;
        right_rotate(t,z); // case 3로 바꿔서 해결
      }
      z->parent->color = RBTREE_BLACK;  // case 3: 위 함수의 대칭
      z->parent->parent->color = RBTREE_RED;
      left_rotate(t,z->parent->parent); // 
      
    }
  }
  t->root->color = RBTREE_BLACK;
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left;
  if(y->left != t->nil) y->left->parent = x;
  y->parent = x->parent;
  if(x->parent == t->nil) t->root = y;
  else if(x == x->parent->left) x->parent->left = y;
  else x->parent->right = y;

  y->left = x;
  x->parent = y;
}
void right_rotate(rbtree *t, node_t *x){
  node_t *y = x->left;
  x->left = y->right;
  if(y->right != t->nil) y->right->parent = x;
  y->parent = x->parent;
  if(x->parent == t->nil) t->root = y;
  else if(x == x->parent->right) x->parent->right = y;
  else x->parent->left = y;

  y->right = x;
  x->parent = y;
}
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *cur = t->root;
  if(cur == NULL || cur == t->nil) return NULL;

  while(cur != t->nil){
    if (cur->key == key){
      return cur;
    }
    if (cur->key > key) cur = cur->left;
    else if (cur->key < key) cur = cur->right;
  }
  return NULL; //찾지 못하고 nil node까지 온 경우
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  
  while(cur->left != t->nil) cur = cur->left; // cur의 왼쪽자식이 nil이면 종료

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *cur = t->root;
  
  while(cur->right != t->nil) cur = cur->right; // cur의 오른쪽자식이 nil이면 종료

  return cur;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
