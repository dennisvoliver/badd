/*
 * This is a 32-bit binary adder, technically.
 * It is a simulation of a real binary
 * adder made up of logic gates.
 * It deals with boolean objects, which,
 * in this implementation are chars.
 * There are two versions of this thing,
 * the fadd() and the sfadd().
 * The fadd() uses conventional gates
 * like the xor, and, or but it uses
 * nand gates at the bottom.
 * The sfadd() on the other hand uses
 * nothing but nand gates. It is a more
 * optimal but less readable version of
 * fadd().
 *
 * For the programmer's convenience, I
 * have also made integer-bool 
 * converters like the itob() and btoi()
 * but it is important that the bool
 * arrays be initialized with bset()
 * and the size must not exceed the
 * BITS constant.
 *
 * The program works well and has no
 * known bugs at the moment.
 * It is slow and inefficient, for sure
 * but those are not the goal of this
 * project. This is made entirely for
 * educational purposes.
 *
 * There are no comments in most of the
 * code because it was originally
 * written on paper. All the comments
 * and even the mistakes are written
 * there. Hopefully, you can find them.
 *
 * It can be expanded to do other things
 * like the computer books always say.
 * I hope you will enjoy using it.
 * Just don't think you can build it up
 * to the point where you'll need an
 * operating system.
 * Good luck.
 */

#include <stdio.h>
typedef char bool;
#define TRUE '1'
#define FALSE '0'
#define BITS 32
#define NUL '\0'
#define mod(x,y) ((x) - (((x) / (y)) * (y)))

bool sum[BITS + 1];
bool carry;

bool nand(bool a, bool b)
{
	if (a == TRUE)
		if (b == TRUE)
			return FALSE;
	return TRUE;
}

bool and(bool a, bool b)
{
/*	if (a == TRUE)
 *		if (b == TRUE)
 *			return TRUE;
 *	return FALSE;
 */
	a = nand(a, b);
	return nand(a, a);
}

bool or(bool a, bool b)
{
/*
 *	if (a == FALSE)
 *		if (b == FALSE)
 *			return FALSE;
 *	return TRUE;
 */
	a = nand(a, a);
	b = nand(b, b);
}

bool xor(bool a, bool b)
{
/*	if (a == TRUE) {
 *		if (b == TRUE)
 *			return FALSE;
 *	} else
 *		if (b == FALSE)
 *			return FALSE;
 *	return TRUE;
 */
	bool x, y;
	x = nand(a, a);
	y = nand(b, b);
	a = nand(a, b);
	b = nand(x, y);
	b = nand(a, b);
	return nand(b, b);
}

/* adds two bits */
bool hadd(bool a, bool b)
{
	bool x;
	x = xor(carry, a);
	carry = or(and(a, carry), and(b, x));
	return xor(b, x);
}

/* adds bytes */
void fadd(bool *a, bool *b)
{
	int i = 0;
	while (i < (BITS - 1))
	{	sum[i] = hadd(a[i], b[i]);
		i = i + 1;
		sum[i] = carry;
	}
		sum[i] = hadd(a[i], b[i]);
}

/* initializes boolean arrays */
void bset(bool *ar, int n)
{
	/* n is the number of
	 * objects in ar[] excluding
	 * the NUL
	 * it is assumed to be >= 1
	 */
	int i;
	i = 0;
	while (i < n)
	{	ar[i] = FALSE;
		i = i + 1;
	}
	ar[i] = NUL;
}

/* converts int to bool array */
int itob( int num, bool *out)
{
	int i;
	i = 0;
	while (num > 0)
	{	if (mod(num, 2) > 0)
			out[i] = TRUE;
		/* remove if out[i] is
		 * by default FALSE
		 */
		/* else out[i] = FALSE; */
		i = i + 1;
		num = num / 2;
	}
	/* number of bits converted */
	return i;
}

/* converts bool array to int */
int btoi( bool *ar)
{
	int num, i, j;
	num = i = 0;
	j = 1;
	while (i < BITS)
	{	if (ar[i] == TRUE)
			num = num + j;
			j = j * 2;
			i = i + 1;
	}
	return num;
}

/* faster hadd() */
bool shadd(bool a, bool b)
{
	bool x, y;
	x = nand(a, a);
	y = nand(carry, carry);
	carry = nand(carry, a);
	x = nand(nand(x, y), carry);
	y = nand(x, x);
	carry = nand(carry, nand(y, b));
	b = nand(nand(x, nand(b, b)), nand(b, y));
	return nand(b, b);
}

/* faster fadd */
void sfadd(bool *a, bool *b)
{
	int i = 0;
	while (i < (BITS - 1))
	{	sum[i] = shadd(a[i], b[i]);
		i = i + 1;
		sum[i] = carry;
	}
		sum[i] = shadd(a[i], b[i]);
}

int main(void)
{
	int i;
	bool a[BITS + 1];
	bool b[BITS + 1];
	carry = FALSE;
	bset(a, BITS);
	bset(b, BITS);
	bset(sum, BITS);
	itob(149, a);
	itob(150, b);
	/* fadd(a, b); */
	sfadd(a, b);
	printf("%d + %d = %d\n",
		btoi(a), btoi(b), btoi(sum));

}
