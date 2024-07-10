// A small terminal rain simulator to play around with
// Move with arrow keys
// quit with "q"

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define DELAY 50000

typedef struct {
  int x, y;
  int delay;
  int current_delay;
  int start_delay;
} Rain;

void random_rain(Rain rain[], int num_rain) {
  for (int i = 0; i < num_rain; i++) {
    rain[i].x = i;
    rain[i].y = -1;
    rain[i].delay = (rand() % 3) + 1;
    rain[i].current_delay = rain[i].delay;
    rain[i].start_delay =
        (rand() % LINES) *
        5; // this affects how quickly the rain comes, and how random it looks
  }
}

void draw_rain(Rain rain[], int num_rain) {
  for (int i = 0; i < num_rain; i++) {
    mvprintw(rain[i].y, rain[i].x, "|");
  }
}

void move_rain(Rain rain[], int num_rain) {
  for (int i = 0; i < num_rain; i++) {
    if (rain[i].start_delay > 0) {
      rain[i].start_delay--;
    } else if (rain[i].current_delay > 0) {
      rain[i].current_delay--;
    } else {
      rain[i].y += 1;
      rain[i].current_delay = rain[i].delay;
      if (rain[i].y >= LINES) {
        rain[i].x = rand() % COLS;
        rain[i].y = 0;
        rain[i].current_delay = rain[i].delay;
      }
    }
  }
}

int main() {
  initscr();
  int rows, cols;
  getmaxyx(stdscr, rows, cols);

  int ch;
  int x, y;
  int num_rain = rows * 5;
  Rain rain[num_rain];
  struct timespec ts = {0, DELAY * 1000};

  raw();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(FALSE);

  // init char position
  x = COLS / 2;
  y = LINES - 1;

  timeout(0); // non-blocking getch
  random_rain(rain, num_rain);

  // move things
  while ((ch = getch()) != 'q') {
    clear();

    move_rain(rain, num_rain);
    draw_rain(rain, num_rain);

    switch (ch) {
    case KEY_UP:
      y = (y > 0) ? y - 1 : y;
      break;
    case KEY_DOWN:
      y = (y < LINES - 1) ? y + 1 : y;
      break;
    case KEY_LEFT:
      x = (x > 0) ? x - 1 : x;
      break;
    case KEY_RIGHT:
      x = (x < COLS - 1) ? x + 1 : x;
      break;
    }

    mvprintw(y, x, "@");
    refresh();
    nanosleep(&ts, NULL);
  }

  endwin();
  return 0;
}
