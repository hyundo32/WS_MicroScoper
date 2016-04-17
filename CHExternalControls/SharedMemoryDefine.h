﻿/*
#define PLC_CONNECTOR_ALIVE	0

#define PLC_WRITEWORD_TIMER_COUNT	10
#define PLC_WRITEWORD_TIMER_SIZE	24
#define PLC_WRITEWORD_LONGDATA_COUNT	3
#define PLC_LONGDATA_MAXSIZE		2048

#define PLC_WRITEWORD_SIGNAL		0
#define PLC_WRITEWORD_ADDRESS		PLC_WRITEWORD_SIGNAL+1
#define PLC_WRITEWORD_VALUE			PLC_WRITEWORD_ADDRESS+14
#define PLC_WRITEWORD_TIMER			PLC_WRITEWORD_VALUE+2

#define PLC_WRITELONG_SIGNAL		PLC_WRITEWORD_TIMER_COUNT*PLC_WRITEWORD_TIMER_SIZE+10
#define PLC_WRITELONG_ADDRESS		PLC_WRITELONG_SIGNAL+1
#define PLC_WRITELONG_WORDLENGTH	PLC_WRITELONG_ADDRESS+14
#define PLC_WRITELONG_DATA			PLC_WRITELONG_WORDLENGTH+4

#define PLC_READ_SIGNAL0			PLC_WRITELONG_SIGNAL+(PLC_WRITEWORD_LONGDATA_COUNT*PLC_LONGDATA_MAXSIZE)+10
#define PLC_READ_SIGNAL1			PLC_READ_SIGNAL0+2
#define PLC_READ_SIGNAL2			PLC_READ_SIGNAL1+2
#define PLC_READ_SIGNAL3			PLC_READ_SIGNAL2+2
#define PLC_READ_SIGNAL4			PLC_READ_SIGNAL3+2

#define PLC_READ_LONGDATA_SIGNAL	PLC_READ_SIGNAL4+10
#define PLC_READ_LONGDATA_ADDRESS	PLC_READ_LONGDATA_SIGNAL+2
#define PLC_READ_LONGDATA_LENGTH	PLC_READ_LONGDATA_ADDRESS+14
#define PLC_READ_LONGDATA_VALUE		PLC_READ_LONGDATA_LENGTH+14

#define SHARED_MEMORY_SIZE			PLC_READ_LONGDATA_VALUE+PLC_LONGDATA_MAXSIZE+10

*/



#define PLC_CONNECTOR_ALIVE	0

#define PLC_READ_ADDRESS_LENGTH			14
#define PLC_WRITEWORD_TIMER_COUNT		10
#define PLC_WRITEWORD_TIMER_SIZE		24
#define PLC_WRITEWORD_LONGDATA_COUNT	3
#define PLC_LONGDATA_MAXSIZE			1024
#define PLC_MAX_WORDSIZE				512
#define PLC_READWORD_MOTORPOS_SIZE		10

#define PLC_READ_SIGNAL0			0
#define PLC_READ_SIGNAL1			(PLC_READ_SIGNAL0+2)
#define PLC_READ_SIGNAL2			(PLC_READ_SIGNAL1+2)
#define PLC_READ_SIGNAL3			(PLC_READ_SIGNAL2+2)
#define PLC_READ_SIGNAL4			(PLC_READ_SIGNAL3+2)
#define	PLC_READ_MOTOR_POS			(PLC_READ_SIGNAL4+2)

//물류 데이터 전용
#define PLC_READ_LONGDATA_SIGNAL_1	PLC_READ_MOTOR_POS+PLC_READWORD_MOTORPOS_SIZE*2+20
#define PLC_READ_LONGDATA_ADDRESS_1	(PLC_READ_LONGDATA_SIGNAL_1+2)
#define PLC_READ_LONGDATA_LENGTH_1	(PLC_READ_LONGDATA_ADDRESS_1+14)
#define PLC_READ_LONGDATA_VALUE_1	(PLC_READ_LONGDATA_LENGTH_1+14)

//일반적인 데이터 읽기용
#define PLC_READ_LONGDATA_SIGNAL_2	(PLC_READ_LONGDATA_VALUE_1+PLC_LONGDATA_MAXSIZE+10)
#define PLC_READ_LONGDATA_ADDRESS_2	(PLC_READ_LONGDATA_SIGNAL_2+2)
#define PLC_READ_LONGDATA_LENGTH_2	(PLC_READ_LONGDATA_ADDRESS_2+14)
#define PLC_READ_LONGDATA_VALUE_2	(PLC_READ_LONGDATA_LENGTH_2+14)

//Z축제어 비동기 읽기용
#define PLC_READ_LONGDATA_SIGNAL_3	(PLC_READ_LONGDATA_VALUE_2+PLC_LONGDATA_MAXSIZE+10)
#define PLC_READ_LONGDATA_ADDRESS_3	(PLC_READ_LONGDATA_SIGNAL_3+2)
#define PLC_READ_LONGDATA_LENGTH_3	(PLC_READ_LONGDATA_ADDRESS_3+14)
#define PLC_READ_LONGDATA_VALUE_3	(PLC_READ_LONGDATA_LENGTH_3+14)

#define PLC_WRITEWORD_SIGNAL		PLC_READ_LONGDATA_VALUE_3+PLC_LONGDATA_MAXSIZE+20
#define PLC_WRITEWORD_ADDRESS		(PLC_WRITEWORD_SIGNAL+1)
#define PLC_WRITEWORD_VALUE			(PLC_WRITEWORD_ADDRESS+14)
#define PLC_WRITEWORD_TIMER			(PLC_WRITEWORD_VALUE+2)

#define PLC_WRITELONG_SIGNAL		(PLC_WRITEWORD_TIMER_COUNT*PLC_WRITEWORD_TIMER_SIZE+10)
#define PLC_WRITELONG_ADDRESS		(PLC_WRITELONG_SIGNAL+1)
#define PLC_WRITELONG_WORDLENGTH	(PLC_WRITELONG_ADDRESS+14)
#define PLC_WRITELONG_DATA			(PLC_WRITELONG_WORDLENGTH+4)

#define SHARED_MEMORY_SIZE			PLC_WRITELONG_DATA+PLC_LONGDATA_MAXSIZE+20