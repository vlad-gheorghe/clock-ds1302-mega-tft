//original from https://www.youtube.com/watch?v=YMZ_JP5mGJg&t=168s
//original code https://www.mediafire.com/folder/xd9bbndpd7k91/A

#include <Time.h>
#include <DS1302.h>//downloaded from http://www.henningkarlsen.com/electronics/library.php?id=5
#include <EEPROM.h>//included in default arduino compiler

#include <TFT_HX8357.h>



#include "Free_Fonts.h" 
//#include "gfxfont.h"

int spkrPin = 9;//speaker
int offPin = 8;//multi-use button 
int menuPin = 5;//button to bring up menu or select
int upPin = 6;//button to increase value
int downPin = 7;//pin to decrease value
//int redPin = A1;//red pin for LED
//int bluePin = 10;//blue pin for LED
//int greenPin = A0;//green pin for LED
//int backlightPin = A7;//backlight pin for lcd
String menuText[] = {"Set time/date",  "12/24 hour time"};//,"Set alarm","All alarms off", "List alarms on","Alarms", "Clear alarms" };//,  "Toggle LED use", "Backlight +/-"};//list of items in menu
//String patternName[] = {":ALARM 0   ", ":Alarm sound B ", ":Alarm sound C ", ":Alarm sound D ", ":Oscillating A  ", ":Oscillating B  ", ":High pitch    ", ":Random tones  "};//list of alarm pattern names
Time T;//time stored on arduino, often updated by RTC
int a = 0;//used in functions
int u1 = 0;//used in functions
int d1 = 0;//used in functions
/*byte arrow[8] = {//right pointing arrow used in menu
    0b00000,
    0b00100,
    0b00010,
  0b11111,
  0b00010,
  0b00100,
  0b00000,
  0b00000
};
*/
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//Adafruit_TFTLCD lcd(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TFT_HX8357 lcd = TFT_HX8357();
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);//initialize LCD
DS1302 rtc(2, 3, 4);//initialize RTC




#include <DHT.h>
#define DHTPIN 10     // what pin we're connected DHT11 or DHT22
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22 
DHT dht(DHTPIN, DHTTYPE);
int h,t1;
float te;

