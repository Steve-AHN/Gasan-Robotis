#include <18LF14K50.h>
#device ADC=10

#FUSES WDT                   	//Watch Dog Timer
#FUSES WDT32                 	//Watch Dog Timer uses 1:32 Postscale

#device ICD=TRUE
#use delay(clock=48MHz,crystal=12MHz,USB_FULL,restart_wdt)

