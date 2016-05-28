typedef enum {PORTA,PORTB,PORTC}GPIOPort;
typedef enum {GPIO_Pin0=0x01,GPIO_Pin1=0x02,GPIO_Pin2=0x04,GPIO_Pin3=0x08,GPIO_Pin4=0x10,GPIO_Pin5=0x20,GPIO_Pin6=0x40,GPIO_Pin7=0x80}GPIOPins;
typedef enum {OUT,IN}GPIOMode;
typedef enum {NOPU,PULLUP}GPIOPU;


typedef struct {
	unsigned char GPIO_Pin;
	GPIOMode	GPIO_Mode;
	GPIOPU		GPIO_PU;
}GPIO_InitTypeDef;





void GPIO_Init(unsigned char Port, GPIO_InitTypeDef* GPIO_Init_Struct)
{
	unsigned char *tris;
	unsigned char mask;
	
	if (Port == PORTA) {
		tris = 0xf92;	
	}else if (Port == PORTB) {
		tris = 0xf93;
	}else {	// PortC
		tris = 0xf94;
	}
 	mask = GPIO_Init_Struct->GPIO_Pin;

	if (GPIO_Init_Struct->GPIO_Mode == IN) {
		*tris |= mask;
	}else {
		*tris &= ~mask;
	}
}


void GPIO_DeInit(GPIOPort Port)
{
	unsigned char	*latchAddr;
	unsigned char 	*tris;

	if (Port == PORTA) {
		latchAddr = 0xf89;
		tris = 0xf92;	
	}else if (Port == PORTB) {
		latchAddr = 0xf8a;
		tris = 0xf93;
	}else {	// PortC
		latchAddr = 0xf8b;
		tris = 0xf94;
	}
	*latchAddr= 0;
	*tris = 0xff;
}



void GPIO_Write(GPIOPort Port, unsigned char portVal)
{
	unsigned char *portAddr;

	if (Port == PORTA) {
		portAddr = 0xf80;	
	}else if (Port == PORTB) {
		portAddr = 0xf81;
	}else {	// PortC
		portAddr = 0xf82;
	}
	*portAddr = portVal;
}

void GPIO_WriteBit(GPIOPort Port, unsigned char GPIO_pin, char bitVal)
{
	unsigned char	latchData;
	unsigned char	*latchAddr;
	unsigned char 	*portAddr;

	if (Port == PORTA) {
		portAddr = 0xf80;	
		latchAddr = 0xf89;
	}else if (Port == PORTB) {
		portAddr = 0xf81;
		latchAddr = 0xf8a;
	}else {	// PortC
		portAddr = 0xf82;
		latchAddr = 0xf8b;
	}
	latchData = *latchAddr;		// Read current Out data on the latch
	if (bitVal) {
		latchData |= GPIO_pin;
		*portAddr = latchData; 
	}else {
		latchData &= ~GPIO_pin;
		*portAddr = latchData;
	}	
}


unsigned char GPIO_ReadData(GPIOPort Port)
{
	unsigned char *portAddr;
	unsigned char retData;

	if (Port == PORTA) {
		portAddr = 0xf80;	
	}else if (Port == PORTB) {
		portAddr = 0xf81;
	}else {	// PortC
		portAddr = 0xf82;
	}
	retData = *portAddr;
	return retData;
}


unsigned char GPIO_ReadInputData(GPIOPort Port)
{
	unsigned char *tris;
	unsigned char retData, mask;

	if (Port == PORTA) {
		tris = 0xf92;	
	}else if (Port == PORTB) {
		tris = 0xf93;
	}else {				// PortC
		tris = 0xf94;
	}

	retData =  GPIO_ReadData(Port);
	mask = *tris;
	retData &= mask;

	return retData;
}

char GPIO_ReadInputDataBit(GPIOPort Port, unsigned char GPIO_pin)
{
	unsigned char retData;

	retData = GPIO_ReadData(Port);
	retData &= GPIO_pin;	// Test Selected Bit

	if (retData) return 1;
	else return 0;
}


unsigned char GPIO_ReadLatch(GPIOPort Port)
{
	unsigned char *latchAddr;
	unsigned char retData;

	if (Port == PORTA) {
		latchAddr = 0xf89;
	}else if (Port == PORTB) {
		latchAddr = 0xf8a;
	}else {	// PortC
		latchAddr = 0xf8b;
	}

	retData = *latchAddr;
	return retData;
}


unsigned char GPIO_ReadOutputData(GPIOPort Port)
{
	unsigned char *tris;
	unsigned char retData, mask;

	if (Port == PORTA) {
		tris = 0xf92;	
	}else if (Port == PORTB) {
		tris = 0xf93;
	}else {				// PortC
		tris = 0xf94;
	}

	retData =  GPIO_ReadLatch(Port);
	mask = *tris;
	retData &= (~mask);

	return retData;
}

char GPIO_ReadOutputDataBit(GPIOPort Port, unsigned char GPIO_pin)
{
	unsigned char retData;

	retData = GPIO_ReadLatch(Port);
	retData &= GPIO_pin;	// Test Selected Bit

	if (retData) return 1;
	else return 0;
}


void GPIO_ResetBits(GPIOPort Port, unsigned char GPIO_pin)
{
	GPIO_WriteBit(Port, GPIO_pin, 0);
}


void GPIO_SetBits(GPIOPort Port, unsigned char GPIO_pin)
{
	GPIO_WriteBit(Port, GPIO_pin, 1);
}

	