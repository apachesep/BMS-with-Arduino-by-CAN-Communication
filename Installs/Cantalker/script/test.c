void main (void)
{
	char buff[128], *ptr;
	printf ("input=? ");
	ptr = gets (buff);

	printf ("inp=%s, %p, %p\n", buff, buff, ptr);
}