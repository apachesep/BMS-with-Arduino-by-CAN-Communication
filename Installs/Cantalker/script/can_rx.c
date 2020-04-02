void main (void)
{
	CAN_FRAME can;
	int no, total = 0;
	UINT32 pt, ct;
	
	pt = timems ();
	while (1)
	{
		no = DNK_GetTotalRcvCanDataCount ();
		while (no)
		{
			DNK_GetRcvCanData (&can);
			if (can.canId & 0x400)
			{
				DNK_AddLog (1, &can);
				total++;
			}
			no--;
		}
		ct = timems ();
		if (ct - pt >= 1000)
		{
			printf ("total=%8d\r", total);
			pt = ct;
		}
	}
}