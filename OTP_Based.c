#include <LPC21xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uart0.h"
#include "delay.h"
#include "rtc.h"
#include "keypad.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "I2C_EEPROM.h"
#include "I2C.h"
#include "types.h"
#include "pin_cfg.h"
#include "pin_cfg_defines.h"
#include "defines.h"


#define EN1 21
#define PIN1 22
#define PIN2 23 
#define Buzzer 24
#define Time 1

#define EINT3_PIN      20 //select the one port pin which will give support for EINT3
#define EINT3_VIC_CHNO 17	  //assign the EINT3 channel number
#define EINT3_LED      25  	//toggle one LED for every interrupt, so select one port pin


unsigned char pwd[5]="1234",r_pwd[5];

char str[5];
char i =0;
u32 Hr,Mn,Sec,start_sec,start_min;
u32 Key,Key1,Key2,Key3,Key4,Key5,Key6;
u8 timeout;
u8 elapsed;
u32 old_sec;
u32 otp_val;
void otp(u32 num)
{
        if(num==0)
        {
                //display 0 on lcd
                str[i++] = '0';
        }
        else
        {
                //extract digit by digit and store it into array
                while(num)
                {
                        str[i++] = (num%10)+48;
                        num = num/10;
                }
                //display digit by digit on LCD
        }
}

void GSM_Send_SMS(char *number,char *msg)
{
   UART0_TxString("AT\r\n");
   delay_ms(1000);
	UART0_TxString("AT+CMGF=1\r\n");
   delay_ms(1000);
   UART0_TxString("AT+CMGS=");
   UART0_TxChar('"');
   UART0_TxString(number);
   UART0_TxChar('"');
   UART0_TxString("\r\n");
   delay_ms(1000);
   //UART0_TxString("Do Not Share this OTP with anyone");
   UART0_TxString(msg);
   UART0_TxChar(0x1A);// Ctrl+z to send sms
   delay_ms(5000);
}

//ISR SYNTAX: void ISR_NAME(void) __irq
void eint0_isr(void) __irq
{
	//toggle EINT_LED
	CPLBIT(IOPIN0,EINT3_LED);	
	CmdLCD(CLEAR_LCD);
	StrLCD("Current Password:");
	Key5=ReadNum();
	if(Key5==Key)
	{
	//	CmdLCD(CLEAR_LCD);
	  //StrLCD("Confirm Password:");
	  //Key6=ReadNum();
	  //if(Key6==Key5)
	  //{
			 CmdLCD(CLEAR_LCD);
	     StrLCD("SET new PASSWORD- ");
       Key4=ReadNum();
		   CmdLCD(CLEAR_LCD);
	     StrLCD("Confirm PASSWORD- ");
	     Key6=ReadNum();
		   if(Key6==Key4)
			 {
	     pwd[0]=((Key4%10000)/1000)+48;
	     pwd[1]=((Key4%1000)/100)+48;
	     pwd[2]=((Key4%100)/10)+48;
	     pwd[3]=(Key4%10)+48;
	
	     i2c_eeprom_page_write(0x50,0x0000,pwd,4);
       i2c_eeprom_seq_read(0x50,0x0000,r_pwd,4);
	
	     Key3=atoi((const char *)r_pwd);
	//clear eint0 status (through EXTINT SFR)
	     EXTINT = 1<<0;
	//clear eint0 status in VIC (through VICVectAddr )
	     VICVectAddr = 0;
		 }
	 }
}	

void Enable_EINT0(void)
{
	//configure P0.1/P0.16 as EINT0 input pin
	CfgPortPin(PORT0,EINT3_PIN,PIN_FUNC4);
	
	//configure VIC (Vector Interrupt controller)
	//def External interrupts (EINT0/EINT1/EINT2/EINT3) is IRQ types (by using VICIntSelect SFR)
	
	//enable EINT0 via VIC (by using VICIntEnable SFR)
	VICIntEnable = 1<<EINT3_VIC_CHNO;
	//enable vectored irq slot 0 for EINT0 (0-15 based on requirement – by using VICVectCntl0 SFR)
	VICVectCntl0 = (1<<5) | (EINT3_VIC_CHNO);
	//load isr address into slot 0 (0-15 based on requirement – by using VICVectAddr0 SFR)
	VICVectAddr0 = (u32)eint0_isr;
	
	//configure External Interrupts Peripheral
	//configure EINT0/EINT1/EINT2/EINT3 as edge triggered (use EXTMODE SFR)
   EXTMODE = 1<<0;
	//def EINT0/EINT1/EINT2/EINT3 is Falling Edge Triggerd (use EXTPOLAR SFR)	
}


