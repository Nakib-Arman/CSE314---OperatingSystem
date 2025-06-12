#include<stdio.h>


int arr[5] = {0,1,1,1,1};

int isnotempty()
{
  int return_value = 0;
  for(int i=0; i<5;i++) {
    // acquire(&p->lock);
    if(arr[i] == 1){
      return_value = 1;
    }
    // release(&p->lock);
  }
  return return_value;
}

int main()
{
    int a = isnotempty();
    printf("%d\n",a);
}