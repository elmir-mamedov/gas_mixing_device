
/*1. user zadava pomer ref:konc (napr. 52 -> 5 strik ref + 2 strik konc)
  //hotovo
//2. vyprazdnit strikacku.
  //hotovo
//3. vyplachnout strikacku vzduchem pred kazdym michanim
  //hotovo
//4.michani
  //hotovo
  5. pomer zadavan klavesnici i monitorem
  //hotovo.
  
  6. reset button
  //hotovo

  7. LCD
  //hotovo

  8. otacky
  //hotovo

  9. user input display
  //hotovo
  
*/  


#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#define I2CADDR 0x38 
#include <Servo.h>
#include "strikacka.h"
#include <LiquidCrystal_I2C.h>  /*include LCD I2C Library*/
LiquidCrystal_I2C lcd(0x27,16,2);

Servo servo1;
Servo servo2;

//variables:
unsigned long loop_number = 0;
bool escape_flag = false;
bool reset_flag= false;
int tensDigit;
int onesDigit;

float fract_1;
float fract_2;
float wholeNumber1; 
float decimalDigit1;   
float decimalDigit2;    
float decimal_number1;

float fract_3;
float fract_4;
float wholeNumber2; 
float decimalDigit3;   
float decimalDigit4;    
float decimal_number2;
float decimal_fraction1;
float decimal_fraction2;
float spin_flag1;
float spin_flag2;

static String input;

Strikacka strik;

//keypad initialization
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'D','#','0','*'},
  {'C','9','8','7'},
  {'B','6','5','4'},
  {'A','3','2','1'}
};

// Digitran keypad, bit numbers of PCF8574 i/o port
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574 );


void setup() {

  //keyboard_1();
  //keypad setup
  Wire.begin( );
  kpd.begin( makeKeymap(keys) );
  Serial.begin(9600);
 

  //LCD setup
  lcd.init();  /*LCD display initialized*/
  lcd.clear();     /*Clear LCD Display*/
  lcd.backlight();      /*Turn ON LCD Backlight*/
  
  Serial.println("Priprava serv..");
  lcd.clear();
  lcd.setCursor(0,0);   /*Set cursor to Row 1*/
  lcd.print("Priprava serv.."); /*print text on LCD*/  
  
  servo1.attach(9); // attach servo to pin 9
  delay(2000);
  servo2.attach(10); // attach servo to pin 10
  
  Serial.println("Ready");
  delay(20);
  lcd.clear();
  lcd.setCursor(6,0);   /*Set cursor to Row 1*/
  lcd.print("Ready"); /*print text on LCD*/  

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
}

