/* OBD PID Scan Test */

#define OBD_REQ_CAN_ID    0x7DF
#define OBD_RSP_CAN_ID    0x7E8

void main (void)
{
    CAN_FRAME can, rcan;
    UINT8     *cmd = can.data;
    UINT8     pid;

    printf ("Set Rcv CAN ID %04X\n", OBD_RSP_CAN_ID);
    DNK_SetCanID (0, OBD_RSP_CAN_ID);

    DNK_ClearRcvCanData ();
    cmd[0] = 0x02;
    cmd[1] = 0x01;
    cmd[3] = 0x55;
    cmd[4] = 0x55;
    cmd[5] = 0x55;
    cmd[6] = 0x55;
    cmd[7] = 0x55;

    can.canId = OBD_REQ_CAN_ID;
    can.dlc = 8;

    for (pid = 0; pid <= 64; pid++)
    {
        printf ("Query PID %d  --> ", pid);
        can.data[2] = pid;
        DNK_SendCanData (&can);

        sleep (50);

        if (DNK_GetTotalRcvCanDataCount() > 0)
        {
            DNK_GetRcvCanData (&rcan);
            if (rcan.canId == OBD_RSP_CAN_ID)
            {
                cmd = rcan.data;
                printf ("Rsp : %02X %02X %02X %02X %02X %02X %02X %02X\n",
                    cmd[0], cmd[1], cmd[2], cmd[3], cmd[4], cmd[5], cmd[6], cmd[7]);
            }
        }
        else
        {
            putchar ('\n');
        }
    }
}
                
