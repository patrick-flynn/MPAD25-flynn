/* 
/ Title:
/
/ Description:
/
/ Author:
/
/ Created:
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>

int main(void) {
  unsigned int i=0;
  unsigned long l=0;
  unsigned long long ll=0;


  while (i+1 > i) i++;
  printf("MAXUINT is %u (0x%x)\n",i,i);

  printf("sizeof(uint) is %d\n",sizeof(i));

  while (l+1 > l) l++;
  printf("MAXULONG is %lu (0x%lx)\n",l,l);

  printf("sizeof(ulong) is %d\n",sizeof(l));

  while (ll+1 > ll) ll++;
  printf("MAXULONGLONG is %llu (0x%llx)\n",ll,ll);

  printf("sizeof(ulonglong) is %d\n",sizeof(ll));

  return 0;

}