void loop() {
  int analog_signal;
  analog_signal = analogRead(A0);
  
    while(true){
                  delay(500);
                  Serial.println("Press x or * to start");
                  lcd.clear();
                  lcd.setCursor(0,1);
                  lcd.print("Press * to start");
                  
                  while(!Serial.available()){
                    kb_start();
                    delay(10);
                    if (escape_flag) {
                      escape_flag = false;
                      break; // Exit the outer while loop
                    } else if(reset_flag){
                    break;
                    }
                  }
                  
                  if (reset_flag){
                  break;
                  }
                  
                  int maximumSensorState = digitalRead(7);     
                  int minimumSensorState = digitalRead(8);
                
                    
                
                  
                  // vyprazdnit strikacku
                  
                  empty();
                  
                  if (reset_flag){
                  break;
                  }
                  
                  // user input (digit separator)
                  Serial.println("Please, enter ref:conc ratio");
                  lcd.clear();
                  lcd.setCursor(0,0);
                  lcd.print("Please, enter");
                  lcd.setCursor(0,1);
                  lcd.print("ref:conc ratio");
                
                  //clear the buffer
                  
                  while(Serial.available()){
                    Serial.read();
                  }
                
                  // wait for the input from monitor or keypad
                  
                  while(!Serial.available()){
                    keyboard_2();
                    delay(10);
                    if(escape_flag){
                      escape_flag = false;
                      break;
                    }else if(reset_flag){
                      break;
                    }
                  }
                  if (reset_flag){
                    break;
                  }
                  
                  if (Serial.available() > 0) {                           // Input was made from monitor
                    String input = Serial.readStringUntil('\n'); // Read the input string until newline character
                    
                    if (input.length() == 2) { // Check if the input has exactly 2 characters
                
                      int tens_digit = input.charAt(0) - '0';
                      int ones_digit = input.charAt(1) - '0';
                      
                      //// michani
                
                           
                      rinse();
                      
                      if(reset_flag){
                        break;
                      }     
                      
                      Serial.print("Mixing reference and concentrated gases in a ratio of ");
                      Serial.print(tens_digit);
                      Serial.print(":");
                      Serial.println(ones_digit);

                      lcd.clear();
                      lcd.setCursor(2,0);   /*Set cursor to Row 1*/
                      lcd.print("ref. : konc."); /*print text on LCD*/ 
                      lcd.setCursor(5,1);   /*Set cursor to Row 1*/ 
                      lcd.print(tens_digit); /*print text on LCD*/ 
                      lcd.setCursor(6,1);
                      lcd.print(":");
                      lcd.setCursor(7,1);
                      lcd.print(ones_digit);
                      
                      int opakovani1 = 0;
                      while(opakovani1 < tens_digit){
                        ref();
                        
                        if(reset_flag){
                          break;
                        }
                        
                        opakovani1++; 
                        Serial.print("Reference gas: completed ");
                        Serial.print(opakovani1 );
                        Serial.print(" / ");   
                        Serial.println(tens_digit);   

                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Reference gas");
                        lcd.setCursor(1,1);
                        lcd.print("Completed");
                        lcd.setCursor(11,1);
                        lcd.print(opakovani1);
                        lcd.setCursor(12,1);
                        lcd.print("/");
                        lcd.setCursor(13,1);
                        lcd.print(tens_digit);

                        if(reset_flag){
                          break;
                        }
                      }
                      int opakovani2 = 0;
                      while(opakovani2 < ones_digit){
                        konc();
                        
                        if(reset_flag){
                          break;
                        }
                        
                        opakovani2++; 
                        Serial.print("Concentrate: completed ");
                        Serial.print(opakovani2 );
                        Serial.print(" / ");   
                        Serial.println(ones_digit);  

                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Concentrate gas");
                        lcd.setCursor(1,1);
                        lcd.print("Completed");
                        lcd.setCursor(11,1);
                        lcd.print(opakovani2);
                        lcd.setCursor(12,1);
                        lcd.print("/");
                        lcd.setCursor(13,1);
                        lcd.print(ones_digit);
                      }

                
                    if(reset_flag){
                      break;
                    }
                      
                    Serial.println("Hotovo");
                    }
                    else {
                      
                      if(reset_flag){
                        break;
                      }
                      
                      Serial.println("Invalid input. Please enter a 2-digit number.");
                      
                    }
                    
                    if(reset_flag){
                      break;
                    }
                    
                  }
                  
                  else {                            //input was made from keypad
                    rinse();
                    
                    if(reset_flag){
                      break;
                    }

                    if (decimal_number1 == 0.00 && decimal_number2 == 0.00){
                    
                    Serial.print("Mixing reference and concentrated gases in a ratio of ");
                    Serial.print(tensDigit);
                    Serial.print(":");
                    Serial.println(onesDigit);

                    lcd.clear();
                    lcd.setCursor(2,0);   /*Set cursor to Row 1*/
                    lcd.print("ref. : conc."); /*print text on LCD*/ 
                    lcd.setCursor(6,1);   /*Set cursor to Row 1*/ 
                    lcd.print(tensDigit); /*print text on LCD*/ 
                    lcd.setCursor(7,1);
                    lcd.print(":");
                    lcd.setCursor(8,1);
                    lcd.print(onesDigit);
                    delay(1000);
                    
                    int opakovani1 = 0;
                    while(opakovani1 < tensDigit){

                      ref();
                      
                      if(reset_flag){
                        break;
                      }     
                      

                      opakovani1++; 
                      Serial.print("Reference gas: completed ");
                      Serial.print(opakovani1 );
                      Serial.print(" / ");   
                      Serial.println(tensDigit); 

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Reference gas");
                      lcd.setCursor(1,1);
                      lcd.print("Completed");
                      lcd.setCursor(11,1);
                      lcd.print(opakovani1);
                      lcd.setCursor(12,1);
                      lcd.print("/");
                      lcd.setCursor(13,1);
                      lcd.print(tensDigit);
                          
                    }
                    
                    int opakovani2 = 0;
                    while(opakovani2 < onesDigit){
                      
                      konc();             
                      
                      if(reset_flag){
                        break;
                      } 
                      
                      opakovani2++; 
                      Serial.print("Concentrate: completed ");
                      Serial.print(opakovani2 );
                      Serial.print(" / ");   
                      Serial.println(onesDigit); 

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Concentrated gas");
                      lcd.setCursor(1,1);
                      lcd.print("Completed");
                      lcd.setCursor(11,1);
                      lcd.print(opakovani2);
                      lcd.setCursor(12,1);
                      lcd.print("/");
                      lcd.setCursor(13,1);
                      lcd.print(onesDigit);
                      
                    }
                    
                    if(reset_flag){
                      break;
                    }
                    delay(2000);
                    Serial.println("Completed.");
                    lcd.clear();
                    lcd.setCursor(3,0);
                    lcd.print("Completed! ");
                    delay(2000);
                    
                    } else {                                      //input is a decimal number
            
                    Serial.print("Mixing reference and concentrated gases in a ratio of ");
                    Serial.print(decimal_number1);
                    Serial.print(":");
                    Serial.println(decimal_number2);

                    lcd.clear();
                    lcd.setCursor(2,0);   /*Set cursor to Row 1*/
                    lcd.print("ref. : conc."); /*print text on LCD*/ 
                    lcd.setCursor(3,1);   /*Set cursor to Row 1*/ 
                    lcd.print(decimal_number1); /*print text on LCD*/ 
                    lcd.setCursor(7,1);
                    lcd.print(":");
                    lcd.setCursor(8,1);
                    lcd.print(decimal_number2);
                    delay(1000);
//ref_decimal      
                    int opakovani1 = 0;
                    while(opakovani1 < wholeNumber1){

                      ref();
                      
                      if(reset_flag){
                        break;
                      }     
                      

                      opakovani1++; 
                      Serial.print("Reference gas: completed ");
                      Serial.print(opakovani1 );
                      Serial.print(" / ");   
                      Serial.println(decimal_number1); 

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Reference gas");
                      lcd.setCursor(0,1);
                      lcd.print("Completed");
                      lcd.setCursor(10,1);
                      lcd.print(opakovani1);
                      lcd.setCursor(11,1);
                      lcd.print("/");
                      lcd.setCursor(12,1);
                      lcd.print(decimal_number1);
                          
                    }
 ///ref_frac                  
                    int total_spins = 0;
                    spin_flag1 = decimal_fraction1*122;
                    
                    keyboard_1();
  
                    if(reset_flag){
                      return;
                    }
                    
                    servo1_zavreno();
                    servo_ref();
                      
                    while (total_spins < spin_flag1){
                      keyboard_1();

                      if(reset_flag){
                      return;
                       }
                      
                      analog_signal = analogRead(A0);
                      digitalWrite(2, HIGH);
                      digitalWrite(3, LOW);

                      if (analog_signal > 1000){
                        total_spins++;
                        delay(150);
                      }
                    }
                    digitalWrite(2, LOW);
                    digitalWrite(3, LOW);

                    servo2_zavreno();
                    servo_vzorek();

                    out();
                    
                    if(reset_flag){
                      return;
                    }
//conc_decimal
                    int opakovani2 = 0;
                    while(opakovani2 < wholeNumber2){
                      
                      konc();             
                      
                      if(reset_flag){
                        break;
                      } 
                      
                      opakovani2++; 
                      Serial.print("Concentrate: completed ");
                      Serial.print(opakovani2 );
                      Serial.print(" / ");   
                      Serial.println(decimal_number2); 

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Concentrated gas");
                      lcd.setCursor(0,1);
                      lcd.print("Completed");
                      lcd.setCursor(10,1);
                      lcd.print(opakovani2);
                      lcd.setCursor(11,1);
                      lcd.print("/");
                      lcd.setCursor(12,1);
                      lcd.print(decimal_number2);
                      
                    }
                    
                    if(reset_flag){
                      break;
                    }
                    
//conc_frac
                    total_spins = 0;
                    spin_flag2 = decimal_fraction2*122;

                    keyboard_1();

                    if(reset_flag){
                      return;
                    }

                    servo1_zavreno();
                    servo_konc();

                    while (total_spins < spin_flag2){
                     
                      keyboard_1();

                      if(reset_flag){
                      return;
                       }
                      
                      analog_signal = analogRead(A0);
                      digitalWrite(2, HIGH);
                      digitalWrite(3, LOW);

                      if (analog_signal > 1000){
                        total_spins++;
                        delay(150);
                      }
                    }
                    digitalWrite(2, LOW);
                    digitalWrite(3, LOW);

                    servo2_zavreno();
                    servo_vzorek();

                    out();
                    
                    if(reset_flag){
                      return;
                    }
                                        
                    delay(2000);
                    Serial.println("Completed.");
                    lcd.clear();
                    lcd.setCursor(3,0);
                    lcd.print("Completed! ");
                    delay(2000);

                    decimal_number1 = 0.00; //reset decimal_number initiator
                    decimal_number2 = 0.00;
                    
                    }

                  }
    }
    reset();
    reset_flag=false;
    while(Serial.available()){
      Serial.read();
    }
}






























