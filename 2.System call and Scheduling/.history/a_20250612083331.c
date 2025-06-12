#include<stdio.h>


int
isnotempty(void)
{
  struct proc *p;
  int return_value = 0;
  for(p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);
    if(p->inQ == 1){
      return_value = 1;
    }
    release(&p->lock);
  }
  return return_value;
}