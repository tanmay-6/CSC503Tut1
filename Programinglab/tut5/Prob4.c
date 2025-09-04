#include <stdio.h>
#include <stdint.h>
int *p;
int val;

void foo(int a){
  p = &a;
  val = *(--p);
  printf("In foo\n");
}


void bar(int b){
  p = &b;
  *(--p) = val;
  printf("In bar\n");
}

int main(){
  uintptr_t address = 0x7fffffffceb8;
  long int *ptr = (long int *)address;
  *ptr = 0x000055555555522e;
  //0x000055555555522e 
  int a = 1, b = 2;
  foo(a);
  bar(b);
  return 0;
}