// funkce

void konc(){
  keyboard_1();
  
  if(reset_flag){
    return;
  }
  
  servo1_zavreno();
  servo_konc();
  in();

  if(reset_flag){
    return;
  }
  
  servo2_zavreno();
  servo_vzorek();
  out();

  if(reset_flag){
    return;
  }
}

void ref(){
  keyboard_1();
  
  if(reset_flag){
    return;
  }
  servo1_zavreno();
  servo_ref();
  in();
  
  if(reset_flag){
    return;
  }
  servo2_zavreno();
  servo_vzorek();
  out();
  
  if(reset_flag){
    return;
  }
}

void empty(){
  keyboard_1();

  if(reset_flag){
    return;
  }
  
  Serial.println("Emptying..");
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Emptying..");
  servo2_zavreno();
  servo_odpad();  
  out();
  if(reset_flag){
    return;
  }
  Serial.println("Emptying completed.");
  lcd.setCursor(3,1);
  lcd.print("Completed.");
  delay(1000);
}


void in(){
  int maximumSensorState = digitalRead(7);     
  int minimumSensorState = digitalRead(8); 
  while (maximumSensorState == 1) {
    keyboard_1();           
    maximumSensorState = digitalRead(7);     // senzor u motoru - stisknuty = 0
    minimumSensorState = digitalRead(8);     // senzor u strikacky - stisknuty = 0
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
 
    if(reset_flag){
    break; 
    }
  }
  
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  delay(500);

}
  
