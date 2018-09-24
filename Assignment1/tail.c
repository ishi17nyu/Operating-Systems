#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
char buf[1024];

void
tail(int fd,int nol)
{
  int count,i,n,m;
  count = 0;
  char *p ,*q;
  int ptr_tailDump;

  ptr_tailDump = open ("tailDump", O_CREATE | O_RDWR);
  while((n = read(fd, buf, sizeof(buf))) > 0)
  {
    write(ptr_tailDump,buf,n);
      for(i=0; i<n; i++){
       if(buf[i] == '\n')
        {
         count++;
        }
       }
   }

  close(fd);
  close(ptr_tailDump);

  ptr_tailDump = open ("tailDump", 0);
  if(count <= nol)
  {
    while((n = read(ptr_tailDump, buf, sizeof(buf))) > 0)
      write(1, buf, n);
    if(n < 0)
    {
      printf(1, "tail: read error\n");
    }
  }
  else
  {
    m = 0;
    while((n = read(ptr_tailDump, buf+m, sizeof(buf)-m)) > 0){
    m += n;
    p = buf;
    while((q = strchr(p, '\n')) != 0){
      count--;
      if(count < nol)
      {
        *q = '\n';
        write(1, p, q+1 - p);
      }
      *q = 0;
      p = q+1;
    }
    if(p == buf)
      m = 0;
    if(m > 0){
      m -= p - buf;
      memmove(buf, p, m);
    }
  }
 }
 close(ptr_tailDump);
 unlink("tailDump");
}

int
main(int argc , char *argv[])
{
int fd;

if(argc <= 1)
{
   tail(0, 10);
}
if(argc == 2)
{
    if( argv[1][0] == '-')
    {
     int nol = atoi(++argv[1]);
     tail(0, nol);
    }
    else
    {
    if((fd = open(argv[1], 0)) < 0){
    printf(1, "tail: cannot open %s\n", argv[1]);
    exit();
    }
    tail(fd, 10);
    }
}
if(argc > 2)
{
  if((fd = open(argv[2], 0)) < 0){
    printf(1, "tail: cannot open %s\n", argv[2]);
    exit();
  }
   int nol = atoi(++argv[1]);
   tail(fd, nol);
}

exit();
}
