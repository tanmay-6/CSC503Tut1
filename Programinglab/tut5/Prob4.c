#include <stdio.h>
int *p;
int val;

void foo(int a){
  p = &a;
  val = *(p);
  printf("In foo\n");
}


void bar(int b){
  p = &b;
  *(--p) = val;
  printf("In bar\n");
}

int main(){
  int a = 1, b = 2;
  foo(a);
  bar(b);
  return 0;
}
