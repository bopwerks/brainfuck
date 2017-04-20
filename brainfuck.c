/* brainfuck.c -- A brainfuck interpreter. */
#include <stdio.h>
#include <stdlib.h>

/* maximum length of tape */
enum { MAXTAPE = 32768 };

/* tape to store program and data */
static unsigned char tape[MAXTAPE];

static void run(FILE *fp);

int
main(int argc, char *argv[])
{
  FILE *fp;
  if (argc < 2) {
    fprintf(stderr, "Usage: %s file\n", argv[0]);
    return EXIT_FAILURE;
  }
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    perror("fopen");
    return EXIT_FAILURE;
  }
  run(fp);
  fclose(fp);
  return EXIT_SUCCESS;
}

static void
run(FILE *fp)
{
  /* generic counter*/
  int i;

  /* temporary variable to store a given block */
  int block;

  /* the last-read character from the input */
  int ch;

  /* the program counter */
  int pc;

  /* the end of the program and start of the data */
  int pend;

  /* the data pointer */
  int data;

  /* the jumps lookup table maps the end of a block
   * to the start of the same block and vice versa.
   */
  int jumps[MAXTAPE];

  /* the blocks stack stores the start of each block */
  int blocks[100];
  int nblocks;

  /* initialize block stack */
  nblocks = 0;
  for (i = 0; i < 100; ++i)
    blocks[i] = 0;

  /* read the input onto the tape and compute the jumps */
  for (i = 0; i < MAXTAPE && (ch = fgetc(fp)) != EOF; ++i) {
    tape[i] = ch;
    jumps[i] = 0;
    if (ch == '[')
      blocks[nblocks++] = i;
    else if (ch == ']') {
      block = blocks[--nblocks];
      jumps[i] = block;
      jumps[block] = i;
    }
  }
  data = pend = i;
  while (i < MAXTAPE)
    tape[i++] = 0;
  
  /* display the jump list */
  /* for (i = 0; i < pend; ++i) */
  /*   printf("%c  ", tape[i]); */
  /* putchar('\n'); */
  /* for (i = 0; i < pend; ++i) */
  /*   printf("%-2d ", jumps[i]); */
  /* putchar('\n'); */
  /* return; */
  
  /* start the machine */
  for (pc = 0; pc < pend; ++pc) {
    switch (tape[pc]) {
      case '>':
        ++data;
        break;
      case '<':
        --data;
        break;
      case '+':
        ++tape[data];
        break;
      case '-':
        --tape[data];
        break;
      case '.':
        putchar(tape[data]);
        break;
      case ',':
        tape[data] = getchar();
        break;
      case '[':
        if (tape[data] == 0)
          pc = jumps[pc];
        break;
      case ']':
        if (tape[data] != 0)
          pc = jumps[pc];
        break;
      default:
        break;
    }
  }
}
