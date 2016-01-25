//================================== Libraries ==========================================\\
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <stdio.h>
#include <ctype.h>
#include <LiquidCrystal.h>
#include "DHT.h"
#define DHTPIN 7     // what digital pin we're connected to
//================================== Variables ==========================================\\
int characterMAX = 20;
int currentLine = 0;
bool firstInit = true;
LiquidCrystal_I2C lcd(0x27, 20, 4);
char hello[] = "Welcome, please wait for the system to start up...";
char *helloPtr = hello;
float temperature1, humidity1, heatIndex1;
int line; // the current line one the display we are at.
char tempLine[] = "Temperature: ";
char humidLine[] = "Humidity: ";
char heatIndexLine[] = "Heatindex: ";
char *temperaturePtr = tempLine;
char *humidityPtr = humidLine;
char *heatIndexPtr = heatIndexLine;
#define ARRAYSIZE 20
int displayLength = 20;
char tempBufferArray[50];
char *tempPtr;
int blaeser;
float rainbow, bhut, chocolate, habanero;
char habaneroPin[] = "A0";
char chocolatePin[] = "A1";
char bhutPin[] = "A2";
char rainbowPin[] = "A3";
char habaneroLine[] = "Habanero: ";
char rainbowLine[] = "Rainbow: ";
char bhutLine[] = "Bhut: ";
char chocolateLine[] = "Chocolate: ";
char *chocolatePTR = chocolateLine;
char *bhutPTR = bhutLine;
char *rainbowPTR = rainbowLine;
char *habaneroPTR = habaneroLine;
//================================== Methods ==========================================\\
void letterByLetter(char* &input, int line);
void letterByLetter(char* &input, int x, int y);
void clearDisplay();
void initSoilScreen();
void initDHT22Screen();
void updateDHT22();
void updateSoilHumidity();
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
//=============================== The main program ====================================\\

//What has to start up? //TODO Please make it so, a series of buttons can control this :)
void setup() {
  //initialize the lcd
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.home();
  clearDisplay();
  //start the dht22
  dht.begin();  
  //Tell a welcome message
  letterByLetter(helloPtr, 0);
  delay(1000);
  clearDisplay();
}


//The main loop //TODO please make it so, a series of buttons can control this part
void loop() {
  clearDisplay();
  initDHT22Screen();
  updateDHT22();
  delay(2000); // the sensor is slow, so we need a delay in order for the sensor to calibrate.
  clearDisplay();
  initSoilScreen();
  updateSoilHumidity();
  delay(2000);
}

//Method to set the static text of the Soil screen view.
void initSoilScreen(){
  //Writing the lines, each letter at a time
  letterByLetter(rainbowPTR, 0);
  letterByLetter(bhutPTR, 1);
  letterByLetter(chocolatePTR, 2);
  letterByLetter(habaneroPTR, 3);

  //inserting a % sign to tell that it is soil humidity %
  char percent[] = "%";
  // make it into a ptr to print it
  char *percentPTR = percent;
  for(int i=0; i<4; i++){
    letterByLetter(percentPTR, 19, i);
  }
}

//initializing the static text of this view
void initDHT22Screen(){
  //write the static text 
  letterByLetter(temperaturePtr, 0);
  letterByLetter(humidityPtr, 1);
  letterByLetter(heatIndexPtr, 2);
}

//updating the humidity and temperaute values
void updateDHT22(){
  
  delay(2000);//make sure that we get the 2 sec delay we need
  
  //read the temperature and humidity - heatindex is bugged //TODO please fix this, atm it is the same as the temperature
  humidity1 = dht.readHumidity();
  temperature1 = dht.readTemperature(false, false);
  heatIndex1 = dht.computeHeatIndex(temperature1, humidity1, false);
  
  // make the three float values into char arrays, so they can be displayed onto the LCD display
  char bufferHumidity[5];
  char bufferTemp[5];
  char bufferHeat[5];
  dtostrf(humidity1, 5, 1, bufferHumidity);
  dtostrf(temperature1, 5, 1, bufferTemp);
  dtostrf(heatIndex1, 5, 1, bufferHeat);
  

  //insert the values
  int character = 13;
  
  //for humidity
  for(int i = 0; i <4; i++){
    lcd.setCursor(character+i, 1);
    lcd.print(bufferHumidity[i+1]);
  } 
  lcd.setCursor(17,1); 
  lcd.print("%");

  //for Tenperature
  for(int i = 0; i <4; i++){
    lcd.setCursor(character+i, 0);
    lcd.print(bufferTemp[i+1]);
  } 
  lcd.setCursor(17,0); 
  lcd.print("C");

  //for heatindex
  for(int i = 0; i < 4; i++){
    lcd.setCursor(character+i, 2);
    lcd.print(bufferTemp[i+1]);
  } 
}