int main() {
	
	
	  u8 min_st,sec_st,min_en,sec_en;
    InitLCD();
    Keypad_Init();
    RTC_Init();
   // RTCSetTime(5, 5, 5);         // Set time initially
    init_i2c();                  // For EEPROM
    Enable_EINT0();              // For password reset ISR

    // Configure output pins
    IODIR0 |= (1 << EN1) | (1 << PIN1) | (1 << PIN2) | (1 << Buzzer) | (1 << EINT3_LED);
    IOSET0 |= 1 << EN1;

    // Load password from EEPROM
    i2c_eeprom_seq_read(0x50, 0x0000, r_pwd, 4);
    Key3 = atoi((const char *)r_pwd);  // stored password

    while (1) {
        // Welcome screen
        CmdLCD(CLEAR_LCD);
        CmdLCD(0x0C);
        CmdLCD(0x80);
        StrLCD("OTP Based Limited");
        CmdLCD(0xC0);
        StrLCD("Access System");
        delay_ms(2000);

        // Password input
    password_entry:
        CmdLCD(CLEAR_LCD);
        StrLCD("Enter Password:");
        Key = ReadNum();
        delay_ms(500);

        if (Key == Key3) {
            CmdLCD(CLEAR_LCD);
            StrLCD("Access Granted");
            delay_ms(1000);

            // Generate OTP using RTC
           //RTCGetTime(&Hr, &Mn, &Sec);
            //memset(str, 0, sizeof(str));
            // = 0;
            //otp(HOUR);
            //otp(MIN);
            //otp(SEC);
            //i = 0;
					  srand(SEC);
            otp(1000+rand()%9000);

            // Send OTP via GSM
            CmdLCD(CLEAR_LCD);
            StrLCD("Sending OTP...");
            GSM_Send_SMS("9516253166", str); // use your number
            delay_ms(1000);

           // OTP validation
              otp_val = atoi(str);  // correct OTP
           timeout = Time;        // seconds
            elapsed = 0;
            old_sec;
            RTCGetTime(&Hr, &Mn, &Sec);
            old_sec = Sec;
            //min_st=MIN;
            //sec_st=SEC;
            CmdLCD(CLEAR_LCD);
            StrLCD("Enter OTP:");
            U32LCD(otp_val);
						//Key2=ReadNum();

       otp_validation:
            while (1) {
                RTCGetTime(&Hr, &Mn, &Sec);
                if (Sec != old_sec) {
                    old_sec = Sec;
                    elapsed++;

                    CmdLCD(0xC0);
                    StrLCD("Time Left: ");
                    U32LCD(timeout - elapsed);

                    if (elapsed >= timeout) {
                        CmdLCD(CLEAR_LCD);
                        StrLCD("OTP Expired");
                        delay_ms(1500);
                        goto password_entry;
                    }
                }

                Key2= ReadNum(); // blocking read

                if (Key2 == otp_val) {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Access Granted");
                    IOSET0 |= 1 << PIN2;
                    IOCLR0 |= 1 << PIN1;
                    delay_ms(2000);
                    IOSET0 |= 1 << PIN1;
                    IOSET0 |= 1 << PIN2;
									//  SETBIT(IOPIN0,PIN2);
                   // CLRBIT(IOPIN0,PIN1);
                    break;
                } else {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Wrong OTP");
                    IOSET0 |= 1 << Buzzer;
                    delay_ms(1000);
                    IOCLR0 |= 1 << Buzzer;
                    goto otp_validation;
                }
            }
        } else {
            CmdLCD(CLEAR_LCD);
            StrLCD("Wrong Password");
            delay_ms(1500);
            goto password_entry;
        }
    }
}
