typedef enum {BAUD_300=39999,BAUD_1200=9999, BAUD_2400=4999, BAUD_9600=1249, BAUD_10417=1151, BAUD_19200=624, BAUD_38400 = 312, BAUD_57600=207, BAUD_115200=103}USARTBaud;
typedef enum {USART_DataLength_8b, USART_DataLength_9b}USARTDataLength;
typedef enum {USART_StopBits_1, USART_StopBits_1_5, USART_StopBits_2}USARTStopBits;
typedef enum {USART_Parity_No, USART_Parity_Even, USART_Parity_Odd}USARTParity;
typedef enum {USART_Mode_Tx=0x20, USART_Mode_Rx=0x10}USARTMode;
typedef enum {USART_IT_TXE=0x10, USART_IT_RXNE=0x20}USARTIT;
typedef enum {DISABLE, ENABLE}FunctionalState;


typedef struct {
	unsigned long	USART_BaudRate;
	char			USART_DataLength;
	char			USART_StopBits;
	char			USART_Parity;
	char			USART_Mode;
}USART_InitTypeDef;


// Since we do not have hw Parity and hw Stopbit Control
// We need to Save them as a global variables
unsigned char gUSART_StopBits=0, gUSART_Parity=0;
unsigned long gUSART_delay=0;

// Rx Buffer related Variables
#define MAX_DATA	128
unsigned char USART_Buf[MAX_DATA];
unsigned int rxHead=0, rxTail=0, rxCount=0;



void USART_Cmd(FunctionalState fs)
{
	unsigned char *rcsta;
	char cmData;
	
	rcsta = 0xfab;				// control register addr

	if (fs == ENABLE) {					
		cmData = *rcsta;
		cmData |= 0x80;
		*rcsta = cmData;		// Serial Modeule Enable, TxD/RxD pins are set by UART
	}else{
		cmData = *rcsta;
		cmData &= 0x7f;
		*rcsta = cmData;		// Serial Modeule disable, TxD/RxD pins are set by UART
	}
}




void SetBaudRate(unsigned long baudRate)
{
	unsigned char *spbrg, *spbrgh;

	spbrg = 0xfaf;
	spbrgh = 0xfb0;

	*spbrg = (baudRate & 0x00ff);
	*spbrgh = ((baudRate>>8) & 0x00ff);

	if (gUSART_StopBits != USART_StopBits_1) { // When the Stop bit is bigger that 1

		gUSART_delay = (unsigned long) gUSART_StopBits;	// delay == 1 or 2

		switch (baudRate) {
			case BAUD_300:
				gUSART_delay *= 1665;
			break;

			case BAUD_1200:
				gUSART_delay *= 415;
			break;

			case BAUD_2400:
				gUSART_delay *= 208;
			break;

			case BAUD_9600:
				gUSART_delay *= 52;
			break;

			case BAUD_10417:
				gUSART_delay *= 47;
			break;

			case BAUD_19200:
				gUSART_delay *= 26;
			break;

			case BAUD_38400:
				gUSART_delay *= 13;
			break;

			case BAUD_57600:
				gUSART_delay *= 8;
			break;

			case BAUD_115200:
				gUSART_delay *= 4;
			break;

			default:
			break;
		}
	}
}




void USART_Init(USART_InitTypeDef *USART_InitStruct)
{
	unsigned char	*txsta, *rcsta;
	char dataLength, txmode, rxmode;
	
	txsta = 0xfac;								// Tx Control Register	
	rcsta = 0xfab;								// Rx Control Register

	dataLength = USART_InitStruct->USART_DataLength;
	gUSART_Parity = USART_InitStruct->USART_Parity;
	gUSART_StopBits = USART_InitStruct->USART_StopBits;

	SetBaudRate(USART_InitStruct->USART_BaudRate);	// Set BaudRate

	txmode = USART_InitStruct->USART_Mode & 0x20;		// Check Mode_tx
	rxmode = USART_InitStruct->USART_Mode & 0x10;		// Check Mode_rx

	if (dataLength ==USART_DataLength_8b) {		// 8 data bits
		if (gUSART_Parity == USART_Parity_No) { // 8 data bits and no parity = Total 8 bits
			*txsta = 0x04 | txmode;						// 8 bit transmit
			*rcsta = 0x00 | rxmode;						// 8 bit receiver
		}else{									// 8 data bits and 1 parity = total 9 bits
			*txsta = 0x44 | txmode;						// 9 bit transmit
			*rcsta = 0x41 | rxmode;						// 9 bit receiver
		}
	}else {										// 9 data bits,  parity control by user
		*txsta = 0x44 | txmode;						// 9 bit transmit
		*rcsta = 0x41 | rxmode;						// 9 bit receiver
	}
}


