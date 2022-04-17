#include <IRremote.h>
#include "SR04.h"
#include <Servo.h>

//initialising the best value for comparison
float best = 100000.00;

// initialising different variables
int increment = 17;
int greenValue = 0;
int redValue;
int blueValue;
int delaytime = 90;

// calling servo functions
Servo myservo;
float forservo;
float rotation;

// define pins pn Arduino board
#define BLUE 2
#define GREEN 3
#define RED 4

#define TRIG_PIN 11
#define ECHO_PIN 10
#define WHITE 12

const int pass = 1111;           // The password
int i = 0;                      // Variable to know the position in the sequence of a pressed number
int int_customKey = 0;          // Variable to get the integer value of the pressed number
int in_pass = 0;                // The current password being introduced
int power[4] = {1000, 100, 10, 1};    // Array to get the result of 10^(2-i) when computing the integer value of the password being introduced
int state = 1;

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a, b;
int whiteValue;

int tonePin = 7;

float start;
float current = 0;
float difference = 0;

int number = 0;
String Pressed = "Not Char";
const int RECV_PIN = 6;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;
const byte whiteLED = 12;

int pirPin = 5;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
 
void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  pinMode(whiteLED, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  pinMode(pirPin, INPUT);     // declare sensor as input
  pinMode(tonePin, OUTPUT); // Set buzzer - pin 9 as an output
  pinMode(8, OUTPUT); //tell arduino the buzzer is an output device
  analogWrite(8, 0); //raise the voltage sent out of the pin by 1
  myservo.attach(9);//connect pin 9 with the control line(the middle line of Servo) 
  myservo.write(0);// move servos to center position -> 90°

}

void loop(){

while (state==1){
  val = digitalRead(pirPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
      digitalWrite(RED, 255);
      delay(1000);
      digitalWrite(RED, 0);
      state = 2;
      start = millis();
      Serial.println(start);
      
    }
  } else {
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}

while (state==2){
  current = millis();
  difference = current - start;
  forservo = difference/30000;
  rotation = forservo*180;
  if (rotation > 180){
    rotation = 180;
  }
  myservo.write(rotation);
  Serial.println(rotation);
  if (difference > 30000){
    state = 3;
    digitalWrite(RED, 255);
  }
  if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        if(results.value == 0xFFA25D){
          //Serial.println("Power");
          Pressed = "Power";          
        }
        
        if(results.value == 0xFF629D) {
        //Serial.println("Vol+");
        Pressed = "Vol+";
        }
        
        if(results.value == 0xFFE21D) {
        //Serial.println("Func/Stop");
        Pressed = "Func";
        }
        
        if(results.value == 0xFF22DD) {
        //Serial.println("|<<");
        Pressed = "|<<";
        }
        
        if(results.value == 0xFF02FD) {
        //Serial.println("Play");
        Pressed = "Play";
        }
        
        if(results.value == 0xFFC23D) {
        //Serial.println(">>|");
        Pressed = ">>|";
        } 
                     
        if(results.value == 0xFFE01F) {
        //Serial.println("Down");
        Pressed = "Down";
        }  
        
        if(results.value == 0xFFA857) {
        //Serial.println("Vol-");
        Pressed = "Vol-";
        }
         
        if(results.value == 0xFF906F) {
        //Serial.println("Up");
        Pressed = "Up";
        } 
        
        if(results.value == 0xFF9867) {
        //Serial.println("EQ");
        Pressed = "EQ";
        }
         
        if(results.value == 0xFFB04F) {
        //Serial.println("ST");
        Pressed = "ST";
        } 

        if(results.value == 0xFF6897) {
        //Serial.println("0");
        number = 0;
        } 
        
        if(results.value == 0xFF30CF) {
        //Serial.println("1");
        number = 1;
        } 
        
        if(results.value == 0xFF18E7) {
        //Serial.println("2");
        number = 2;
        } 
        
        if(results.value == 0xFF7A85) {
        //Serial.println("3");
        number = 3;
        } 
        
        if(results.value == 0xFF10EF) {
        //Serial.println("4");
        number = 4;
        } 
        
        if(results.value == 0xFF38C7) {
        //Serial.println("5");
        number = 5;
        } 
        
        if(results.value == 0xFF5AA5) {
        //Serial.println("6");
        number = 6;
        } 
        
        if(results.value == 0xFF42BD) {
        //Serial.println("7");
        number = 7;
        } 
        
        if(results.value == 0xFF4AB5) {
        //Serial.println("8");
        number = 8;
        } 
        
        if(results.value == 0xFF52AD) {
        //Serial.println("9");
        number = 9;
        }
      //Serial.println(number);
      //Serial.println(Pressed);

      if (i < 4){                                   // If i < 3, more numbers to complete the password are needed
      //int_customKey = customKey - '0';            // Obtaining the integer value of the pressed button. ASCII code of the pressed button minus ASCII code of character '0'.
      in_pass = in_pass + number*power[i]; // Computing the integer value of the password being introduced
      i = i+1;                                    // A button has been pressed, so the position in the sequence for the next number is updated
      Serial.print('*');                          // Sending character '*' for the serial monitor
    }
  if (i >= 4){                                    // If i >= 3, a full password has been introduced. Time to check!
     Serial.println();
     if (in_pass == pass){                        // Is the introduced password equal to the password (pass)?
      Serial.println("Password is correct");      // The password introduced is correct. Sending info to the serial monitor
      in_pass = 0;                                // Resetting the value of the password introduced
      i = 0;                                      // Resetting the sequence counter

      if (difference < best){
        digitalWrite(BLUE, 255);
        for (int l = 0; l < 255; l += increment) // fades in green while blue on
          {
          greenValue += increment;
          digitalWrite(GREEN, greenValue);
          Serial.println(greenValue);
          delay(delaytime);
          }
        for (int l = 0; l < 255; l += increment) // fades out green while blue on
          {
          greenValue -= increment;
          analogWrite(GREEN, greenValue);
          Serial.println(greenValue); 
          delay(delaytime);
          }

        for (int l = 0; l < 255; l += increment) // fades in green while blue on
          {
          redValue += increment;
          digitalWrite(RED, redValue);
          Serial.println(redValue);
          delay(delaytime);
          }
        for (int l = 0; l < 255; l += increment) // fades out green while blue on
          {
          redValue -= increment;
          analogWrite(RED, redValue);
          Serial.println(redValue);
          delay(delaytime);
          }
          
         digitalWrite(BLUE, 0);
         digitalWrite(RED, 0);
         digitalWrite(GREEN, 0);
         best = difference;
      }
      
      else if (difference < 8000){
      digitalWrite(GREEN, 255);
      delay(1000);
      digitalWrite(GREEN, 0);
      }

      else {
        digitalWrite(GREEN,255);
        digitalWrite(RED, 255);
        delay(1000);
        digitalWrite(GREEN, 0);
        digitalWrite(RED,0);
      }
      
      myservo.write(0);
      state = 4;
      }
      else{
      Serial.println("Password is not correct");  // The password introduced is not correct. Sending info to the serial monitor
      in_pass = 0;                                // Resetting the value of the password introduced
      i = 0;                                      // Resetting the sequence counter   
      digitalWrite(RED, 255);
      delay(1000);
      digitalWrite(RED, 0);
      }
    }
    
      }
      
      key_value = results.value;
      irrecv.resume(); 
   delay(800); 
  }
 
