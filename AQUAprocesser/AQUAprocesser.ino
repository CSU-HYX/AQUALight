#include <Wire.h>
String str = "R:0;G:0;B:0;W:0";
int sensorPin = A0;
float adapt = 0;
int cnt = 0;
int tmp = 0;

void setPwmFrequency2560(int pin, int divisor) {
  byte mode;
  if((pin >= 2 && pin <= 13) || (pin >= 44 && pin <= 46))
  {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
      if(pin == 4 || pin == 13){TCCR0B = TCCR0B & 0b11111000 | mode;}
      else if(pin == 11 || pin == 12 || pin == 13){TCCR1B = TCCR1B & 0b11111000 | mode;}
      else if(pin == 8 || pin == 9){TCCR2B = TCCR2B & 0b11111000 | mode;}
      else if(pin == 5 || pin == 2 || pin == 3){TCCR3B = TCCR3B & 0b11111000 | mode;}
      else if(pin == 6 || pin == 7 || pin == 8){TCCR4B = TCCR4B & 0b11111000 | mode;}
      else if(pin == 46 || pin == 45 || pin == 44){TCCR5B = TCCR5B & 0b11111000 | mode;}
  }
}

void setup() {
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  pinMode(3, OUTPUT);
  setPwmFrequency2560(44,8);
  pinMode(7, OUTPUT);
  setPwmFrequency2560(6,1);
  pinMode(5, OUTPUT);
  pinMode(2, OUTPUT);
  setPwmFrequency2560(5,1); //pin2,pin5属于同一个计时器，设置一次即可。
}


void loop() {
  int sensorValue = analogRead(sensorPin);
  if(cnt<10){
    tmp += sensorValue;
    cnt+=1;
    }
  if(cnt==10){
    adapt = tmp;
    adapt /= 10230;
    tmp=0;
    cnt = 0;
    }
  delay(100);
  }

void receiveEvent(int howMany) {
  String inputString="";
 while (0 <Wire.available()) {
  char c= Wire.read();
  inputString += c;
 }
    if(inputString.length()>0&&inputString[0]=='R'){str = inputString;}
  inputString = "";
  int R = str.substring(2, str.indexOf('G')-1).toInt();
  int G = str.substring(str.indexOf('G')+2,str.indexOf('B')-1).toInt();
  int B = str.substring(str.indexOf('B')+2,str.indexOf('W')-1).toInt();
  int W = str.substring(str.indexOf('W')+2,str.length()).toInt();
  analogWrite(3,int(B*adapt));
  analogWrite(7,int(R*adapt));
  analogWrite(5,int(W*adapt));
  analogWrite(2,int(G*adapt));
  delay(1000);
}
