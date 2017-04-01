




/*
 * Author: Seytonic modyfied by roblad
 *         https://twitter.com/seytonic
 *         https://www.youtube.com/seytonic
 * GIT:
 *         https://github.com/roblad/Duckduino-microSD
 *         https://github.com/basic4/USB-Rubber-Ducky-Clone-using-Arduino-Leonardo-Beetle/blob/master/Clone_Instructions.pdf
 *         
 *         v1.01 Added led signals and optimise soft
 *
 *       Long lines of strings may crash the arduino due to taking up too much RAM, if you have a line "STRING ..." over 300 characters 
 *       then split it into separate lines of strings, this won't affect how your script runs, it just reduces how much of your script 
 *       is held in memory at any one time.
 */


#include "HID-Project.h"
#include <SPI.h>
#include <SD.h>
#include <string.h>
//#include <Keyboard.h>

//define pins according board 
#define one  A2
#define two A1
#define thre A0
#define four 9
#define CS 10

#include <avr/wdt.h>
#define Reset_AVR() wdt_enable(WDTO_1S); while(1) {}
//void(* resetFunc) (void) = 0;
  File myFile;
  boolean first = true;

void setup() { 

 MCUSR = 0;
 
/* Write logical one to WDCE and WDE */
/* Keep old prescaler setting to prevent unintentional time-out */
 WDTCSR |= _BV(WDCE) | _BV(WDE);
 WDTCSR = 0;

 

  pinMode(LED_BUILTIN, OUTPUT);
  String dip = ""; // Name of the file that will be opened

  // Sets the given pins as switches for the dip switches
  digitalRead(one);
  digitalRead(two);
  digitalRead(thre);
  
  pinMode(one, INPUT_PULLUP);
  pinMode(two, INPUT_PULLUP);
  pinMode(thre, INPUT_PULLUP);
  pinMode(four, INPUT_PULLUP);
  
  // Switches are checked, dip string is contructed
  if (digitalRead(one) == LOW){dip += "1";} else {dip += "0";}
  if (digitalRead(two) == LOW){dip += "1";} else {dip += "0";}
  if (digitalRead(thre) == LOW){dip += "1";} else {dip += "0";}
  if (digitalRead(four) == LOW){dip += "1";} else {dip += "0";}

  dip += ".txt";

delay (10000); //wait for drivers

  if (!SD.begin(CS)) {
     //no card
      while(1){
        Keyboard.end();
        digitalWrite(LED_BUILTIN, HIGH);
        delay(50);
        digitalWrite(LED_BUILTIN, LOW);
        delay(50);
        
       }
     return;
  }
  
  // Desired file is opened
  
  generatehelpFile();
  
  myFile = SD.open(dip);
  if (myFile) {
    Keyboard.begin();
    
    String line = "";
    while (myFile.available()) {
      char m = myFile.read();
      if (m == '\n'){
        Line(line);
        line = "";
        }
        else if((int) m != 13)
        {
          line += m;
        }
    }
    Line(line);
    
    myFile.close();

   
    
  } else {
    //no file
          while(1){
            Keyboard.end();
            digitalWrite(13,HIGH);
            delay(100);
            digitalWrite(13,LOW);
            delay(50);
          } 
  }

       for (byte i = 0; i < 6;i++){ 
        digitalWrite(LED_BUILTIN, HIGH);

        delay(500);
        digitalWrite(LED_BUILTIN, LOW);

        delay(500);
        
     }
     

  Keyboard.end();


  
}


//create help on SDcard
void generatehelpFile() {
      

  
  if (! SD.exists("readme.txt")) { 
   
   
   
   File myFile0 = SD.open("readme.txt", FILE_WRITE);

    delay (50);
   
    if (myFile0) {

      myFile0.println(F(""));
      delay (50);
      myFile0.println(F("There is bad usb help file, created when first time SD card is inserted to bad USB (do wlamywaczki)"));
      delay (50);
      myFile0.println(F(""));
      delay (50);
      myFile0.println(F("Wlamywaczka by RL check link for scripting: https://github.com/hak5darren/USB-Rubber-Ducky"));
      delay (50);
      myFile0.println(F("Check also my GIT source for additional info: https://github.com/roblad/Duckduino-microSD"));
      delay (50);
      myFile0.println(F("There is needed script files creation in SD card according to four DIP switches sequence set"));
      delay (50);
      myFile0.println(F("Because there are 4 DIP (or 5 - five is for repeating execution of script) you have to create files:"));
      delay (50);
      myFile0.println(F("4 DIPs = 2^4 gives 16 files - 0000.txt, 0001.txt, 0011.txt till 1111.txt"));
      delay (50);
      myFile0.println(F("All nescesary info you will find in GITs i.e. scripting language"));
      delay (50);
      myFile0.println(F("I added additional script commands according HID-Project.h see additional info in my GIT for more detailes."));
      delay (50);
      myFile0.println(F(""));
      delay (50);
      myFile0.println(F("Do not use for bad reason :-) and ENJOY !!!"));
      delay (50);
      myFile0.println(F(""));
      delay (50);
      myFile0.println(F("Copy rights R.L."));
      delay (50);
    // close the file:
    myFile0.close();
    delay (200);
    }
  } 
}

