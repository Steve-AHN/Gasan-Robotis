#include <18LF14K50.h>
#device ADC=10

#FUSES WDT                   	//Watch Dog Timer
#FUSES WDT32                 	//Watch Dog Timer uses 1:32 Postscale
#FUSES CPUDIV1					// No Clock Devide 12Mhz
#FUSES USBDIV1					// USB Clock Source No Devide
#FUSES HS						// External HighSpeed Crystal
#FUSES PLLEN					// 4 x PLL Enable
#FUSES PCLKEN					// USE Primary Clock
#FUSES PUT						// Power Up timer Enable
#FUSES BROWNOUT					// Enable Brown Out
#FUSES BORV27					// Brown out volt = 2.7V
#FUSES NOMCLR					// No MCLR, we use PA3
#FUSES PROTECT					// Code Protect

#use delay(clock=48MHz,crystal=12MHz,USB_FULL,restart_wdt)

