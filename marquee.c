/*
 * File: Marquee.c
 * Name: Ian Booth
 * Date: 2026/02/05
 * Description:
 * Creates a moving LED marquee effect using keyboard LEDs.
 * keyboard inputs L/R/Q wsitch the pattern to left movement (L),
 * Right movement (R), and exiting the program (Q).
 */

#include <linux/kd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define WHLECONT 1

void changemode(int);
int kbhit(void);
void delay_ms(int);
void setLEDs(unsigned char);

/*
 * Description: Controls the marquee effect of the LEDS using user keyboard
 * input
 * @param a: none (void)
 * @return: 0 when program exits
 */
int main(void) {
  unsigned char leds = 0x01; // 0000 0001 (Starts at Scroll Lock)
  char direction = 'L';      // starts in the L direction
  char key;

  changemode(1);

  while (WHLECONT) { // continues while loop until q is pressed as while loop
                     // will always equal one

    if (kbhit()) { // Check keyboard without blocking program from running
      key = getchar();

      if (key == 'Q' || key == 'q') { // this if else uses key variable to see
                                      // which mode the program is in
        break;
      } else if (key == 'L' || key == 'l') {
        direction = 'L';
      } else if (key == 'R' || key == 'r') {
        direction = 'R';
      }
    }

    if (direction == 'L') { // this if else bit shifts the LED one to the right
                            // or left depending on mode
      leds <<= 1;
      if (leds > 0x04) { // if it goes past 3 in either direction it will reset
                         // the marquee loop
        leds = 0x01;
      }
    } else {
      leds >>= 1;
      if (leds == 0x00) { // same as above, reseting the LED marquee loop
        leds = 0x04;
      }
    }

    setLEDs(leds); // sets the led that is lit to bitshifted number
    delay_ms(500);
  }

  setLEDs(0x0);  // turn LEDs off when while loop is exited
  changemode(0); // sets changemode to zero which allows keyboard control
  return 0;
}

/*
 * Description: uses ioctl to set LEDs on keyboard either on or off
 * @param val: the bit value of the LEDs that need to turn on or off
 * @return: none
 * side effects: changes the keyboard state?
 */
void setLEDs(unsigned char val) { ioctl(1, KDSETLED, val); }

/*
 * Description: Delays program for specified amount of milliseconds using a
 * monotonic clock.
 * @param milliseconds: number of milliseconds to wait
 * @return: none
 * side effects: uses CPU while waiting
 */
void delay_ms(int milliseconds) {
  struct timespec start, now;
  clock_gettime(CLOCK_MONOTONIC, &start); // records the start time

  while (1) {
    clock_gettime(CLOCK_MONOTONIC, &now); // gets current time
    long elapsed =
        (now.tv_sec - start.tv_sec) * 1000 +
        (now.tv_nsec - start.tv_nsec) / 1000000; // computes the elapsed time
    if (elapsed >= milliseconds)                 // exits when delay is done
      break;
  }
}

/*
 * Description: either enables or disables keyboard control
 * @param dir: 1 to enable retreiving keyboard inputs without disturbing
 * program, 0 for regular operation
 * @return: none
 * side effects: modifies terminal settings, if the program exits without
 * returning them to normal it could be bad
 */
void changemode(int dir) {
  static struct termios oldt, newt; // stores the original and modified settings

  if (dir == 1) {
    tcgetattr(STDIN_FILENO, &oldt); // save the current terminal setting
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);        // disable canonical mode
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // applies new settings
  } else {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restores the original settings
  }
}

/*
 * Description: checks if a key has been pressed without blocking program
 * @param none:
 * @return: non-zero if a key press is available, 0 if no key press is available
 * side effects: none
 */
int kbhit(void) {
  struct timeval tv = {
      0L,
      0L}; // sets timeout for checking if button pressed to zero, does not wait
  fd_set fds;

  FD_ZERO(&fds); // clears the temporary file that holds button press
  FD_SET(STDIN_FILENO, &fds); // monitor standard input
  return select(STDIN_FILENO + 1, &fds, NULL, NULL,
                &tv); // checks if an input is ready
}