byte refresh, citire;
extern uint8_t term[];
/*

Wiring:

220R: 220 Ohm resistor
GND: Ground
dig: digital port on arduino
5V+: 5 volt output on arduino

When viewing the LCD from behind, Left most  pin is #1:
1  -> GND
2  -> 5V+
3  -> GND
4  -> dig 5
5  -> GND
6  -> dig 4
7  -> none
8  -> none
9  -> none
10 -> none
11 -> dig 1
12 -> dig 0
13 -> dig 3
14 -> dig 2
15 -> 220r to 5V+
16 -> GND

RTC pins:
VCC1 -> none
VCC2 -> 5V+
GND  -> GND
SCLK -> analog A5
I/O  -> dig 7
RTS  -> dig 8

Button pins:
up (second to right)  : dig 9 to button to GND
down (farthest to left) :dig 10 to button to GND
menu (second to left) : dig 11 to button to GND
off (farthest to right) : dig 12 to button to GND

Speaker:
dig 13 to speaker to GND

LED, Second to left pin is GND:
1 red  : dig 6 to LED to 220r to GND
2 GND  : GND
3 blue : dig 10 to LED to 220r to GND
4 green: dig 9 to LED to 220r to GND

EEPROM values (Memory that is stored when the arduino is off):
x: alarm #: 0 to 9
x+1: alarm state. 1=on, 0=off
x+2: alarm hour 24hr, 0-23
x+3: alarm minute, 0-59
x+4: pattern. 0-7
x+5: sun  1 on, 0 off
x+6: mon  1 on, 0 off
x+7: tue  1 on, 0 off
x+8: wed  1 on, 0 off
x+9: thu  1 on, 0 off
x+10: fri  1 on, 0 off
x+11: sat 1 on, 0 off
x+12: reps. 0 to 255

504: 12/24hr. 1 = 12, 2 = 24
505: use LED, 2 = always, 1 = alarm only, 0 = no
506: backlight, 0 = none, 200 = max
*/
void setup(){
  dht.begin(); 
  rtc.halt(false);//start RTC
  rtc.writeProtect(false);
  rtc.setTCR(1);//trickle charge on
  pinMode(offPin, INPUT_PULLUP);//initialize certain pins
  pinMode(spkrPin, OUTPUT);
  pinMode(menuPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(upPin, INPUT_PULLUP);
  //pinMode(backlightPin, OUTPUT);
  //pinMode(redPin, OUTPUT);
//  pinMode(bluePin, OUTPUT);
 // pinMode(greenPin, OUTPUT);
// lcd.reset();

 // uint16_t identifier = lcd.readID();
 //  lcd.begin(identifier);
   lcd.begin();
  lcd.fillScreen(TFT_NAVY);
   lcd.setRotation(3);
  //lcd.begin(16,2);//16 columns, 2 rows
//  lcd.createChar(0,arrow);
  randomSeed(millis());
 lcd.setFreeFont(FSB12); 
 te = dht.readTemperature();
int t1 = 10*te;
h = dht.readHumidity();

citire = 0;
}

void loop(){  

char tempF[6]; //Local variable to store converted temperature reading from Real Time Clock module
float temperature; //Intermediate temperature variable to convert Celsius to Farenheit
unsigned long now = millis(); //Local variable set to current value of Arduino internal millisecond run-time timer
//struct ts t; //Structure for retrieving and storing time and date data from real time clock

// read DHT sensor just once, each 15 seconds
if ((T.sec == 0) && (citire == 0)) 
{
  citire = 1;
te = dht.readTemperature();
int t1 = 10*te;
h = dht.readHumidity();
//delay(50);
//Serial.println(t.sec);
}
if ((T.sec == 15) && (citire == 0))
{
  citire = 1;
te = dht.readTemperature();
int t1 = 10*te;
h = dht.readHumidity();
//delay(50);
//Serial.println(t.sec);
}
if ((T.sec == 30) && (citire == 0)) 
{
  citire = 1;
te = dht.readTemperature();
int t1 = 10*te;
h = dht.readHumidity();
//delay(50);
//Serial.println(t.sec);
}
if ((T.sec == 45) && (citire == 0)) 
{
  citire = 1;
te = dht.readTemperature();
int t1 = 10*te;
h = dht.readHumidity();
//delay(50);
//Serial.println(t.sec);
}
if ((T.sec == 1) || (T.sec == 16)) citire = 0;
if ((T.sec == 31) || (T.sec == 46)) citire = 0;


 
  dispTime(0);
  delay(50);
  if(readButton(menuPin)){//start menu if menupin pressed
    switch(menu()){
    case 0:
      //set time 
      DispsetTime();
      dispTime(1);
      break;
  //  case 3:
      //all alarms off
    //  allOff();
      // dispTime(1);
     // break;
  //  case 2:
      //set alarm 
   //   setAlarm();
   //   dispTime(1);
   //   break;
    case 1:
      //12/24hr time
      if(EEPROM.read(504) != 2)
       EEPROM.write(504, 2);//24
      else
        EEPROM.write(504, 1);//12
      dispTime(1);
      break;
  //  case 5:
     // view alarm
   //  viewAlarm(); 
   //   dispTime(1);
   //  break;
   // case 6:
      //reset all alarm mem
   //   for(int k=0;k<503;k++){
   //     if(EEPROM.read(k) != 0)
   //      EEPROM.write(k,0);
   //  }
     // dispTime(1);
   //   break;
   // case 4:
      //view list of activated alarms
   //   DispalarmsOn();
   //   dispTime(1);
   //   break;
 //   case 7:
      //cycle LED configurations
  //    if(EEPROM.read(505) < 2)
 //       EEPROM.write(505, EEPROM.read(505)+1);
 //     else{
 //       EEPROM.write(505, 0);
 //       }
 //     lcd.setCursor(15,1);
  //    lcd.print(EEPROM.read(505));
 //     delay(500);
 //    // outputLED(0,0,0);
  //    dispTime(1);
  //    break;
   // case 8:
      //backlight
  //    setBacklight();
   //   dispTime(1);
   //   break;
    case -1://cancel
      dispTime(1);
      break;
    }
  }   
//  if(readButton(offPin)){//press off while at home to activate backlight for 3 seconds
 //   analogWrite(backlightPin, 200);
 //   delay(3000);
 //   backlightOn();
 //   }
  
    
}

int menu(){//returns -1 for close, and other crap
  lcd.fillScreen(BLACK);
  int item = 0;
  int last = -1;
 // analogWrite(backlightPin, 200);//backlight on for menu
  while(readButton(menuPin))
    delay(20);
  while(!readButton(offPin)){//close menu with off pin
    if(item != last){
      lcd.fillScreen(BLACK);
      lcd.setCursor(0,50);
      lcd.write((byte)0);
      lcd.print(menuText[item]);
      if(item != 1){
        lcd.setCursor(0,150);
        lcd.print(" ");
        lcd.print(menuText[item+1]);
      }
    }
    last = item;
    if(readButton(menuPin))
      return item;
    if(readButton(downPin) && item<1){
      item++;
      delay(50);
    }
    if(readButton(upPin) && item>0){ 
      item--;
      delay(50);
    }      
    delay(50);
  }
  return -1; 
}

void DispsetTime(){//complete sequence to set time and date
  lcd.fillScreen(BLACK);lcd.setTextSize(2);
  while(readButton(menuPin))
    delay(20);
  lcd.setCursor(0,100);
  lcd.print("Set hour:");
  int last = -1;
  while(!readButton(menuPin)){
    lcd.setCursor(0,50);lcd.setTextColor(YELLOW,BLACK);
    if(last !=T.hour){
   lcd.setTextColor(YELLOW);  lcd.fillRect(0,0,310,65,GREEN); lcd.print(T.hour);
      if(T.hour<10)
        lcd.print(" ");
    }
    last = T.hour;

    if(readButton(upPin) && T.hour<23){
      T.hour++;
      delay(50);
    }
    if(readButton(downPin) && T.hour>0){
      T.hour--;
      delay(50);
    }
    delay(50);
  }
  lcd.fillScreen(BLACK);
  while(readButton(menuPin))
    delay(20);
  lcd.setCursor(0,100);
  lcd.print("Set minute:");
  last = -1;
  while(!readButton(menuPin)){
    lcd.setCursor(0,50);lcd.setTextColor(YELLOW,BLACK);
    if(last != T.min){
    lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);  lcd.print(T.min);
      if(T.min<10)
        lcd.print(" ");
    }
    last = T.min;
    if(readButton(upPin) && T.min<59){
      T.min++;
      delay(50);
    }
    if(readButton(downPin) && T.min>0){
      T.min--;
      delay(50);
    }
    delay(50);
  }
  lcd.fillScreen(BLACK);
  while(readButton(menuPin))
    delay(20);
  lcd.setCursor(0,100);
  lcd.print("Set day:");
  last = -1;
  while(!readButton(menuPin)){
    lcd.setCursor(0,50);
    if(last != T.date){
      lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);  lcd.print(T.date);
      if(T.date<10)
        lcd.print(" ");
    }
    last = T.date;
    if(readButton(upPin) && T.date<31){
            T.date++;
            delay(50);
                }
    if(readButton(downPin) && T.date>1){
      T.date--;
      delay(50);
    }
    delay(50);
  }
  lcd.fillScreen(BLACK);
  while(readButton(menuPin))
    delay(20);
  lcd.setCursor(0,100);
  lcd.print("Set month:");
  last = -1;
  while(!readButton(menuPin)){
    lcd.setCursor(0,50);
    if(last != T.mon){
    lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);    lcd.print(T.mon);
      if(T.mon<10)
        lcd.print(" ");
    }
    last = T.mon;
    if(readButton(upPin) && T.mon<12){
      T.mon++;
      delay(50);
    }
    if(readButton(downPin) && T.mon>1){
      T.mon--;
      delay(50);
    }
    delay(50);
  }
  lcd.fillScreen(BLACK);
  while(readButton(menuPin))
    delay(20);
  lcd.setCursor(0,100);
  lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);  lcd.print("Set year:");
  last = -1;
  while(!readButton(menuPin)){
    lcd.setCursor(0,50);
    if(last != T.year){
      lcd.print(T.year);
      if(T.year<10)
        lcd.print(" ");
    }
    last = T.year;
    if(readButton(upPin) && T.year<2100){
      T.year++;
      delay(50);
    }
    if(readButton(downPin) && T.year>2015){
      T.year--;
      delay(50);
    }
    delay(50);
  }
  lcd.fillScreen(BLACK);
  while(readButton(menuPin))
    delay(20);
  lcd.setCursor(0,100);
  lcd.print("Set WK day:");
  last = -1;
  while(!readButton(menuPin)){
    lcd.setCursor(0,50);
    if(last != T.dow){
      switch(T.dow){
      case 1:
       lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);   lcd.print("Sunday          ");
        break;
      case 2:
        lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);  lcd.print("Monday          ");
        break;
      case 3:
       lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);   lcd.print("Tuesday         ");
        break;
      case 4:
       lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);   lcd.print("Wednesday       ");
        break;
      case 5:
        lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);  lcd.print("Thursday        ");
        break;
      case 6:
         lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN); lcd.print("Friday          ");
        break;
      case 7:
       lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);   lcd.print("Saturday        ");
        break;
      }
    }
    last = T.dow;
    if(readButton(upPin) && T.dow<7){
      T.dow++;
      delay(50);
    }
    if(readButton(downPin) && T.dow>1){
      T.dow--;
      delay(50);
    }
    delay(50);
  }
  updateRTC(); 
}
/*
void setAlarm(){//complete sequence to set and activate selected alarm
  lcd.fillScreen(BLACK);
  while(readButton(menuPin))
    delay(20);
  lcd.setCursor(0,100);lcd.setTextSize(2);
  lcd.print("Select alarm:");
  int last = -1;
  if(a == -1)
    a = 0;
  while(!readButton(menuPin)){
    lcd.setCursor(0,50);//lcd.setTextColor(YELLOW,BLACK);
    if(last != a){
     lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);   lcd.print(a);
      if(a<1)
        lcd.print(" ");
    }
    last = a; 
    if(readButton(upPin) && a<1){
      a++;
      delay(50);
    }
    if(readButton(downPin) && a>0){
      a--;
      delay(50);
    }
    delay(50);
  }
  lcd.fillScreen(BLACK);
  while(readButton(menuPin))
    delay(20);
  lcd.setCursor(0,100);
  lcd.print("Set hour:");
  int val = EEPROM.read(a*13+2);
  last = -1;
  while(!readButton(menuPin)){
    lcd.setCursor(0,50);
    if(last != val){
    lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);    lcd.print(val);
      if(val<10)
        lcd.print(" ");
    }
    last = val;
    if(readButton(upPin) && val<23){
      val++;
      delay(50);
    }
    if(readButton(downPin) && val>0){
      val--;
      delay(50);
    }
    delay(50);
  }
  EEPROM.write(a*13+2, val);
  lcd.fillScreen(BLACK);
  while(readButton(menuPin))
    delay(20);
  lcd.setCursor(0,100);;
  lcd.print("Set minute:");
  last = -1;
  val = EEPROM.read(a*13+3);
  while(!readButton(menuPin)){
    lcd.setCursor(0,50);
    if(last != val){
   lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);     lcd.print(val);
      if(val<10)
        lcd.print(" ");
    }
    last = val;
    if(readButton(upPin) && val<59){
      val++;
      delay(50);
    }
    if(readButton(downPin) && val>0){
      val--;
      delay(50);
    }
    delay(50);
  }
  EEPROM.write(a*13+3, val);
  lcd.fillScreen(BLACK);
  while(readButton(menuPin))
    delay(20);
  lcd.setCursor(0,100);
  lcd.print("Set pattern:");
  last = -1;
 
  val = EEPROM.read(a*13+4);
  while(!readButton(menuPin)){
    // lcd.setCursor(0,1);
    if(last != val){
   lcd.setTextColor(YELLOW); lcd.fillRect(0,0,400,65,GREEN);   lcd.setCursor(0,50);  lcd.print(val);
  lcd.setCursor(100,50);  lcd.setTextColor(YELLOW); lcd.fillRect(0,120,310,65,GREEN);   lcd.print(patternName[val]);
    }
    last = val;
    if(readButton(upPin) && val<1){
      val++;
      delay(50);
    }
    if(readButton(downPin) && val>0){
      val--;
      delay(50);
    }
    delay(50);
  }
  EEPROM.write(a*13+4, val);
  lcd.fillScreen(BLACK);
  while(readButton(menuPin))
    delay(20);
  lcd.setCursor(0,100);
  lcd.print("Up: Sun on");
  lcd.setCursor(0,50);
  lcd.print("Down: Sun off");
  do{
    u1 = readButton(upPin);
    d1 = readButton(downPin);
    delay(50);
  }
  while(!u1 && !d1);
  if(u1)
    EEPROM.write(a*13+5, 1);
  else
    EEPROM.write(a*13+5, 0);
  while(readButton(upPin) || readButton(downPin))
    delay(20);
  lcd.fillScreen(BLACK);
  lcd.setCursor(0,100);
  lcd.print("Up: Mon on");
  lcd.setCursor(0,50);
  lcd.print("Down: Mon off");
  do{
    u1 = readButton(upPin);
    d1 = readButton(downPin);
    delay(50);
  }while(!u1 && !d1);      
  if(u1)
    EEPROM.write(a*13+6, 1);
  else
    EEPROM.write(a*13+6, 0);
  while(readButton(upPin) || readButton(downPin))
    delay(20);  
  lcd.fillScreen(BLACK);
  lcd.setCursor(0,100);
  lcd.print("Up: Tue on");
  lcd.setCursor(0,50);
  lcd.print("Down: Tue off");
  do{
    u1 = readButton(upPin);
    d1 = readButton(downPin);
    delay(50);
  }while(!u1 && !d1);
  if(u1)
    EEPROM.write(a*13+7, 1);
  else
    EEPROM.write(a*13+7, 0);
  while(readButton(upPin) || readButton(downPin))
    delay(20);
  lcd.fillScreen(BLACK);
  lcd.setCursor(0,100);
  lcd.print("Up: Wed on");
  lcd.setCursor(0,50);
  lcd.print("Down: Wed off");
  do{
    u1 = readButton(upPin);
    d1 = readButton(downPin);
    delay(50);
  }while(!u1 && !d1);
  if(u1)
    EEPROM.write(a*13+8, 1);
  else
    EEPROM.write(a*13+8, 0);
  while(readButton(upPin) || readButton(downPin))
    delay(20);
  lcd.fillScreen(BLACK);
  lcd.setCursor(0,100);
  lcd.print("Up: Thu on");
  lcd.setCursor(0,50);
  lcd.print("Down: Thu off");
  do{
    u1 = readButton(upPin);
    d1 = readButton(downPin);
    delay(50);
  }while(!u1 && !d1);
  if(u1)
    EEPROM.write(a*13+9, 1);
  else
    EEPROM.write(a*13+9, 0);
  while(readButton(upPin) || readButton(downPin))
    delay(20);
  lcd.fillScreen(BLACK);
  lcd.setCursor(0,100);
  lcd.print("Up: Fri on");
  lcd.setCursor(0,50);
  lcd.print("Down: Fri off");
  do{
    u1 = readButton(upPin);
    d1 = readButton(downPin);
    delay(50);
  }while(!u1 && !d1);
  if(u1)
    EEPROM.write(a*13+10, 1);
  else
    EEPROM.write(a*13+10, 0);
  while(readButton(upPin) || readButton(downPin))
    delay(20);
  lcd.fillScreen(BLACK);
  lcd.setCursor(0,100);
  lcd.print("Up: Sat on");
  lcd.setCursor(0,50);
  lcd.print("Down: Sat off");
  do{
    u1 = readButton(upPin);
    d1 = readButton(downPin);
    delay(50);
  }while(!u1 && !d1);
  while(readButton(upPin) || readButton(downPin))
    delay(20);
  if(u1)
    EEPROM.write(a*13+11, 1);
  else
    EEPROM.write(a*13+11, 0);
    
  lcd.fillScreen(BLACK);
  lcd.setCursor(0,100);
  lcd.print("Set repetitions:");
  last = -1;
  lcd.setCursor(0,50);
  val = EEPROM.read(a*13+12);
  while(!readButton(menuPin)){
    lcd.setCursor(0,50);
    if(last != val){
   lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);     lcd.print(val);
      if(val<100)
        lcd.print("  ");
    }
    last = val;
    if(readButton(upPin) && val<255){
      val++;
      delay(50);
    }
    if(readButton(downPin) && val>0){
      val--;
      delay(50);
    }
    delay(50);
  }
  EEPROM.write(a*13+12, val);
  lcd.fillScreen(BLACK);
  turnOn(a);
}
*/
/*
void allOff(){//turn all alarms off
  for(int k=0;k<10;k++){
    if(EEPROM.read(k*13+1)!=0)
      EEPROM.write(k*13+1, 0);
  }
}


void viewAlarm(){//complete sequence to view selected alarm
  lcd.fillScreen(BLACK);
  while(readButton(menuPin))
    delay(20);
 // lcd.setCursor(0,100);lcd.setTextSize(2);
  int last = -1;
  if(a == -1)
    a = 0;
  while(!readButton(offPin)){
    lcd.setCursor(0,50);lcd.fillRect(0,0,310,65,GREEN);lcd.setTextSize(1);lcd.setTextColor(YELLOW);
    if(last != a){
      lcd.print("#");
      lcd.print(a);
      lcd.print(" ");
      if(EEPROM.read(a*13+2) <10)
        lcd.print("0");
      lcd.print(EEPROM.read(a*13+2));
      lcd.print(":");
      if(EEPROM.read(a*13+3) <10)
        lcd.print("0");
      lcd.print(EEPROM.read(a*13+3));
      lcd.print(" ");
      if(EEPROM.read(a*13+5) == 1)
        lcd.print("S"); 
      else
        lcd.print("s");
      if(EEPROM.read(a*13+6) == 1)
        lcd.print("M");
      else
        lcd.print("m");
      if(EEPROM.read(a*13+7) == 1)
        lcd.print("T");
      else
        lcd.print("t");
      if(EEPROM.read(a*13+8) == 1)
        lcd.print("W");
      else
        lcd.print("w");
      if(EEPROM.read(a*13+9) == 1)
        lcd.print("R");
      else
        lcd.print("r");
      if(EEPROM.read(a*13+10) == 1)
        lcd.print("F");
      else
        lcd.print("f");
      if(EEPROM.read(a*13+11) == 1)
        lcd.print("S"); 
      else
        lcd.print("s");
      lcd.setCursor(0,20);
      //lcd.setTextSize(2);lcd.setTextColor(YELLOW,BLACK);
      lcd.print("Pat:");
      lcd.print(EEPROM.read(a*13+4));
      lcd.print(" Reps:");
      lcd.print(EEPROM.read(a*13+12));
      if(EEPROM.read(a*13+12)<100)
        lcd.print("  ");delay(1000);
    //  outputLED(0,0,0);
  //    if(EEPROM.read(a*13+1)==1){
   //     if(EEPROM.read(a*13+12)>0)
   //       digitalWrite(greenPin,1);
    //    else
    //      digitalWrite(bluePin,1);
   //   }else
    //      digitalWrite(redPin,1);     
    }
    last = a;
    if(readButton(menuPin)){
      last = -1;
      if(EEPROM.read(a*13+1) == 0)
        EEPROM.write(a*13+1,1);
      else
        EEPROM.write(a*13+1,0);
      delay(50);
        }
    if(readButton(upPin) && a<9){
      a++;
      delay(50);
    }
    if(readButton(downPin) && a>0){
      a--;
      delay(50);
    }
    delay(50);
  }
}


void DispalarmsOn(){//lcds alarms turned on
  lcd.fillScreen(BLACK);lcd.setTextColor(YELLOW,BLACK);
  lcd.setCursor(0,100);
  lcd.print("Alarms on:");
  for(int a = 0;a<10;a++){
    lcd.setCursor(0,50);
    if(EEPROM.read(a*13+1)==1){
     lcd.setTextColor(YELLOW); lcd.fillRect(0,0,310,65,GREEN);   lcd.print(a);
      lcd.print(" ");
      delay(1000);
    }
  }
  while(readButton(offPin))
    delay(20);
}

//void setBacklight(){//vary backlight brightness
  //lcd.fillScreen(BLACK);
 // while(readButton(menuPin))
 //   delay(20);
 // lcd.setCursor(0,0);
 // lcd.print("Backlight %:");
 /* int last = -2;
  if(EEPROM.read(506) == 101)
    a = -1;
  else
    a = ceil((float) 100*EEPROM.read(506)/255);
    
  while(!readButton(menuPin)){
    lcd.setCursor(0,1);
    if((last != a)){
      if(a>=0){
        lcd.print(a);
      //  analogWrite(backlightPin, 255*a/100);
        lcd.print("          ");
        }
      else{
        lcd.print("auto level");
        backlightOn(-1);
        }
    }
    
    last = a;
    if(readButton(upPin) && a <100){
      a++;
      delay(50);
    }
    if(readButton(downPin) && a>-1){
      a--;
      delay(50);
    }
    delay(50);
  }
  if(a == -1)
    EEPROM.write(506, 101);
  else
    EEPROM.write(506, 255*a/100);
*/  
//}

