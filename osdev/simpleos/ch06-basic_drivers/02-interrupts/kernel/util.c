#include "util.h"


/*
 * memory_copy copies nbytes of source to dest.
 */
void memory_copy(char *source, char *dest, int nbytes)
{
	int i;
	for (i=0; i<nbytes; i++)
		*(dest+i) = *(source+i);
}

/*
 * memory_set sets len bytes of dest with val.
 */
void memory_set(u8 *dest, u8 val, u32 len)
{
	u8 *tmp = (u8 *)dest;

	for ( ; len != 0; len--)
		*temp++ = val;
}


/*
 * int_to_ascii
 */
void int_to_ascii(int n, char str[])
{
	int i, sign;
	if ((sign = n) < 0)
		n = -n;

	i = 0;
	do
	{
		str[i++] = n % 10 + '0';
	} while((n /= 10) > 0);

	if (sign < 0)
		str[i++] = '-';
	str[i] = '\0';
}
