
/*
 * skip.c
 *
 * Written by R. Sena (raff@aromatic.com)
 *
 *-------------------------------------------------------------
 *
 * A very simple tool to generate an opcode map file compatible
 * with dz80 disassembler
 *
 *-------------------------------------------------------------
 *
 * usage: skip address_range address_range...
 *
 * where: address ranges are in the form:
 *
 *        fromaddr-toaddr
 *
 * fromaddr,toaddr are hex values
 * and the addresses in the range fromaddr..toaddr-1 are marked
 * to be skipped
 *
 *-------------------------------------------------------------
 *
 * example: skip 100-200 1000-1234 7000-10000
 *
 * will generate an 'opcode map file' to skip the disassemble
 * of addresses in the ranges 0x0100..0x01FF 0x1000..0x1233  and
 * 0x7000..0xFFFF
 */
 
#define MEMSIZE	65536
#define MASK8	0xFF

unsigned char mem[MEMSIZE/8];

void mem_reset(void)
{
  int i;

  for (i=0; i < sizeof(mem); i++)
    mem[i] = MASK8;
}

void mem_skip(int min, int max)
{
  int pc;

  for (pc = min; pc < max; pc++)
    mem[pc >> 3] &= ~(1 << (pc & 7));
}

void print_mem(void)
{
  int fd = creat("opmap", 0666);
  int i;

#if 0
  for (i=0; i < sizeof(mem); i++)
    if (mem[i] != MASK8) printf("%04x: %02x\n", i * 8, mem[i]);
#else
  write(fd, mem, sizeof(mem));
#endif

  close(fd);
}

void get_range(char *s, int *min, int *max)
{
  sscanf(s, "%x-%x", min, max);
  printf("%x-%x\n", *min, *max);
}

main(int argc, char **argv)
{
  int i, min, max;

  mem_reset();
  
  for (i=1; i < argc; i++) {
    get_range(argv[i], &min, &max);
    mem_skip(min, max);
  }

  print_mem();
}
