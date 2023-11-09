#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
void rb_insert_fixup(rbtree *t, node_t *z);
void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);
void inorder(const rbtree *t,node_t *cur, key_t *arr,int num, const size_t n,int *visited);
void free_rbtree_nodes(rbtree *t,node_t* cur);
void rb_delete_fixup(rbtree *t, node_t *x);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  NIL->color = RBTREE_BLACK;
  p->nil = NIL;
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  free_rbtree_nodes(t,t->root); // 먼저 트리 내의 모든 노드를 free!
  free(t->nil); // NIL free
  free(t); // 트리 free
}

void free_rbtree_nodes(rbtree *t,node_t* cur) {
    if (cur == t->nil) {
        return;
    }

    // 왼쪽 서브트리 해제
    free_rbtree_nodes(t, cur->left);

    // 오른쪽 서브트리 해제
    free_rbtree_nodes(t, cur->right);

    // 노드 메모리 해제
    free(cur);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {

  // TODO: implement insert
  node_t *x = t->root;
  node_t *y = t->nil;
  if(t->root == t->nil){
    node_t *z = (node_t *)malloc(sizeof(node_t));
    z->left = t->nil;
    z->right = t->nil;
    z->color = RBTREE_BLACK;
    z->key = key;
    t->root = z;
    z->parent = t->nil; // 루트의 부모는 Nil
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
  if( z->key < y->key) y->left = z;
  else y->right = z;
  
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  rb_insert_fixup(t,z);

  return z;
}

void print_tree(rbtree *t,node_t *cur){
  if(cur == t->nil) return;
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
        continue;
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
        continue;
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
  print_tree(t,t->root);
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
  if ( cur == t->nil) return cur;
  while(cur->left != t->nil) cur = cur->left; // cur의 왼쪽자식이 nil이면 종료

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *cur = t->root;
  
  while(cur->right != t->nil) cur = cur->right; // cur의 오른쪽자식이 nil이면 종료

  return cur;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil) t->root = v;
  else if(u == u->parent->left) u->parent->left = v;
  else u->parent->right = v;

  v->parent = u->parent;
  
}
node_t *find_successor(rbtree *t,node_t *cur){
  node_t *successor = cur;
  if( cur == t->nil) return cur;

  while( successor->left != t->nil) successor = successor->left;
  return successor;
}
int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z;
  color_t delete_color = y->color;
  node_t *x;
  if(z->left == t->nil){ // 왼쪽 자식 없음
    x = z->right;
    rb_transplant(t,z,z->right);  
  }
  else if (z->right ==t->nil){ // 오른쪽 자식 없음
    x = z->left;
    rb_transplant(t,z,z->left);
  }
  else { // 자식이 두명 -> successor의 컬러를 지워야함
    y = find_successor(t,z->right);
    delete_color = y->color;
    x = y->right;

    if (y->parent == z) {
      x->parent = y;
    } 
    else{
      rb_transplant(t,y,y->right);
      y->right = z->right;
      y->right->parent = y;
    }
  
    rb_transplant(t,z,y);
    y->left= z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (delete_color == RBTREE_BLACK) rb_delete_fixup(t,x);
  print_tree(t,t->root);
  free(z);
  return 0; 
}
void rb_delete_fixup(rbtree *t, node_t *x){
  node_t *w;
  while( x != t->root && x->color == RBTREE_BLACK){
    if( x == x->parent->left){
      w = x->parent->right;
      if( w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t,x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t,w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t,x->parent);
          x = t->root;
        }
    }
    else{ // x == x->parent->right
       w = x->parent->left;
      if( w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t,w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t,x->parent);
        x = t->root;
        }
    } 
  }
  x->color = RBTREE_BLACK;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int *visited = (int *)calloc(1,n * sizeof(int*));

  inorder(t,t->root,arr,0,n,visited);
  free(visited);
  return 0;
}

void inorder(const rbtree *t,node_t *cur, key_t *arr,int num, const size_t n,int *visited){ //need test..!

  if(cur == t->nil) return;

  inorder(t,cur->left,arr,num,n,visited);

  if(cur != t->nil && num < n){
    for(int i=0;i<n;i++){
      if( visited[i] == 0){
        arr[i] = cur->key;
        visited[i] = 1;
        break;
      }
    }
  }
  inorder(t,cur->right,arr,num,n,visited);
  
}