void main (void)
{
    int  bCon = TCP_Open ("127.0.0.1", 21);
    char buff[128];
    int  size, count = 0;
    int  pt, ct;

    if (!bCon) 
    {
        printf ("TCP connection failed\n");
        return;
    }

    pt = timems ();
    while (1)
    {
        size = sizeof (buff) - 1;
        if (TCP_GetRcvDataSize ())
        {
            int i;
            size = TCP_GetData (buff, size);
            buff[size] = 0;
            printf ("s=%d, %s\n", size, buff);
            
        }
        ct = timems ();
        if (ct - pt >= 1000)
        {
            pt = ct;
            size = sprintf (buff, "count=%d\n", count++);
	    TCP_SendData (buff, size);
        }
    }


    TCP_Close ();
}