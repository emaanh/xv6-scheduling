#include "types.h"
#include "user.h"
#include "date.h"

char *months[] = {
  "January", "February", "March", "April", "May", "June",
  "July", "August", "September", "October", "November", "December"
};

void print_2digit(int fd, int n) {
  if(n < 10) {
    printf(fd, "0%d", n);
  } else {
    printf(fd, "%d", n);
  }
}

void print_date(struct rtcdate r) {
  int hour = r.hour;
  char *ampm = "am";

  if (hour >= 12) {
    ampm = "pm";
  }
  if (hour == 0) {
    hour = 12;
  }
  if (hour > 12) {
    hour -= 12;
  }

  //xv6 printf does not support %02d, so i printed the leading 0s manually.
  printf(1, "%d:", hour);
  print_2digit(1, r.minute);
  printf(1, ":");
  print_2digit(1, r.second);
  printf(1, "%s %s %d, %d\n", ampm, months[r.month-1], r.day, r.year);
}


int
main(int argc, char *argv[])
{
    struct rtcdate r;
    if (date(&r)) {
        printf(2, "date failed\n");
        exit();
    }
    print_date(r);
    exit();
}