while (state==3){
  Serial.println("ERROR");
  analogWrite(8, 100);
  digitalWrite(tonePin, LOW); // Send 1KHz sound signal...
  delay(2);        // ...for 1 sec
  digitalWrite(tonePin, HIGH);     // Stop sound...
  delay(2);        // ...for 1sec
  current = millis();
  difference = current - start;
  if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        if(results.value == 0xFFA25D){
          //Serial.println("Power");
          Pressed = "Power";          
        }
        
        if(results.value == 0xFF629D) {
        //Serial.println("Vol+");
        Pressed = "Vol+";
        }
        
        if(results.value == 0xFFE21D) {
        //Serial.println("Func/Stop");
        Pressed = "Func";
        }
        
        if(results.value == 0xFF22DD) {
        //Serial.println("|<<");
        Pressed = "|<<";
        }
        
        if(results.value == 0xFF02FD) {
        //Serial.println("Play");
        Pressed = "Play";
        }
        
        if(results.value == 0xFFC23D) {
        //Serial.println(">>|");
        Pressed = ">>|";
        } 
                     
        if(results.value == 0xFFE01F) {
        //Serial.println("Down");
        Pressed = "Down";
        }  
        
        if(results.value == 0xFFA857) {
        //Serial.println("Vol-");
        Pressed = "Vol-";
        }
         
        if(results.value == 0xFF906F) {
        //Serial.println("Up");
        Pressed = "Up";
        } 
        
        if(results.value == 0xFF9867) {
        //Serial.println("EQ");
        Pressed = "EQ";
        }
         
        if(results.value == 0xFFB04F) {
        //Serial.println("ST");
        Pressed = "ST";
        } 

        if(results.value == 0xFF6897) {
        //Serial.println("0");
        number = 0;
        } 
        
        if(results.value == 0xFF30CF) {
        //Serial.println("1");
        number = 1;
        } 
        
        if(results.value == 0xFF18E7) {
        //Serial.println("2");
        number = 2;
        } 
        
        if(results.value == 0xFF7A85) {
        //Serial.println("3");
        number = 3;
        } 
        
        if(results.value == 0xFF10EF) {
        //Serial.println("4");
        number = 4;
        } 
        
        if(results.value == 0xFF38C7) {
        //Serial.println("5");
        number = 5;
        } 
        
        if(results.value == 0xFF5AA5) {
        //Serial.println("6");
        number = 6;
        } 
        
        if(results.value == 0xFF42BD) {
        //Serial.println("7");
        number = 7;
        } 
        
        if(results.value == 0xFF4AB5) {
        //Serial.println("8");
        number = 8;
        } 
        
        if(results.value == 0xFF52AD) {
        //Serial.println("9");
        number = 9;
        }
      //Serial.println(number);
      //Serial.println(Pressed);

      if (i < 4){                                   // If i < 3, more numbers to complete the password are needed
      //int_customKey = customKey - '0';            // Obtaining the integer value of the pressed button. ASCII code of the pressed button minus ASCII code of character '0'.
      in_pass = in_pass + number*power[i]; // Computing the integer value of the password being introduced
      i = i+1;                                    // A button has been pressed, so the position in the sequence for the next number is updated
      Serial.print('*');                          // Sending character '*' for the serial monitor
    }
  if (i >= 4){                                    // If i >= 3, a full password has been introduced. Time to check!
     Serial.println();
     if (in_pass == pass){                        // Is the introduced password equal to the password (pass)?
      Serial.println("Password is correct");      // The password introduced is correct. Sending info to the serial monitor
      in_pass = 0;                                // Resetting the value of the password introduced
      i = 0;                                      // Resetting the sequence counter
      digitalWrite(GREEN, 255);
      delay(1000);
      digitalWrite(GREEN, 0);
      myservo.write(0);
      state = 4;
      }
      else{
      Serial.println("Password is not correct");  // The password introduced is not correct. Sending info to the serial monitor
      in_pass = 0;                                // Resetting the value of the password introduced
      i = 0;                                      // Resetting the sequence counter   
      digitalWrite(RED, 255);
      delay(1000);
      digitalWrite(RED, 0);
      }
    }
    
      }
      
      key_value = results.value;
      irrecv.resume(); 
   delay(800); 
  }


