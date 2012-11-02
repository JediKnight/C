#include <ncurses.h>

#undef KEY_UP
#define KEY_UP 65
#undef KEY_DOWN
#define KEY_DOWN 66
#undef KEY_RIGHT
#define KEY_RIGHT 67
#undef KEY_LEFT
#define KEY_LEFT 68

int main()
{
  WINDOW *pad;
  int maxY, maxX, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol, i, key;

  initscr();
  noecho();
  cbreak();
  start_color();
  use_default_colors();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  getmaxyx(stdscr, maxY, maxX);
  pminrow = 0;
  pmincol = 0;
  sminrow = 0;
  smincol = 0;
  smaxrow = maxY;
  smaxcol = 25;
  pad = newpad(maxY, 30);
  wbkgd(pad, COLOR_PAIR(1));

  prefresh(pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);

  for(i = 0; i < 10000000; i++)
    {
      switch(key = getch())
	{
	case KEY_UP:
	  sminrow--;
	  smaxrow--;
	  break;

	case KEY_DOWN:
	  sminrow++;
	  smaxrow++;
	  break;

	case KEY_RIGHT:
	  smincol++;
	  smaxcol++;
	  break;

	case KEY_LEFT:
	  smincol--;
	  smaxcol--;
	  break;

	case 'q':
	  goto ENDHANDLING;
	}

      prefresh(pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);
      usleep(500);
    }

 ENDHANDLING:
  endwin();
  delwin(pad);
}
