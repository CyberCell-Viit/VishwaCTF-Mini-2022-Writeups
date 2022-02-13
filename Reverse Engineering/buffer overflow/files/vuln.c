#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <signal.h>

#define FLAGSIZE_MAX 64

char flag[FLAGSIZE_MAX];

void sigsegv_handler(int sig) {
  fprintf(stderr, "%s\n", flag);
  fflush(stderr);
  exit(1);
}
int new(char * input) {
  int check = 1;
  char * a[6] = {
    'B',
    'U',
    'F',
    'F',
    'E',
    'R'
  };
  for (int i = 0; i < 6; i++) {
    if (input[i] != a[i]) {
      check = 0;
      break;
    }
  }
  if (!check) {
   

    printf("\ninvalid first input.\n");
    return 0;

  } else
    return 1;
}
void vuln(char * input) {
  char buf[30];
  strcpy(buf, input);
}

int main(int argc, char ** argv) {

  FILE * f = fopen("flag.txt", "r");

  if (f == NULL) {
    printf("Flag File is Missing.\n");
    exit(0);
  }

  if (argc > 2) {

    printf("Received: %s\n", argv[2]);
  } else {
    printf("This program takes 2 argument.\n");
    return 0;
  }

  fgets(flag, FLAGSIZE_MAX, f);
  signal(SIGSEGV, sigsegv_handler);

  gid_t gid = getegid();
  setresgid(gid, gid, gid);
  if (!new(argv[1])) return 0;
  vuln(argv[2]);

  return 0;
}