while (state == 4){
   analogWrite(BLUE, 255);
   analogWrite(GREEN, 255);
   analogWrite(RED, 255);
   analogWrite(8, 0);
   digitalWrite(tonePin, LOW);
   digitalWrite(whiteLED, 255);
   
   while (state == 4){
   if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        if(results.value == 0xFFA25D){
          Serial.println("Power");
          Pressed = "Power";
          digitalWrite(RED, LOW);
          digitalWrite(BLUE, LOW);
          digitalWrite(GREEN, LOW);
          digitalWrite(RED, LOW);
          delay(10000);
          state=1;
          digitalWrite(whiteLED, LOW);
          digitalWrite(RED, LOW);
        }
        else if(results.value == 0xFFE21D){
          Serial.println("Func");
          Pressed = "Func";
          digitalWrite(RED, 0);
          digitalWrite(BLUE, 0);
          digitalWrite(GREEN, 0);
          digitalWrite(RED,255);
          delay(900);
          digitalWrite(GREEN,255);
          delay(900);
          digitalWrite(RED,0);
          state=2;
          digitalWrite(whiteLED, LOW);
          digitalWrite(GREEN, 0);
          start = millis();
          difference = 0;
        } 
   
    }
    key_value = results.value;
    irrecv.resume();

    
   a=int(sr04.Distance());     //Return distance in cm
   Serial.print(a);
   Serial.print("cm   ");
   whiteValue = (a*5);
   if (whiteValue < 255){
     if (whiteValue > 255){
      whiteValue = 255;
     }
     if (a < 2){
      whiteValue = 0;
     }
     Serial.println(whiteValue);
     digitalWrite(whiteLED, HIGH);
     analogWrite(WHITE, whiteValue); 
     analogWrite(RED, whiteValue);
     analogWrite(GREEN, whiteValue);
     analogWrite(BLUE, whiteValue);
     delay(500);
   }
   delay(500);
   if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        if(results.value == 0xFFA25D){
          Serial.println("Power");
          Pressed = "Power";
          digitalWrite(RED, LOW);
          digitalWrite(BLUE, LOW);
          digitalWrite(GREEN, LOW);
          digitalWrite(RED,255);
          delay(10000);
          state=1;
          digitalWrite(whiteLED, LOW);
          digitalWrite(RED,0);
        }
        
        else if(results.value == 0xFFE21D){
          Serial.println("Func");
          digitalWrite(RED, 0);
          digitalWrite(BLUE, 0);
          digitalWrite(GREEN, 0);
          Pressed = "Func";
          digitalWrite(RED,255);
          delay(900);
          digitalWrite(GREEN,255);
          delay(900);
          digitalWrite(RED,0);
          state=2;
          digitalWrite(whiteLED, LOW);
          digitalWrite(GREEN, 0);
          start = millis();
          difference = 0;
        } 
    }
    key_value = results.value;
    irrecv.resume();
   }}

   }