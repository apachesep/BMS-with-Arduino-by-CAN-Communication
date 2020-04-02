/* gugudan */

void main (void)
{
    int i, j;
    CAN_FRAME can;

    for (i = 2;i < 10;i++)
    {
        printf ("----[%d]------\n", i);
        can.canId = i;
        can.dlc = 3;
        can.data[0] = i;
        for (j = 1;j < 10;j++)
        {
           printf ("%d * %d = %2d\n", i, j, i*j);
           can.data[1] = j;
           can.data[2] = i*j;
           DNK_AddLog (0, &can);
           DNK_SendCanData (&can);
        }
    }
}