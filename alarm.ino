// IR settings
#include <IRremote.h>
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

// IR Constants
const long BUTTON_1 = 0xFF6897;
const long BUTTON_2 = 0xFF9867;
const long BUTTON_3 = 0xFFB04F;
const long BUTTON_4 = 0xFF30CF;
const long BUTTON_5 = 0xFF18E7;
const long BUTTON_6 = 0xFF7A85;
const long BUTTON_7 = 0xFF10EF;
const long BUTTON_8 = 0xFF38C7;
const long BUTTON_9 = 0xFF5AA5;
const long BUTTON_0 = 0xFF4AB5;
const long BUTTON_OK = 0xFF02FD;
const long BUTTON_DOWN = 0xFFA857;
const long BUTTON_UP = 0xFF629D;
const long BUTTON_LEFT = 0xFF22DD;
const long BUTTON_RIGHT = 0xFFC23D;
const long BUTTON_STAR = 0xFF42BD;
const long BUTTON_HEX = 0xFF52AD;

// Variables
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int firstNumber = 1;
int secondNumber = 1;
boolean isFirstNumber = true;




// CONSTANTS
const int BUTTON_PIN = 3;
const int LED_NUM1_PIN = 4;
const int LED_NUM2_PIN = 5;

const String RESET_PROGRAM_STRING = "r";

const String PLAYBACK_STRING = "p";
const String RESET_ALARM_STRING = "i";

const int RESET_ALARM_BUTTONS_COUNT = 5;
const int RESET_ALARM_BUTTONS[RESET_ALARM_BUTTONS_COUNT] = {8, 7, 6, 10}; // TODO

const int SETUP_ALARM_BUTTONS_COUNT = 5;
const int SETUP_ALARM_BUTTONS[SETUP_ALARM_BUTTONS_COUNT]= {8, 7, 6, 1}; // TODO

const int LONG_WAITING_INDEX = 2;


const int LONG_WAITING_TIME = 1500;
const int NORMAL_WAITING_TIME = 400;
const int BUTTON_PUSH_TIME = 100;
const int BETWEEN_BUTTONS_TIME = 200;



void reInit(){
  Serial.println("# Re-Init...");
  Serial.println(" ");
  Serial.println('\n');
  Serial.println("*********************************");
  
  stringComplete = false;
  inputString = "";
  
  firstNumber = 0;
  secondNumber = 0;
  
  isFirstNumber = true;
  
  initLeds();
  
  
  Serial.println("=== Button Pusher ====");
  Serial.println(" - Reset Program: ");
  Serial.print("     PC: ");
  Serial.println(RESET_PROGRAM_STRING);
  Serial.println("     Remote Control: *");
  
  Serial.println(" - Reset the Alarm: ");
  Serial.print("     PC: ");
  Serial.println(RESET_ALARM_STRING);
  Serial.println("     Remote Control: #");
  
  Serial.println(" - Playback buttons: ");
  Serial.print("     PC: ");
  Serial.println(PLAYBACK_STRING);
  Serial.println("     Remote Control: OK");
}

void initLeds(){
  digitalWrite(LED_NUM1_PIN, LOW);
  digitalWrite(LED_NUM2_PIN, LOW);
}

void onLedNum1(){
  digitalWrite(LED_NUM1_PIN, HIGH);
}

void onLedNum2(){
  digitalWrite(LED_NUM2_PIN, HIGH);
}

void imitateButtonPush(){
    digitalWrite(BUTTON_PIN, HIGH);
    delay(BUTTON_PUSH_TIME);
    digitalWrite(BUTTON_PIN, LOW);
}

void imitateMultipleButtonPush(int pushCount){
  Serial.print("     Push: ");
  Serial.println(pushCount);
  int i;
  for(i = 0; i < pushCount; i = i + 1){
      imitateButtonPush();
      delay(BETWEEN_BUTTONS_TIME);
   }
}

void pushSetupButtons(){
  int i;
  Serial.println("   Go to the corresponding menu...");
  // Get in the corresponding menu item
  for (i = 0; i < SETUP_ALARM_BUTTONS_COUNT-1; i = i + 1) {
     imitateMultipleButtonPush(SETUP_ALARM_BUTTONS[i]);
     // Long wainting
     if(i == LONG_WAITING_INDEX){
        Serial.println("     Long Wainting...");
        delay(LONG_WAITING_TIME);
     }else{
        delay(NORMAL_WAITING_TIME);
     }
  }
  
  // Playback car specific numbers
  Serial.println("   Playback car specific numbers...");
  imitateMultipleButtonPush(firstNumber);
  delay(NORMAL_WAITING_TIME);
  imitateMultipleButtonPush(secondNumber);
  initLeds();
}