void updateRTC(){//set Hour, Minute, Day, Month, Year to RTC memory
  rtc.setTime(T.hour, T.min, 0);
  rtc.setDate(T.date, T.mon, T.year);
  rtc.setDOW(T.dow); 
}

void readRTC(){//set Hour, Minute, Day, Month, Year from RTC memory
  Time t = rtc.getTime();
    T = t;
}

//void turnOn(int num){//turn selected alarm on
//  EEPROM.write(num*13+1, 1);
//}

//void turnOff(int num){//turn selected alarm off
 // EEPROM.write(num*13+1, 0);
//}

void dispDay(){//print date in d.m.y on second line
  lcd.setCursor(5, 25);lcd.setFreeFont(FMBO12); 
  switch(T.dow){
  case 1:
    lcd.print("Duminica");
    break;
  case 2:
    lcd.print("Luni");
    break;
  case 3:
    lcd.print("Marti");
    break;
  case 4:
    lcd.print("Miercuri");
    break;
  case 5:
 lcd.print("Joi");
    break;
  case 6:
    lcd.print("Vineri");
    break;
  case 7:
    lcd.print("Simbata");
    break;
  }
  
   lcd.setCursor(140, 35); lcd.setTextSize(2);lcd.setTextColor(CYAN);lcd.setFreeFont(FSB12);
  lcd.print(T.date);
  lcd.print(" -");
  dispMonth();
  //lcd.print(T.mon);
 // lcd.print("-"); 
  lcd.setCursor(370, 35); 
  lcd.print(T.year);//year always printed YYYY
  lcd.setCursor(170, 313);lcd.setTextSize(1);lcd.setTextColor(BLUE);lcd.setFreeFont(FSSO9);
  lcd.print("ARDUINO Mega cu DS1302 si DHT11");
}