//clears the display
void clearDisplay() {
  char emptyArray[] = " ";
  for (int i = 0; i < 4; i++) { // 4 lines
    for (int k = 0; k < 20; k++) { //20 characters in each line
      lcd.setCursor(k, i);
      lcd.print(emptyArray);
    }
  }
}


int iArray;
int arraySize;
int arrayTMP;

//used for writing from the first character in the line and for strings longer than 20 characters
void letterByLetter(char* &input, int line) {
  iArray = 0;
  arraySize = 0;
  arrayTMP = 0;

  currentLine = line;
  iArray = 0;

  lcd.setCursor(0, currentLine);


//TODO please insert at function that can print multiline, and places the whole word on the next line
  while (input[iArray] != '\0') {     // While we are not at the end of the array
    if (arrayTMP == 20) {                 //if we are at the end,
      currentLine++;                      // change line
      arrayTMP = 0;                       // reset the counter
      lcd.setCursor(0, currentLine);      // place the cursor on the next line
      if (input[iArray] == ' ') {         // if the next character is a space,
        iArray++;                         // goto the next place in the array - hence a line won't start with a space :)
      }

    }
    lcd.print(input[iArray]);           //print the element iArray in input array
    delay(100);
    iArray++;                           //see the next place in the array
    arrayTMP++;                         //and count the counter for the line one up
  }
}

int chiliArraySize = 7;
float maxSoilValue = 1023.0;

//used to update the values from the soil humidity
void updateSoilHumidity(){
  //read from analog 0-3
  rainbow = analogRead(A0);
  bhut = analogRead(A1);
  chocolate = analogRead(A2);

  //convert to percentages
  float rainbowPercent = (float)(rainbow/maxSoilValue)*100;  
  float bhutPercent= (float)(bhut/maxSoilValue)*100;
  float chocolatePercent = (float)(chocolate/maxSoilValue)*100;
  float habaneroPercent = (float)(habanero/maxSoilValue)*100;
  
  //create chars to store the values in readable format to the LCD screen
  char rainbowArray[chiliArraySize];
  char bhutArray[chiliArraySize];
  char chocolateArray[chiliArraySize];
  char habaneroArray[chiliArraySize];

  //and then move from float to char[]
  dtostrf(rainbowPercent, 5, 1, rainbowArray);
  dtostrf(bhutPercent, 5,1, bhutArray);
  dtostrf(chocolatePercent, 5, 1, chocolateArray);
  dtostrf(habaneroPercent, 5, 1, habaneroArray);
  /*
  //conversion from int to char[]
  sprintf(rainbowArray, "%f", rainbow);
  sprintf(bhutArray, "%f", bhut);
  sprintf(chocolateArray, "%fl", chocolate);
  sprintf(habaneroArray, "%lf", habanero);
  */

  //create pointers in order to print them
  char *rainbowTmpPtr = rainbowArray;
  char *bhutTmpPtr = bhutArray;
  char *chocolateTmpPtr = chocolateArray;
  char *habaneroTmpPtr = habaneroArray;

  //place the text  //TODO make a metod to place text without delay
  letterByLetter(rainbowTmpPtr,13, 0);
  letterByLetter(bhutTmpPtr, 13, 1);
  letterByLetter(chocolateTmpPtr, 13, 2);
  letterByLetter(habaneroTmpPtr, 13, 3);
}

//used for starting at a specific position on a line, here x is the posistion on the line and y is the line
void letterByLetter(char* &input, int x, int y) {
  if(y > 4 || x > 20 || y < 0 || x < 0){    //make sure we won't get garbage requests
    return;
  }
/*Local veriables */
  iArray = 0;
  arraySize = 0;
  arrayTMP = 0;
  currentLine = y;
  iArray = 0;

  //place the cursor at the desired location
  lcd.setCursor(x, currentLine);

  while (input[iArray] != '\0') { // while we are not at the end of the array
    
    if (arrayTMP == 20) {           //if we are at the end of the line 
      currentLine++;                  //count up the line
      arrayTMP = 0;                   //reset the character counter
      x = 0;                          //make ready to change line
      lcd.setCursor(x, currentLine);  //place the cursor at the next line
      
      if (input[iArray] == ' ') {     //if the character is a space
        iArray++;                       //move another place in the array
      }
    }
    
    lcd.print(input[iArray]);       //print the resulting place in the array
    delay(100);                     //a short delay
    iArray++;                       //goto next place in the array
    arrayTMP++;                     //count up the character counter
  }
}


