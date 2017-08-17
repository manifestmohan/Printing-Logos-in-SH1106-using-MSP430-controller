/*code to make OLED I2c display (SH1106 or SSD1306 based display) work with any TI controller (preferably MSP430) without the use of a display booster pack. Interface it with a keypad so as to display Ferrari logo when '*' is pressed and Lamborghini logo when '#' is pressed on the keypad.*/

#include <msp430g2231.h>       
#include <i2c.h>               //i2c with msp430g2231
#include <i2ckeypad.h>         //i2c with keypad through pcf8574
#include <SH1106.h>            //i2c with display
#include <ferrari_logo.h>      //image of ferrari logo
#include <Lamborghini_logo.h>  //image of lamborghini logo


#define ROWS 4                  //4x3 keypad
#define COLS 3

#define PCF8574_ADDR 0x20       //slave address of pcf8574

i2ckeypad kpd = i2ckeypad(PCF8574_ADDR, ROWS, COLS);

#define OLED_ADDR   0x3C        //Slave address of SH1106

char key;                       //holds the value of key pressed

SH1106   display(OLED_ADDR);

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;   //stop watchdogtimer
	
     i2c_init();                 //i2c initialisation
     kpd.init();                 //keypad initialisation
     display.init();             //display initialisation
     
     while(1)
     {
         if(key=='*')
        {
         display.resetDisplay();  //reset display

        //draw ferrari logo

        display->drawBitmap(1,1,64,128,ferrari_logo);

	   }

         while(key=='*')
          {
             key = kpd.get_key();  //loop until value change;
          }

         if(key=='#')
        {
        
          display.resetDisplay();  //reset display

        //draw lamborghini logo

        display->drawBitmap(1,1,64,128,Lamborghini_logo);

	   }

         while(key=='#')
          {
             key = kpd.get_key();  //loop until value change;
          }

      }
 }