void dispTime(boolean now){//if now, print no matter what
  int lastMinute = T.min;
  //int lastSecond = T.sec;
  readRTC();
  // if(lastSecond != T.sec || now){//only lcd if minute has changed
  // T.sec=T.sec;
  //  delay(500);}
  if(lastMinute != T.min || now){//only lcd if minute has changed
    delay(500);
   // if(EEPROM.read(505) == 2)//use LED
   //   lightLED();
   // else
   //   outputLED(0,0,0);
    if(T.hour == 1 && T.min == 1 && now == 0){//set back 22 seconds per day
      rtc.setTime(T.hour, T.min-1, T.sec+38);
      lcd.print("Please wait...");
      delay(30000);
    }
   // analogWrite(backlightPin, 255);
    //backlightOn();//use backlight
    lcd.fillScreen(BLACK);lcd.setTextColor(YELLOW);lcd.setFreeFont(FSB24);
    lcd.setCursor(20,180);lcd.setTextSize(4);
    
  
     
    int hr = EEPROM.read(504);
    if(hr == 2 && T.hour<10)
      lcd.print("0");
    if(hr == 1 && T.hour>12)
      lcd.print(T.hour-12);
    if(hr == 2)
      lcd.print(T.hour);
    if(hr ==1 && T.hour == 0)
      lcd.print(12);
    if(hr == 1 && T.hour <13 && T.hour !=0)
      lcd.print(T.hour);
    lcd.print(":");
    if(T.min<10)
      lcd.print("0");
    lcd.print(T.min);
    
 
    
    if(hr == 1 && T.hour>11)
      lcd.print(" PM");
    if(hr == 1 && T.hour<12)
      lcd.print(" AM");
   lcd.setFreeFont(FSB12); 
   // checkAlarms();//lcd.setFreeFont(FM12);
  lcd.setTextSize(1); lcd.setTextColor(MAGENTA); dispDay();
  clima(); //lcd.setFreeFont(FM09); 
  }   
}

