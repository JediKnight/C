#include <curses.h>

#define U 65			/* JUMP */
#define D 66			/* DOWN */
#define R 67			/* GOTO */
#define L 68			/* BACK */
#define S 32			/* JUMP */
#define B 98			/* DASH */

int initCurses()
{
  initscr();
  noecho();
  cbreak();
  start_color();
  use_default_colors();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  init_pair(2, COLOR_BLACK, COLOR_RED);
  return 0;
}

int opening()
{
  WINDOW *pad;
  int pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol, maxy, maxx, miny, minx, flg = D;
  pad = newpad(2, 4);
  wbkgd(pad, COLOR_PAIR(2));
  getmaxyx(stdscr, maxy, maxx);
  pminrow = pmincol = sminrow = smincol = 0;
  smaxrow = 2;
  smaxcol = 2;
  miny = minx = 0;
  for(;;)
    {
      switch(flg)
	{
	case D:
	  if(smaxrow < maxy)
	    { smaxrow++; sminrow++; }
	  else
	    { maxy--; flg = R; }
	  break;

	case R:
	  if(smaxcol < maxx)
	    { smaxcol++; smincol++; }
	  else
	    { maxx--; flg = U; }
	  break;

	case U:
	  if(sminrow > miny)
	    { smaxrow--; sminrow--; }
	  else
	    { miny++; flg = L; }
	  break;

	case L:
	  if(smincol > minx)
	  	{ smaxcol--; smincol--; }
	  else
	  	{ minx++; flg = D; }
	  break;
	}

      prefresh(pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);
      usleep(2000);
    }
  delwin(pad);
  return 0;
}

int jump()
{
  return 0;
}

int main()
{
  WINDOW *user;
  int key, skip, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol;

  initCurses();
  user = newpad(2, 4);
  wbkgd(user, COLOR_PAIR(1));
  skip = 0;
  pminrow = pmincol = sminrow = smincol = 0;
  smaxrow = 2;
  smaxcol = 2 * 2;

  opening(user);

  for(;;)
    {
      switch(key = getch())
	{
	case U:
	  sminrow--;
	  smaxrow--;
	  break;

	case D:
	  sminrow++;
	  smaxrow++;
	  break;

	case L:
	  smincol--;
	  smaxcol--;
	  break;

	case R:
	  smincol++;
	  smaxcol++;
	  break;

	case S:
	  if(jump() == -1)
	    { printw("ERROR!!\n何かキーを押して終了してね\n"); getch(); goto ENDHANDLING; }
	  break;

	case 'q':
	  goto ENDHANDLING;

	default:
	  break;
	}

      prefresh(user, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);
    }

 ENDHANDLING:
  endwin();
  return 0;
}
