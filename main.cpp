#include "mbed.h"
#include "TextLCD.h"
#include "Keypad.h"
#include "ultrasonic.h"

TextLCD my_lcd(PTE20, PTE21, PTE22, PTE23, PTE29, PTE30, TextLCD::LCD16x2);
Keypad kpad(PTA12, PTD4, PTA2, PTA1, PTC9, PTC8, PTA5, PTA4);
 
 void dist(int distance){
}
 
ultrasonic mu(PTA13, PTD5, .1, 1, &dist);    //Set the trigger pin to PTA13 and the echo pin to PTD5
                                        //have updates every .1 seconds and a timeout after 1
                                        //second, and call dist when the distance changes
float dispDist;
float prevDist;
float interDist;
float input;

PwmOut ledRed(LED_RED);
PwmOut ledGreen(LED_GREEN);
PwmOut ledBlue(LED_BLUE);

int main()
{
    my_lcd.printf("Input distance (cm) then press #:");
    wait(2);
    my_lcd.cls();
    char key;
    int released = 1;
    char inputArray[16];
    int i = 0 ;
    while(i<16){
        key = kpad.ReadKey();                   //read the current key pressed

        if(key == '\0')
            released = 1;                       //set the flag when all keys are released
           
        if((key != '\0') && (released == 1)) {  //if a key is pressed AND previous key was released
            
            if (key == '#'){
                my_lcd.printf("=");
                break;
            }
            else{
                my_lcd.printf("%c", key);
            }
            
            inputArray[i] = key; 
            i++;         
            released = 0;                       //clear the flag to indicate that key is still pressed
        }
    }
    sscanf(inputArray, "%f", &input);
    my_lcd.locate(0, 1);
    my_lcd.printf("Input: %.1f", input);
    wait(1);
    my_lcd.cls();
    
    mu.startUpdates();//start mesuring the distance
    while(1)
    {        
        dispDist = mu.getCurrentDistance();
        dispDist = dispDist / 10;
        interDist = ((2 * input) / 3 - input / 3) / 5;
        if (dispDist > input){
            PwmOut extRedLed(PTD2);
            PwmOut extGreenLed(PTD0);
            extRedLed = 0;
            extGreenLed = 0;
            wait(0.01);
        }
        
        else if ((dispDist < input && dispDist > 2 * input / 3)){
            PwmOut extRedLed(PTD2);
            PwmOut extGreenLed(PTD0);;
            extRedLed = 0;
            extGreenLed = 1;
            wait(0.01);
        }
        
        else if (dispDist >= input / 3 & dispDist <= (input / 3) + interDist){
            //Do something
            PwmOut extRedLed(PTD2);
            PwmOut extGreenLed(PTD0);
            extRedLed = 0.95;
            extGreenLed = 0;
            wait(0.01);
        }
        else if (dispDist >= (input / 3) + interDist &  dispDist <=  (input / 3) + 2 * interDist){
            //Do something
            PwmOut extRedLed(PTD2);
            PwmOut extGreenLed(PTD0);
            extRedLed = 0.75;
            extGreenLed = 0;
            wait(0.01);
        }
        else if (dispDist >= (input / 3) + 2 * interDist &  dispDist <=  (input / 3) + 3 * interDist){
            //Do something
            PwmOut extRedLed(PTD2);
            PwmOut extGreenLed(PTD0);
            extRedLed = 0.55;
            extGreenLed = 0;
            wait(0.01);
        }
        else if (dispDist >= (input / 3) + 3 * interDist &  dispDist <=  (input / 3) + 4 * interDist){
            //Do something
            PwmOut extRedLed(PTD2);
            PwmOut extGreenLed(PTD0);
            extRedLed = 0.35;
            extGreenLed = 0;
            wait(0.01);
        }
        else if (dispDist >= (input / 3) + 4 * interDist &  dispDist <=  (input / 3) + 5 * interDist){
            //Do something
            PwmOut extRedLed(PTD2);
            PwmOut extGreenLed(PTD0);
            extRedLed = 0.15;
            extGreenLed = 0;
            wait(0.01);
        }
        else if (dispDist < (input / 3)){
                PwmOut extRedLed(PTD2);
                PwmOut extGreenLed(PTD0);
                extRedLed = 0.95;
                extGreenLed = 0;
                wait(0.01);
            }
        
        if (prevDist > dispDist){
            ledRed = 0;
            ledGreen = 0;
            ledBlue = 1;
            wait(0.01);
        }
        
        else if (prevDist < dispDist){
            ledRed = 1;
            ledGreen = 1;
            ledBlue = 0;            
            wait(0.01);
        }
        
        else{
            ledRed.pulsewidth(1);
            ledGreen.pulsewidth(1);
            ledBlue.pulsewidth(1); 
            wait(0.01);
        }
        
        prevDist = dispDist;
        my_lcd.printf("%.1f", dispDist); 
        my_lcd.locate(6, 0);
        my_lcd.printf("cm");
        wait(0.5);
        my_lcd.cls();
        wait(0.01);
    }
}