void out(){
  int maximumSensorState = digitalRead(7);     
  int minimumSensorState = digitalRead(8);  
  while (minimumSensorState == 1) {
    keyboard_1();
    maximumSensorState = digitalRead(7);     // senzor u motoru - stisknuty = 0
    minimumSensorState = digitalRead(8);     // senzor u strikacky - stisknuty = 0

    digitalWrite(2, LOW);
    digitalWrite(3, HIGH); 
    
    if(reset_flag){
    break; 
    }
  }
  
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  delay(500);
}

void rinse(){
  if(reset_flag){
    return;
  }
  keyboard_1();
  Serial.println("Performing rinsing..");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Rinsing..");
  delay(1000);
  servo1_zavreno();
  servo_ref();
  in();

  if(reset_flag){
    return;
  }
  
  servo2_zavreno();
  servo_odpad();
  out();
  
  if(reset_flag){
    return;  
  }
  
  Serial.println("Rinsing completed.");
  lcd.setCursor(0,1);
  lcd.print("Completed.");
  delay(1000);
}

void servo_odpad(){
  keyboard_1();
  servo1.writeMicroseconds(2400);
  delay(1000);
}

void servo_vzorek(){
  keyboard_1();
  servo1.writeMicroseconds(500);
  delay(1000);
}

void servo_konc(){
  keyboard_1();
  servo2.writeMicroseconds(600);
  delay(1000);
}

void servo_ref(){
  keyboard_1();
  servo2.writeMicroseconds(2350);
  delay(1000);
}

void servo1_zavreno(){
  keyboard_1();
  servo1.writeMicroseconds(1950);
  delay(1000);
}

void servo2_zavreno(){
  keyboard_1();
  servo2.writeMicroseconds(1925);
  delay(1000);
}


