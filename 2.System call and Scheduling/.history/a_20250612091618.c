#include<stdio.h>


int arr[5] = {10,20,15,10,10};

int
random(void)
{
  // Take from http://stackoverflow.com/questions/1167253/implementation-of-rand
  static unsigned int z1 = 12345, z2 = 12345, z3 = 12345, z4 = 12345;
  unsigned int b;
  b  = ((z1 << 6) ^ z1) >> 13;
  z1 = ((z1 & 4294967294U) << 18) ^ b;
  b  = ((z2 << 2) ^ z2) >> 27; 
  z2 = ((z2 & 4294967288U) << 2) ^ b;
  b  = ((z3 << 13) ^ z3) >> 21;
  z3 = ((z3 & 4294967280U) << 7) ^ b;
  b  = ((z4 << 3) ^ z4) >> 12;
  z4 = ((z4 & 4294967168U) << 13) ^ b;

  return (z1 ^ z2 ^ z3 ^ z4) / 2;
}

// Return a random integer between a given range.
int
randomrange(int lo, int hi)
{
  if (hi < lo) {
    int tmp = lo;
    lo = hi;
    hi = tmp;
  }
  int range = hi - lo + 1;
  return random() % (range) + lo;
}

int main()
{
  int total_tickets = 0;
  for(int i= 0; i<5 ; i++) {
    total_tickets += arr[i];
  }
  int random_ticket = randomrange(1,total_tickets);
  printf("%d\n",random_ticket);
  int winner;
  for(int i=0; i<5; i++) {
      total_tickets-=arr[i];
      if(total_tickets<0) {
        winner = i;
        break;
    }
  }
  printf("%d\n")
}