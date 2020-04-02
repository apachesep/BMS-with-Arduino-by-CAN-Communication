/* Engine RPM Scan Test t */

#define OBD_REQ_CAN_ID    0x7DF
#define OBD_RSP_CAN_ID    0x7E8

void main (void)
{
    CAN_FRAME can, rcan;
    UINT8     *cmd = can.data;

    DNK_SetCanID (0, OBD_RSP_CAN_ID);

    DNK_ClearRcvCanData ();

    cmd[0] = 0x02;
    cmd[1] = 0x01;
    cmd[2] = 0x0C;
    cmd[3] = 0x55;
    cmd[4] = 0x55;
    cmd[5] = 0x55;
    cmd[6] = 0x55;
    cmd[7] = 0x55;

    can.canId = OBD_REQ_CAN_ID;
    can.dlc = 8;

    while (1)
    {        
        DNK_SendCanData (&can);
        sleep (100);
        if (DNK_GetTotalRcvCanDataCount() > 0)
        {
            DNK_GetRcvCanData (&rcan);
            if (rcan.data[2] == 0x0C)
            {
                int rpm = rcan.data[3] * 256 + rcan.data[4];
                printf ("RPM:%5d\r", rpm / 4);
            }
        }
    }
}