void USART_ITConfig(USARTIT uIT, FunctionalState fs)
{
	unsigned char *PIE1, *INTCON;
	unsigned td;	

	PIE1= 0xfd9;
	INTCON = 0xff2;

	td = *PIE1;			// Read Current Interrupt Status
	//td &= 0xc0;			// Initialize Control Interrupt bits

	if (fs == ENABLE) {	// Enable Interrupt
		td |= uIT;
	}else {				// Disable Interrupt
		td &= (~fs);
	}
	*PIE1 = td;			// Update New Interrupt Status

	td = *INTCON;
	td |=0xc0;			// Enable Global, and all Peripheral Intrerrupt
}



unsigned char USART_ReceiveData()
{
	unsigned char *rcreg, data;

	rcreg = 0xfae;
	data = *rcreg;

	return data;
}




unsigned char	evaluateParity(unsigned char data)
{
	int i, count;
	unsigned char mask;

	mask = 0x01;
	count = 0;
	for(i=0;i<8;i++) {
		mask <<= i;
		if (data & mask) count++;
	}
	
	count %=2;			// maker it 0 or 1

	if (gUSART_Parity== USART_Parity_Even) {	// Check if it Even
		if (count) return 1;		// Result was odd 
		else return 0;				// Result waw Even
	}else {										// Odd Parity
		if (count) return 0;		// Result was odd 
		else return 1;				// Result waw Even
	}
}



void waitTxBufEmpty()
{
	unsigned char *txsta, cData;
	txsta = 0xfac;
	
	do {
		cData = *txsta & 0x02;	// Check Transmit flag
	} while (!cData);
}



void USART_SendData(unsigned char data)
{
	unsigned char *txreg, *txsta;
	unsigned char tsta, pmask;

	pmask =0;

	if 	(gUSART_Parity != USART_Parity_No) {
		pmask = evaluateParity(data);
	}

	// Parity bit set
	txsta = (0xfac & 0xfe);
	tsta = *txsta | pmask;	// update Parity bit
	*txsta = tsta;

	txreg = 0xfad;
	*txreg = data;			// Send data to Tx Buffer

	waitTxBufEmpty();		// wait until transmit finish

	if (gUSART_StopBits) delay_us(gUSART_delay);	// additional stop bit 
}



void USART_SendString(unsigned char* str)
{	
	while (*str) {			//make it sure that str is a string ends with a zero.
		USART_SendData (*str++);
	}
}



//-----------------------------------------------------------------
// Rx Related Functions
//------------------------------------------------------------------
void Push(unsigned char data)
{
	rxHead %= MAX_DATA;
	if (rxCount < MAX_DATA) {
		USART_Buf[rxHead++] = data;
		rxCount++;
	}
}

unsigned char Pop()
{
	unsigned char data=0;
	rxTail %= MAX_DATA;

	if (rxCount>0) {
		data = USART_Buf[rxTail++];
		rxCount = rxCount -1;
	}
	return data;
}

char	isDataReady()
{
	if (rxCount>0) return 1;
	else return 0;
}

void flushRxBuf()
{
	rxHead=rxTail=rxCount=0;
}

#INT_RDA
void USART_Isr()
{
	unsigned char data;

	disable_interrupts(GLOBAL);
	data = USART_ReceiveData();
	Push(data);
	enable_interrupts(GLOBAL);
}