boolean dayMatch(int a){//check if alarm's day matches current day
  switch(T.dow){
  case 1://Sunday
    if(EEPROM.read(a*13+5) == 1)
      return 1;
    return 0;
  case 2://Monday
    if(EEPROM.read(a*13+6)==1) 
      return 1;
    return 0;
  case 3://Tuesday
    if(EEPROM.read(a*13+7)== 1)
      return 1;
    return 0;
  case 4://Wednesday
    if(EEPROM.read(a*13+8)==1)
      return 1;
    return 0;
  case 5://Thursday
    if(EEPROM.read(a*13+9)==1 ) 
      return 1;
    return 0;
  case 6://Friday
    if(EEPROM.read(a*13+10)==1 )
      return 1;
    return 0;
  case 7://Saturday
    if(EEPROM.read(a*13+11)== 1) 
      return 1;
    return 0;
  }
}  
/*
void checkAlarms(){//to be run once a minute
  for(int a = 0;a<10;a++){//go thru alarmsOn
    if(EEPROM.read(a*13+1) == 1){
      if(dayMatch(a) && EEPROM.read(a*13+12)>0){
        if(T.hour==EEPROM.read(a*13+2) && T.min==EEPROM.read(a*13+3)){//if time=alarm, play alarm
          lcd.setCursor(0,230);
       lcd.setTextColor(BLUE);  lcd.setTextSize(1); lcd.print("Playing alarm:"+(a));
        //  lcd.print(a);
          EEPROM.write(a*13+12,EEPROM.read(a*13+12)-1);
          playAlarm(EEPROM.read(a*13+4), 50);
          break;
        }
      }
    }
  }
}
*/
/*
void playAlarm(int pat, int num){//play alarm 0 to 7, cancelled by offPin high
  int f = 0;
  if(num == 0)
    return;
 // if(EEPROM.read(505) != 0){
  //  digitalWrite(redPin, 1);
  //  digitalWrite(greenPin, 1);
  //  digitalWrite(bluePin, 1);//bright white
  //  }
  //digitalWrite(backlightPin, 1);//backlight on
  switch(pat){
    //pattern 1: John Sena
  case 0:
    tone(spkrPin, 367, 500);
    delay(600);
    if(readButton(offPin))
        return;
    tone(spkrPin, 343, 250);
    delay(250);
    if(readButton(offPin))
        return;
    tone(spkrPin, 270, 250);
    delay(320);
    if(readButton(offPin))
        return;
    tone(spkrPin, 310, 2000);
    delay(2500);
    if(readButton(offPin))
        return;
    break;

    //pattern 2: alarm B
  case 1:
    for(int k = 0; k<6; k++){
      tone(spkrPin, 600,1000);
      if(readButton(offPin))
        return;
      delay(2000);
    }
    break;

    //pattern 3: oscillating1
  case 4:
    for(int k=0;k<2;k++){
      for(int f=400;f<2000;f+=50){
        tone(spkrPin, f, 100);
        if(readButton(offPin))
          return;
        delay(80);
      }
      for(int f=2000;f>400;f-=50){
        tone(spkrPin, f, 100);
        if(readButton(offPin))
          return;
        delay(80);
      }
      delay(1000);
    }
    break;

    //pattern 4: oscillating2
  case 5:
    for(int k=0;k<4;k++){
      for(int f=200;f<1200;f+=100){
        tone(spkrPin, f, 300);
        if(readButton(offPin))
          return;
        delay(50);
      }
      for(int f=1200;f>200;f-=100){
        tone(spkrPin, f, 300);
        if(readButton(offPin))
          return;
        delay(50);
      }
      delay(1000);
    }
    break;

    //pattern 5: high pitch
  case 6:
    for(int k=0;k<2;k++){
      for(int f=400;f<3000;f+=100){
        tone(spkrPin, f, 100);
        if(readButton(offPin))
          return;
        delay(80);
      }
      tone(spkrPin, 3000, 4000);
      if(readButton(offPin))
        return;
      delay(4000);
    }
    break;

    //pattern 6: random

  case 7:
    for(int k=0;k<17;k++){
      f = (random()%800) + 200;
      tone(spkrPin, f, 200);
      if(readButton(offPin))
        return;
      delay(300);
      f = (random()%1200) + 400;
      tone(spkrPin, f, 200);
      if(readButton(offPin))
        return;
      delay(300);
    }
    break;

    //pattern 7: alarm C
  case 2:
    for(int k=0;k<8;k++){
      tone(spkrPin, 523, 150);
      if(readButton(offPin))
        return;
      delay(150);
      tone(spkrPin, 659, 150);
      if(readButton(offPin))
        return;
      delay(150);
      tone(spkrPin, 784, 150);
      if(readButton(offPin))
        return;
      delay(600);
    }
    break;

    //pattern 8: alarm D
  case 3:
    for(int k=0;k<4;k++){
      tone(spkrPin, 294,150);
      if(readButton(offPin))
        return;
      delay(150);

      tone(spkrPin, 330,150);
      if(readButton(offPin))
        return;
      delay(150);

      tone(spkrPin, 370,150);
      if(readButton(offPin))
        return;
      delay(150);

      tone(spkrPin, 92,150);
      if(readButton(offPin))
        return;
      delay(150);

      tone(spkrPin, 440.000,150);
      if(readButton(offPin))
        return;
      delay(150);

      tone(spkrPin, 494,150);
      if(readButton(offPin))
        return;
      delay(150);

      tone(spkrPin, 554.365,150);
      if(readButton(offPin))
        return;
      delay(150);

      tone(spkrPin, 587.330,150);
      if(readButton(offPin))
        return;
      delay(150);
    }
    break;
  }
  playAlarm(pat, num-1);
}
*/
/*void lightLED(){//light up the light according to time of day
  switch(T.hour){
    case 22:
    case 23:
    case 4:
    case 3:
    case 2:
    case 1:
    case 0:
      outputLED(25/9, 0, 100/9);
      break;
    case 5:
      outputLED(150/8, 100/4, 145/8);
      break;
    case 6:
      outputLED(150/6, 150/3, 50/6);
      break;
    case 7:
      outputLED(200/6, 150/3, 0);
      break;
    case 8:
      outputLED(200/6, 171/3, 0);
      break;
    case 16:
    case 15:
    case 14:
    case 13:
    case 12:
    case 11:
    case 10:
    case 9:
      outputLED(200/6, 255/3, 0);
      break;
    case 17:
      outputLED(200/6, 200/3, 0);
      break;
    case 18:
      outputLED(200/6, 150/3, 0);
      break;
    case 19:
      outputLED(200/6, 100/3, 50/6);
      break;
    case 20:
      outputLED(150/6, 50/3, 150/6);
      break;
    case 21:
      outputLED(137/16, 0, 200/16);
      break;
  }
} 
//*/
//void outputLED(int r, int g, int b){//colors from 0 to 255
//  analogWrite(redPin, r);
//  analogWrite(bluePin, b);
//  analogWrite(greenPin, g);
//}