void Stop(){ //stop the program and then waits for B button to continue.
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);   
  Serial.println("STOP");
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("STOP");
  while(true){
    char key = kpd.getKey();
    if (key) {
      if (key == 'B') {
      Serial.println("CONTINUE");
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("CONTINUE");
      delay(1500);
      return;
      }else if(key == 'C'){
      reset_flag=true;
      return;
      }else {
      delay(10);
      }
   }
}
}

void keyboard_1(){ //keyboard input of binary number and checks for stop button
  static String input; // String to store the user input
  char key = kpd.getKey();
  if (key) {
    if (key == 'A') {
      // If the key pressed is 'A'
    Stop();
    } else if (isdigit(key)) {
      // If the key is a digit (0-9)
      // Append the digit to the input string
      input += key;

      // Check if the input string is now two characters long
      if (input.length() == 2) {
        // If two digits have been entered, print the input
        Serial.println("User input: " + input);
        // Clear the input for the next entry
        input = "";
      }
    } else {
      // If the key is not a digit, clear the input and print an error message
      input = "";
      Serial.println("Error: Invalid input");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error: Invalid input");
      delay(2000);
    }
  }
}

void keyboard_2(){      //keyboard input of binary number + extraction of ones and tens digit  + checks for stop button
  static String input; // String to store the user input
  char key = kpd.getKey();
  if (key) {
    if (key == 'A') {
      Stop();
      input = "";
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("User input:");
      
    }
    else if (isdigit(key)&& input.length() < 8 || key == '*' && input.length() < 8 || key == 'D' && input.length() < 9) {
      if (key == 'D' && input.length() > 0) {
        // If the key is 'D' and input is not empty, remove the last character
        input.remove(input.length() - 1);
        
        if (input.length() > 3 && input.charAt(1) == '*' && input.length() < 8){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("User input:");
        lcd.setCursor(0,1);
        lcd.print(input.charAt(0));
        lcd.setCursor(1,1);
        lcd.print(input.charAt(1));
        lcd.setCursor(2,1);
        lcd.print(input.charAt(2));
        lcd.setCursor(3,1);
        lcd.print(input.charAt(3));
        lcd.setCursor(4,1);
        lcd.print(" : ");
        if (input.length()> 4){
        lcd.setCursor(7,1);
        lcd.print(input.charAt(4));
        }
        if (input.length()> 5){
        lcd.setCursor(8,1);
        lcd.print(input.charAt(5));
        }
        if (input.length()> 6){
        lcd.setCursor(9,1);
        lcd.print(input.charAt(6));
        }
        if (input.length()> 7){
        lcd.setCursor(10,1);
        lcd.print(input.charAt(7));
        }
      }
      else if (input.length() < 4){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("User input:");
      lcd.setCursor(0,1);
      lcd.print(input);
      }
      
    } else if (isdigit(key) && input.length() < 8 || key == '*' && input.length() < 8) {
      // If the key is a digit (0-9)
      // Append the digit to the input string
      input += key;

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("User input:");
      lcd.setCursor(0,1);
      lcd.print(input);

      
      // Check if the input string is now two characters long
      if (input.length() == 2 && input.charAt(1) != '*') { 
        // If two digits have been entered, extract ones digit and tens digit
        tensDigit = input.charAt(0) - '0'; // Convert char to int
        onesDigit = input.charAt(1) - '0'; // Convert char to int
        
        // Print the extracted digits
        Serial.print("Tens digit: ");
        Serial.println(tensDigit);
        Serial.print("Ones digit: ");
        Serial.println(onesDigit);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("User input:");
        lcd.setCursor(0,1);
        lcd.print(input);
        delay(1000);
        
        // Clear the input for the next entry
        input = "";
        escape_flag = true;
      } else if (input.length() > 3 && input.charAt(1) == '*' && input.length() < 8){

        lcd.setCursor(4,1);
        lcd.print(" : ");
        if (input.length()> 4){
        lcd.setCursor(7,1);
        lcd.print(input.charAt(4));
        }
        if (input.length()> 5){
        lcd.setCursor(8,1);
        lcd.print(input.charAt(5));
        }
        if (input.length()> 6){
        lcd.setCursor(9,1);
        lcd.print(input.charAt(6));
        }
        if (input.length()> 7){
        lcd.setCursor(10,1);
        lcd.print(input.charAt(7));
        }
      }
      
        else if (input.length() == 8 && input.charAt(1) == '*' && input.charAt(5) == '*') { // Check if only one digit is entered before '*'
        wholeNumber1 = input.charAt(0) - '0'; // Convert the input string to an integer
        decimalDigit1 = input.charAt(2) - '0';
        decimalDigit2 = input.charAt(3) - '0';

        fract_1 = decimalDigit1/10;
        fract_2 = decimalDigit2/100;
        decimal_number1 = wholeNumber1 + fract_1 + fract_2;
        decimal_fraction1 = fract_1 + fract_2;

        wholeNumber2 = input.charAt(4) - '0'; // Convert the input string to an integer
        decimalDigit3 = input.charAt(6) - '0';
        decimalDigit4 = input.charAt(7) - '0';

        fract_3 = decimalDigit3/10;
        fract_4 = decimalDigit4/100;
        decimal_number2 = wholeNumber2 + fract_3 + fract_4;
        decimal_fraction2 = fract_3 + fract_4;

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("User input:");
        lcd.setCursor(0,1);
        lcd.print(input);
        lcd.setCursor(4,1);
        lcd.print(" : ");
        lcd.setCursor(7,1);
        lcd.print(input.charAt(4));
        lcd.setCursor(8,1);
        lcd.print(input.charAt(5));
        lcd.setCursor(9,1);
        lcd.print(input.charAt(6));
        lcd.setCursor(10,1);
        lcd.print(input.charAt(7));
        delay(1000);
        
        input = ""; // Clear the input string
        escape_flag = true;

        
      }
        else if(input.length()>7){
        input = "";
        Serial.println("Error: Invalid input");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error:");
        lcd.setCursor(0,1);
        lcd.print("Invalid input");
        delay(2000);
        }
      }
        else if (input.length()>7){
        input = "";
        Serial.println("Error: Invalid input");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error:");
        lcd.setCursor(0,1);
        lcd.print("Invalid input");
        delay(2000);
      }
        else if (input.length()>2  && input.charAt(5) != '*'){
        input = "";
        Serial.println("Error: Invalid input");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error:");
        lcd.setCursor(0,1);
        lcd.print("Invalid input");
        delay(2000);
      }
      
      else {
        // If the key is not a digit, clear the input and print an error message
        input = "";
        Serial.println("Error: Invalid input");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error:");
        lcd.setCursor(0,1);
        lcd.print("Invalid input");
        delay(2000);
      }
    }
    else {
        // If the key is not a digit, clear the input and print an error message
        input = "";
        Serial.println("Error: Invalid input");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error:");
        lcd.setCursor(0,1);
        lcd.print("Invalid input");
        delay(2000);
      }
    }
}


