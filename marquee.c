/*
 * File: ledMarquee.c
 * Name: Ian Booth
 * Date: 2026/02/05
 * Description:
 * Creates a moving LED marquee effect using keyboard LEDs.
 * keyboard inputs L/R/Q wsitch the pattern to left movement (L),
 * Right movement (R), and exiting the program (Q).
 */

#include <linux/kd.h> // KDSETLED
#include <stdio.h>
#include <sys/ioctl.h> // ioctl()
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <termios.h> // keyboard control
#include <time.h>
#include <unistd.h> // usleep()

/* Function Prototypes */
void changemode(int);
int kbhit(void);
void delay_ms(int);
void setLEDs(unsigned char);

/* MAIN */
int main(void) {
  unsigned char leds = 0x01; // 0000 0001 (Scroll Lock)
  char direction = 'L';
  char key;

  changemode(1);

  while (1) {

    /* Check keyboard without blocking */
    if (kbhit()) {
      key = getchar();

      if (key == 'Q' || key == 'q') {
        break;
      } else if (key == 'L' || key == 'l') {
        direction = 'L';
      } else if (key == 'R' || key == 'r') {
        direction = 'R';
      }
    }

    /* Shift LED pattern */
    if (direction == 'L') {
      leds <<= 1;
      if (leds > 0x04) { // past Caps Lock
        leds = 0x01;
      }
    } else {
      leds >>= 1;
      if (leds == 0x00) {
        leds = 0x04;
      }
    }

    setLEDs(leds);
    delay_ms(500);
  }

  /* Cleanup */
  setLEDs(0x0); // turn LEDs off
  changemode(0);
  return 0;
}

/* Turn keyboard LEDs on/off */
void setLEDs(unsigned char val) { ioctl(1, KDSETLED, val); }

/* Delay in milliseconds */

void delay_ms(int milliseconds) {
  struct timespec start, now;
  clock_gettime(CLOCK_MONOTONIC, &start); // high-resolution timer

  while (1) {
    clock_gettime(CLOCK_MONOTONIC, &now);
    long elapsed = (now.tv_sec - start.tv_sec) * 1000 +
                   (now.tv_nsec - start.tv_nsec) / 1000000;
    if (elapsed >= milliseconds)
      break;
  }
}

/* Keyboard mode control (from kbhit.c) */
void changemode(int dir) {
  static struct termios oldt, newt;

  if (dir == 1) {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  } else {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  }
}

/* Check for key press without blocking */
int kbhit(void) {
  struct timeval tv = {0L, 0L};
  fd_set fds;

  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}