void pushResetAlarmButtons(){
  Serial.println("# Reset Alarm: ");

  int i;
  Serial.println("   Go to the reset menu...");
  // Get in the corresponding menu item
  for (i = 0; i < RESET_ALARM_BUTTONS_COUNT-1; i = i + 1) {
     imitateMultipleButtonPush(RESET_ALARM_BUTTONS[i]);
     // Long wainting
     if(i == LONG_WAITING_INDEX){
        Serial.println("     Long Wainting...");
        delay(LONG_WAITING_TIME);
     }else{
        delay(NORMAL_WAITING_TIME);
     }
  }
  
  Serial.println("### DONE Reset Alarm ###");
  Serial.println(" ");
}

boolean isValidNumber(String str)
{
   boolean isNum=false;
   if(!(str.charAt(0) == '+' || str.charAt(0) == '-' || isDigit(str.charAt(0)))) return false;

   for(byte i=1;i<str.length();i++)
   {
       if(!(isDigit(str.charAt(i)) || str.charAt(i) == '.')){
         Serial.println("@ Not a Char");
         return false;
       }
   }
   return true;
}




void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(BUTTON_PIN, OUTPUT);
  
  pinMode(LED_NUM1_PIN, OUTPUT);
  pinMode(LED_NUM2_PIN, OUTPUT);
  
  // Start the IR receiver
  irrecv.enableIRIn();
 
  reInit();
}

void loop() {
  // print the string when a newline arrives:
  
  irReceive();
  
  
  if (stringComplete) {
    // Write 
    Serial.print("= \"");
    Serial.print(inputString);
    Serial.println("\"");
    
    // Process Input from Serial 
    if(inputString.equals(RESET_PROGRAM_STRING)){
      reInit();
    }else if(inputString.equals(PLAYBACK_STRING)){
       Serial.print("# Playback: ");
       Serial.print(firstNumber);
       Serial.print(", ");
       Serial.print(secondNumber);
       Serial.println("...");
       
       // Playback buttons
       pushSetupButtons();
       
       Serial.println("### DONE Playback ###");
       Serial.println(" ");
    }else if(inputString.equals(RESET_ALARM_STRING)){
      pushResetAlarmButtons();
    }else if(isValidNumber(inputString)){
       // Possible numbers
       if(isFirstNumber){
         firstNumber = inputString.toInt();
         Serial.print("# First Number is set to: ");
         Serial.println(firstNumber);
         onLedNum1();
       }else{
         secondNumber = inputString.toInt();
         Serial.print("# Second Number is set to: ");
         Serial.println(secondNumber);
         onLedNum2();
       }
       isFirstNumber = !isFirstNumber;
       Serial.println("### DONE Number Settings ###");
       Serial.println(" ");
    } // End of input handling if
    
    // clear the input string
    inputString = "";
    // clear the string complete
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    if (inChar == '\n') {
      stringComplete = true;
    }else{
      inputString += inChar;
    }
  }
}


int firstIrDigit = 0;
int secondIrDigit = 0;

int firstIrNumber = 0;
int secondIrNumber = 0;

boolean isfirstIrDigit = true;
boolean isfirstIrNumber = true;


int getIrNumber(){
  switch (results.value) {
        case BUTTON_1:
          return 1;
          break;
        case BUTTON_2:
          return 2;
          break;
        case BUTTON_3:
          return 3;
          break;
        case BUTTON_4:
          return 4;
          break;
        case BUTTON_5:
          return 5;
          break;
        case BUTTON_6:
          return 6;
          break;
        case BUTTON_7:
          return 7;
          break;
        case BUTTON_8:
          return 8;
          break;
        case BUTTON_9:
          return 9;
          break;
        case BUTTON_0:
          return 0;
          break;
      }
}

void irReceive(){
   if (irrecv.decode(&results)) {
      
      // DECODE
      switch (results.value) {
        case BUTTON_1:
        case BUTTON_2:
        case BUTTON_3:
        case BUTTON_4:
        case BUTTON_5:
        case BUTTON_6:
        case BUTTON_7:
        case BUTTON_8:
        case BUTTON_9:
        case BUTTON_0:
          if(isfirstIrDigit){
            firstIrDigit = 10 * getIrNumber();
          }else{
            secondIrDigit = getIrNumber();
            int num = firstIrDigit + secondIrDigit;
            if(isFirstNumber){
              firstNumber = num;
              Serial.print("First IrNumber set to: ");
              Serial.println(num);
              onLedNum1();
            }else{
              secondNumber = num;
              Serial.print("Second IrNumber set to: ");
              Serial.println(num);
              onLedNum2();
            }
            isFirstNumber = !isFirstNumber;
          }
          isfirstIrDigit = !isfirstIrDigit;
          
          break;
        case BUTTON_STAR:
          reInit();
          break;
        case BUTTON_HEX:
          pushResetAlarmButtons();
          break;
        case BUTTON_OK:
          pushSetupButtons();
          break;
        default: 
          break;
      }
      
      irrecv.resume(); // Receive the next value
  }
}



