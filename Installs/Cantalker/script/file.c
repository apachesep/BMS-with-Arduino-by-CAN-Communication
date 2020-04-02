void main (void)
{
    FILE *fp;
    fp = fopen ("test.txt", "w");
    if (fp == NULL)
    {
        printf ("file open error\n");
        return;
    }
    fprintf (fp, "file write test\n");
    fclose (fp);
}