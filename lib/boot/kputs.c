#include "kputs.h"

static int curx=0,cury=0;
static const int width=80,height=25;
static volatile unsigned char* screen=(volatile unsigned char*)0xb8000;

#define SCR(x,y) (screen[2*((x)+width*(y))])
#define ATTR(x,y) (screen[2*((x)+width*(y))+1])

int kterpri(void)
{
  curx=0;
  cury++;
  if (cury>=height)
    {
      int x,y;
      for (y=0; y<(height-1); y++)
        for (x=0; x<width; x++)
	  {
	    SCR(x,y)=SCR(x,y+1);
	    ATTR(x,y)=ATTR(x,y+1);
	  }
      for (x=0; x<width; x++)
	{
	  SCR(x,(height-1))=' ';
	  ATTR(x,(height-1))=7;
	}
      cury--;
    }

  return 0;
}

int kcls(void)
{
  int x,y;
  for (y=0; y<height; y++)
    for (x=0; x<width; x++)
      {
	SCR(x,y)=' ';
	ATTR(x,y)=7;
      }
  curx=cury=0;
  return 0;
}

int kputc(unsigned char ch)
{
  switch (ch)
    {
    case '\t':
      curx+=(8-(curx%8));
      if (curx>=width)
        if (kterpri()<0)
          return -1;
      break;

    case '\n':
      if (kterpri()<0)
        return -1;
      break;

    case 014: /* ^L */
      if (kcls()<0)
        return -1;
      break;

    default:
      SCR(curx,cury)=ch;
      ATTR(curx,cury)=7;
      curx++;
      if (curx>=width)
        if (kterpri()<0)
          return -1;
      break;
    }
  return 0;
}

int kputs(const char* s)
{
  if (!s) return -1;
  while (*s)
    {
      if (kputc(*s++)<0)
        return -1;
    }
  return 0;
}