//void backlightOn(int value){//turn on the backlight with given EEPROM value
/*  if(value != 101)
    analogWrite(backlightPin, value);
  else if(value<0){
    switch(T.hour){
    case 6:
    case 21:
      analogWrite(backlightPin, 1);
      break;
   case 7:
    case 8:
    case 19:
    case 20:
      analogWrite(backlightPin, 15);
      break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
  case 15:
    case 16:
   case 17:
    case 18:
      analogWrite(backlightPin, 20);
      break;
    default:
      analogWrite(backlightPin, 0);
      break;
      }
    }
    */
 // }
  
//void backlightOn(){//turn on the backlight with EEPROM set value
  /*int value = EEPROM.read(506);
  if(value != 101)
    analogWrite(backlightPin, value);
  else{
    switch(T.hour){
    case 6:
    case 21:
      analogWrite(backlightPin, 1);
      break;
    case 7:
    case 8:
    case 19:
    case 20:
      analogWrite(backlightPin, 15);
      break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
  case 16:
    case 17:
    case 18:
      analogWrite(backlightPin, 20);
      break;
    default:
      analogWrite(backlightPin, 0);
      break;
      }
    }
    */
 // }
  
bool readButton(int pin){//debounce button and invert output
  if(digitalRead(pin)){
    return 0;
  }
  else{
    delay(40);
    return 1;
    }
    
}
void dispMonth() {//print date in d.m.y on second line
  lcd.setCursor(225, 35);
  switch(T.mon){
  case 1:
    lcd.print("IAN -");
    break;
    case 2:
    lcd.print("FEB -");
    break;
    case 3:
    lcd.print("MAR -");
    break;
    case 4:
    lcd.print("APR -");
    break;
    case 5:
    lcd.print("MAI -");
    break;
    case 6:
    lcd.print("IUN -");
    break;
    case 7:
    lcd.print("IUL -");
    break;
    case 8:
  
   lcd.print("AUG -");
    break;
    case 9:
  lcd.print("SEP -");
    break;
    case 10:
   lcd.print("OCT -");
    break;
    case 11:
    lcd.print("NOI -");
    break;
    case 12:
   lcd.print("DEC -");
    break;
  }}

  void clima(){
 lcd.setCursor(30,230);
lcd.setTextSize(1); 
lcd.setTextColor(RED);
lcd.print("Temperatura :");
    lcd.setTextSize(1); //Set default font size to big
    //  lcd.setCursor(92,8); //Set cursor for temperature lcd
  lcd.setCursor(30,290); //Set cursor for temperature lcd
  lcd.setTextColor(RED,BLACK); lcd.setTextSize(3);
  lcd.print(int(te)); //Send temperature to lcd buffer
//  lcd.print(t); //Send temperature to lcd buffer
  lcd.setCursor(128,290); //Set cursor for temperature lcd
  lcd.print("C"); //Send temperature to lcd buffer
//  lcd.drawCircle(124,8,2,WHITE); //Draw degree symbol after temperature
 // lcd.drawCircle(120,272,3,RED); //Draw degree symbol after temperature
lcd.setTextSize(2);lcd.setCursor(105,260);   lcd.print("o");

  lcd.setCursor(260,230);
lcd.setTextSize(1); 
lcd.setTextColor(GREEN,BLACK);
lcd.print("Umiditate :");
  //lcd.setCursor(0,200); //Set cursor for temperature lcd
   lcd.setCursor(250,290);
lcd.setTextSize(3); 
  lcd.print(h); //Send humidity to lcd buffer
  lcd.setCursor(326,290);
  lcd.print("%");

  
  }

