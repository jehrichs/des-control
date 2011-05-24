
// compile witch gcc -O2 readsensors.c -o readsensors
// set root ownership
//chmod +s readsensors
#include <stdio.h>
#include <sys/io.h>

#define BASEPORT 0x0378

void bin_prnt_byte(int x)
{
   int n;
   int state1 = 0;
   int state2 = 1;
   int state3 = 1;
   int state4 = 1;
   
   for(n=0; n<6; n++)
   {
      if((x & 0x80) !=0)
      {
	switch(n) {
	  case 0:
	    state4 = 0;
	    break;
	  case 4:
	    state3 = 1;
	    break;
	  case 2:
	    state1 = 1;
	    break;
	  case 3:
	    state2 = 1;
	    break;
	}
      }
      else
      {
	switch(n) {
	  case 0:
	    state4 = 1;
	    break;
	  case 4:
	    state3 = 0;
	    break;
	  case 2:
	    state1 = 0;
	    break;
	  case 3:
	    state2 = 0;
	    break;
	}
      }
      x = x<<1;
   }
   
   printf("%d", state1);
   printf("%d", state2);
   printf("%d", state3);
   printf("%d", state4);
   printf("\n");
}

int main()
{

if(ioperm(BASEPORT, 3, 1)) {
	perror("ioperm");
	return 1;
}

outb(1, BASEPORT);
outb(1, BASEPORT + 1);
outb(1, BASEPORT + 1);

sleep(1);

//while(1) {
bin_prnt_byte(inb(BASEPORT + 1));

//usleep(10000);
//}

if(ioperm(BASEPORT, 3, 9)) {
	perror("ioperm");
	return 1;
}

return 0;
}

