/*
 * Low level port IO functions.
 */


/*
 * port_byte_in is a wrapper function that reads a byte from the specified
 * port.
 * "=a" (result) means: put al register in variable result when finished.
 * "d" (port) means: load edx with port.
 */
unsigned char port_byte_in(unsigned short port)
{
	unsigned char result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

/*
 * port_byte_out is a wrappe function that writes a byte to the specified port.
 * "a" (data) means: load eax with data.
 * "d" (port) means: load edx with port.
 */
void port_byte_out(unsigned short port, unsigned char data)
{
	__asm__("out %%al, %%dx" : :"a" (data), "d" (port));
}


unsigned short port_word_in(unsigned short port)
{
	unsigned short result;
	__asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

void port_word_out(unsigned short port, unsigned short data)
{
	__asm__("out %%ax, %%dx" : :"a" (data), "d" (port));
}
