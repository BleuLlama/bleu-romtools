/*
 *	Test program to print a message to printer port
 *	on the Don McKenzie Z80 development system
 */

#define		otport		0x1
#define		shkout		0x2
#define		control		0x3
/* we have to use the bc forms of in and out to guarantee that A13 == 0 */
#define		inb(p)		asm("\tld\tbc,#" #p "\n\tin\ta,(c)\n")
#define		outb(p)		asm("\tld\tbc,#" #p "\n\tout\t(c),a\n")
#define		outpi(p,v)	asm("\tld\ta,#" #v "\n"); outb(p)
#define		strobe		inb(0x2000)

in8255()
{
	int	i;
	/* delay to let circuit settle */
	for (i = 4096; i > 0; --i)
		;
	outpi(control,0xbd);
	outpi(control,0x05);
	outpi(control,0x09);
}

outchar(c)
	int	c;
{
	asm("loop:\n");
	inb(shkout);
	asm("\tbit\t6,a\n");
	asm("\tjr\tnz,loop\n");
	asm("\tbit\t1,a\n");
	asm("\tjr\tz,loop\n");
	c;
	asm("\tld\ta,l\n");
	outb(otport);
	strobe;
}

outstr(s)
	char	*s;
{
	while (*s) {
		outchar(*s);
		++s;
	}
}

main()
{
	char	i;

	in8255();
	outstr("Z80 Small C " "development system\r\n");
	for (i = '0'; i <= '9'; ++i)
		outchar(i);
	for (i = 'A'; i <= 'Z'; ++i)
		outchar(i);
	for (i = 'a'; i <= 'z'; ++i)
		outchar(i);
	outstr("\r\n");
}
