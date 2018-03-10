void main()
{
	// Point to first text cell of video memory.
	char *video_memory = (char *)0xb8000;
	*video_memory = 'X';
}
