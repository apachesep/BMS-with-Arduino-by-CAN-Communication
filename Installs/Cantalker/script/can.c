/* 
    ---- CANTalker C Interpreter ----
         Function Test Example 
*/

void main (int argc, char **argv)
{
    CAN_FRAME   can;
    CAN_STATUS  sts;
    DEVICE_INFO dev;
    UINT32      i, no;
    UINT8       txErrCnt, rxErrCnt;
    char        buff[16];

    printf ("can init\n");
    DNK_SetCanInit ();

    can.timeStamp = rand ();
    can.canId = rand ();
    can.dlc = rand () % 9;
    for (i = 0; i < can.dlc; i++)
    {
        can.data[i] = rand();
    }    
    printf ("can send message: id=%d\n", can.canId);
    DNK_SendCanData (&can);
    DNK_AddLog (0, &can);

    no = DNK_GetTotalRcvCanDataCount ();    
    printf  ("total rx can message count = %d\n", no);
    for (i = 0; i < no; i++)
    {
        DNK_GetRcvCanData (&can);
        DNK_AddLog (1, &can);
    }

    printf ("wait 1 sec\n");
    sleep (1000);
    
    printf ("clear can message\n");
    DNK_ClearRcvCanData ();

    printf ("clear can error\n");
    DNK_SetClearCanError ();

    DNK_ReqCanStatus (&sts);
    printf ("CAN Status=%02X, rxcanId=%X\n", sts.status, sts.canId);

    DNK_ReqErrorCount (&txErrCnt, &rxErrCnt);

    printf ("CAN Error Count: tx=%d, rx=%d\n", txErrCnt, rxErrCnt);

    printf ("Set CAN ID 2.0A 0xFFFFFFFF (all accept)\n");
    DNK_SetCanID (0, 0xFFFFFFFF);

    DNK_ReqCurTimeStamp (&no);
    printf ("Current Timestamp = %d\n", no);
    
    //int DNK_SetCanBaudRate (UINT32 brate);

    memset (buff, 0x00, sizeof (buff));
    DNK_ReqDeviceInfo (&dev);
    strncpy (buff, dev.modelName, 8);
    printf ("ModelName=%s\n", buff);
    strncpy (buff, dev.serialNo, 8);
    printf ("SeriaNo=%s\n", buff);
    
    DNK_SetRcvMode (1);
    printf ("Set TimeStamp Rcv Mode\n");
    
    DNK_ReqRcvMode (&i);
    printf ("Cur TimeStamp Rcv Mode=%d\n", i);
    
    //DNK_ReqCanFilters (FILTER_INFO *filterInfo);
    
    DNK_ReqVersion (&txErrCnt, &rxErrCnt);
    printf ("version=%d.%d\n", txErrCnt, rxErrCnt);
    
    //DNK_ReqSpeedInfo (UINT32 *canSpeed, UINT8 *rs232SpeedIdx);
    //DNK_ReqTcpIpInfo (TCPIP_INFO *pInfo);
    //DNK_SetCanBitTiming (BRGCON *brgcon);
}
