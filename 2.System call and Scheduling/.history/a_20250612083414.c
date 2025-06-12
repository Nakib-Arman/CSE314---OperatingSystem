#include<stdio.h>




int
isnotempty(void)
{
  int return_value = 0;
  for(int i=0; p < &proc[NPROC]; p++) {
    acquire(&p->lock);
    if(p->inQ == 1){
      return_value = 1;
    }
    release(&p->lock);
  }
  return return_value;
}

int main()