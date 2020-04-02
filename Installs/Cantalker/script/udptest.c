void main (void)
{    
    char buff[128];
    int  size, count = 0;
    int  pt, ct;

    UDP_Open ("127.0.0.1", 21, 22);

    pt = timems ();
    while (1)
    {
        size = sizeof (buff) - 1;
        if (UDP_GetRcvDataSize ())
        {
            int i;
            size = UDP_GetData (buff, size);
            buff[size] = 0;
            printf ("s=%d, %s\n", size, buff);
            
        }
        ct = timems ();
        if (ct - pt >= 1000)
        {
            pt = ct;
            size = sprintf (buff, "count=%d\n", count++);
	    UDP_SendData (buff, size);
        }
    }


    UDP_Close ();
}