void reset(){
  Serial.println("Performing reset..");

  decimal_number1 = 0.00; //reset decimal_number initiator
  decimal_number2 = 0.00;
                    
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Performing reset..");
  keyboard_1();
  servo2_zavreno();
  servo_odpad();  

  int maximumSensorState = digitalRead(7);     
  int minimumSensorState = digitalRead(8);  
  while (minimumSensorState == 1) {
    keyboard_1();
    maximumSensorState = digitalRead(7);     // senzor u motoru - stisknuty = 0
    minimumSensorState = digitalRead(8);     // senzor u strikacky - stisknuty = 0

    digitalWrite(2, LOW);
    digitalWrite(3, HIGH); 
  }
  
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  delay(500);
  
  delay(1000);
  reset_flag=false;
  Serial.println("Reset completed.");
  lcd.setCursor(0,1);
  lcd.print("Reset completed.");
  delay(2000);
}

void kb_start(){
  static String input; // String to store the user input
  char key = kpd.getKey();
  if (key) {
    if (key == '*') {
    escape_flag = true;
    } else if (isdigit(key)) {
      // If the key is a digit (0-9)
      // Append the digit to the input string
      input += key;

      // Check if the input string is now two characters long
      if (input.length() == 2) {
        // If two digits have been entered, print the input
        Serial.println("User input: " + input);
        // Clear the input for the next entry
        input = "";
      }
    }else if(key == 'C'){
     reset_flag=true; 
    }else {
      // If the key is not a digit, clear the input and print an error message
      input = "";
      Serial.println("Error: Please press x or * to start.");
    }
  }
}
