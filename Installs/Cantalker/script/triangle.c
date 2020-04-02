/* draw triangle */

void main (int argc, char **argv)
{
    int i, j, max;

    if (argc != 2)
    {
        printf ("Usage: triangle.c [number]\n");
        return;
    }
    max = atoi (argv[1]);
    for (i = 1; i<= max; i++)
    {
        for (j = 0; j < i; j++)
        {
            putchar ('*');
        }
        putchar ('\n');
    }
}