//function for scripts


void Line(String l)
{
  int space_1 = l.indexOf(" ");
  if (space_1 == -1)
  {
    Press(l);
  }
  else if (l.substring(0,space_1) == "STRING")
  {
    Keyboard.print(l.substring(space_1 + 1));
  }
  else if (l.substring(0,space_1) == "DELAY")
  {
    int delaytime = l.substring(space_1 + 1).toInt();
    delay(delaytime);
  }
  else if(l.substring(0,space_1) == "REM"){}
  else
  {
      String remain = l;

      while(remain.length() > 0)
      {
        int latest_space = remain.indexOf(" ");
        if (latest_space == -1)
        {
          Press(remain);
          remain = "";
        }
        else
        {
          Press(remain.substring(0, latest_space));
          remain = remain.substring(latest_space + 1);
        }
        delay(5);
      }
  }

  Keyboard.releaseAll();
}


void Press(String b)
{
  if(b.length() == 1)
  {
    char c = b[0];
    Keyboard.press(c);
  }
  else if (b.equals("ENTER"))
  {
    Keyboard.press(KEY_RETURN);
  }
  else if (b.equals("CTRL"))
  {
    Keyboard.press(KEY_LEFT_CTRL);
  }
    else if (b.equals("SHIFT"))
  {
    Keyboard.press(KEY_LEFT_SHIFT);
  }
    else if (b.equals("ALT"))
  {
    Keyboard.press(KEY_LEFT_ALT);
  }
    else if (b.equals("GUI"))
  {
    Keyboard.press(KEY_LEFT_GUI);
  }
    else if (b.equals("UP") || b.equals("UPARROW"))
  {
    Keyboard.press(KEY_UP_ARROW);
  }
    else if (b.equals("DOWN") || b.equals("DOWNARROW"))
  {
    Keyboard.press(KEY_DOWN_ARROW);
  }
    else if (b.equals("LEFT") || b.equals("LEFTARROW"))
  {
    Keyboard.press(KEY_LEFT_ARROW);
  }
    else if (b.equals("RIGHT") || b.equals("RIGHTARROW"))
  {
    Keyboard.press(KEY_RIGHT_ARROW);
  }
    else if (b.equals("DELETE"))
  {
    Keyboard.press(KEY_DELETE);
  }
    else if (b.equals("PAGEUP"))
  {
    Keyboard.press(KEY_PAGE_UP);
  }
    else if (b.equals("PAGEDOWN"))
  {
    Keyboard.press(KEY_PAGE_DOWN);
  }
    else if (b.equals("HOME"))
  {
    Keyboard.press(KEY_HOME);
  }
    else if (b.equals("ESC"))
  {
    Keyboard.press(KEY_ESC);
  }
    else if (b.equals("INSERT"))
  {
    Keyboard.press(KEY_INSERT);
  }
    else if (b.equals("TAB"))
  {
    Keyboard.press(KEY_TAB);
  }
    else if (b.equals("END"))
  {
    Keyboard.press(KEY_END);
  }
    else if (b.equals("CAPSLOCK"))
  {
    Keyboard.press(KEY_CAPS_LOCK);
  }
    else if (b.equals("F1"))
  {
    Keyboard.press(KEY_F1);
  }
    else if (b.equals("F2"))
  {
    Keyboard.press(KEY_F2);
  }
    else if (b.equals("F3"))
  {
    Keyboard.press(KEY_F3);
  }
    else if (b.equals("F4"))
  {
    Keyboard.press(KEY_F4);
  }
    else if (b.equals("F5"))
  {
    Keyboard.press(KEY_F5);
  }
    else if (b.equals("F6"))
  {
    Keyboard.press(KEY_F6);
  }
    else if (b.equals("F7"))
  {
    Keyboard.press(KEY_F7);
  }
    else if (b.equals("F8"))
  {
    Keyboard.press(KEY_F8);
  }
    else if (b.equals("F9"))
  {
    Keyboard.press(KEY_F9);
  }
    else if (b.equals("F10"))
  {
    Keyboard.press(KEY_F10);
  }
    else if (b.equals("F11"))
  {
    Keyboard.press(KEY_F11);
  }
    else if (b.equals("F12"))
  {
    Keyboard.press(KEY_F12);
  } 
    else if (b.equals("SYSSLEEP"))
  {
    System.write(SYSTEM_SLEEP);
  } 
    else if (b.equals("SYSWAKEUP"))
  {
    System.write(SYSTEM_WAKE_UP);
  } 
    else if (b.equals("SYSPOWER"))
  {
    System.write(SYSTEM_POWER_DOWN);
  } 
   else
  {
   Keyboard.press(NULL);
  }
  
   
}

void loop() {
  // nothing happens after setup
  

  delay (10000);
          digitalWrite(13,LOW);
          delay(250);
          digitalWrite(13,HIGH);
          delay(250);
          digitalWrite(13,LOW);
  delay (10000);
   //Reset for some reason
 //Reset_AVR();
}

//again setup function
