
void main (void)
{
    int  size;
    char buff[128];
    int  count = 0;
    int  pt, ct;

    if (!SER_Open (2, 115200))
    {
        printf ("serial port open error\n");
        return;
    }

    pt = timems ();

    while (count < 100)
    {        
        if (SER_GetRcvDataSize ())
        {
            size = sizeof (buff);
            size = SER_GetData (buff, size);
            buff[size] = 0;
            printf (buff);
        }
        ct = timems ();
        if (ct - pt >= 1000)
        {
            pt = ct;
            size = sprintf (buff, "count = %d\n", count++);
            SER_SendData (buff, size);
        }
    }
    SER_Close ();
}
