#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>

/* 

   Compilation:
   gcc -o speedrun-008 -O0 -no-pie -static speedrun-008.c && strip speedrun-008

   Also need to take the glibc version to give to folks.

 */

__attribute__((constructor)) void set_canary()
{  
   int canary_fd = open("/flag", O_RDONLY);
   uint64_t new_canary;
   
   while (1)
   {
	  char cur;
	  char last;
	  ssize_t num;
	  num = read(canary_fd, &cur, 1);
	  if (num == 0)
	  {
		 break;
	  }
	  last = (new_canary & 0xFF00000000000000) >> 56;
	  new_canary <<= 8;
	  new_canary ^= (cur ^ last);
   }
   close(canary_fd);

   *(uint8_t *)&new_canary = 0;
   uint64_t old_canary;
   __asm__ __volatile__ (
	  ".intel_syntax noprefix;"
	  "mov %0, QWORD PTR fs:0x28;"
	  ".att_syntax;"
	  : "=r"(old_canary)
	  :
	  :
	  );

   uint64_t *canary_ptr = &old_canary;

   while (1)
   {
	  canary_ptr++;
	  if (*canary_ptr == old_canary)
	  {
		 *canary_ptr = new_canary;
		 break;
	  }
   }

   __asm__ __volatile__ (
	  ".intel_syntax noprefix;"
	  "mov rax, %0;"
	  "mov QWORD PTR fs:0x28, rax;"
	  ".att_syntax;"
	  :
	  : "r"(new_canary)
	  : "%rax"
	  );

   new_canary = 0;

   return;
}

void say_hello()
{
   printf("More racing? Haven't you had enough?\n");
}

void what_do_they_say()
{
   char buf[1024];

   printf("Yes?\n");
   read(0, buf, 2015);
   write(1, "Whatever\n", strlen("Whatever\n"));
}

void say_goodbye()
{
   printf("Peace out.\n");
}


int main(int argc, char** argv)
{
   setvbuf(stdout, NULL, _IONBF, 0);

   if (getenv("DEBUG") == NULL)
   {
	  alarm(5);
   }
   
   say_hello();
   what_do_they_say();
   say_goodbye();
}
