int input = 0;
int currentMode = -1;

int switchDelay = 200;  //time needed to turn the valve ON/OFF in milliseconds

bool showMenue = true;
bool runOnce = false;

void setup() {

  pinMode(5, OUTPUT);    // digitalPin #1
  pinMode(4, OUTPUT);     // digitalPin #2
  pinMode(0, OUTPUT);     // digitalPin #3
  pinMode(2, OUTPUT);     // digitalPin #4

  digitalWrite(5, LOW);  //complementary to 
  digitalWrite(4, LOW);   //complementary to 
  digitalWrite(0, LOW);
  digitalWrite(2, LOW); 

  Serial.begin(9600);

  while (!Serial) {
  ;                     // wait for serial port to connect
  }
  Serial.println();
  Serial.println();
  Serial.println("!Please switch 'NL' and 'CR' OFF!");
  Serial.println();
  drawMenue();  

}
void drawMenue(){
  Serial.println("**************************************************************");
  Serial.println("*   Please select desired action from the list below:        *");
  Serial.println("**************************************************************");
  Serial.println("*   1: Press '1' and 'Enter' to close first valve;           *");
  Serial.println("**************************************************************");
  Serial.println("*   2: Press '2' and 'Enter' to open first valve;            *");
  Serial.println("**************************************************************");
  Serial.println("*   3: Press '3' and 'Enter' to close second valve;          *");
  Serial.println("**************************************************************");
  Serial.println("*   4: Press '4' and 'Enter' to open second valve;           *");
  Serial.println("**************************************************************");
  Serial.println("*   5: Press '5' and 'Enter' to view leakage sensor value;   *");
  Serial.println("**************************************************************");
  Serial.println("*        Press '0' and 'Enter' to draw this menue;           *");
  Serial.println("**************************************************************");
  showMenue = false;
}

void switchCycle(){
  switch(currentMode){
  
  case 0:
  Serial.println();
  Serial.println("!Please switch 'NL' and 'CR' OFF!");
  Serial.println();
  runOnce = false;
  showMenue = true;
  break;
  
  case 1:
  digitalWrite(2, HIGH);  // first valve closed
  digitalWrite(0, HIGH);    // powerUp first switch
  Serial.println("**************************************************************");
  Serial.println("*                     FIRST VALVE CLOSED!                    *");
  Serial.println("**************************************************************"); 
  currentMode = -1;
  runOnce = false;
  showMenue = true;
  delay(switchDelay);
  digitalWrite(0, LOW);    // powerDown first switch    
  break;
    
  case 2:
  digitalWrite(2, LOW);   // first valve open
  digitalWrite(0, HIGH);    // powerUp first switch
  delay(switchDelay);
  digitalWrite(0, LOW);    // powerDown first switch
  Serial.println("**************************************************************");
  Serial.println("*                     FIRST VALVE OPENED!                    *");
  Serial.println("**************************************************************"); 
  currentMode = 0;
  runOnce = false;
  showMenue = true;
  break;
            
  case 3:
  digitalWrite(5, HIGH);  // second valve closed
  digitalWrite(4, HIGH);   // powerUp second switch                    
  delay(switchDelay);
  digitalWrite(4, LOW);  // powerDown second switch
  Serial.println("**************************************************************");
  Serial.println("*                    SECOND VALVE CLOSED!                    *");
  Serial.println("**************************************************************");
  currentMode = -1;
  runOnce = false;
  showMenue = true;
  break;
    
  case 4:
  digitalWrite(5, LOW);   // second valve open
  digitalWrite(4, HIGH);   // powerUp second switch
  delay(switchDelay);
  digitalWrite(4, LOW);  // powerDown second switch
  Serial.println("**************************************************************");
  Serial.println("*                    SECOND VALVE OPENED!                    *");
  Serial.println("**************************************************************"); 
  currentMode = -1;
  runOnce = false;
  showMenue = true;
  break;
    
  case 5:
  int sensorValue = analogRead(A0);
  Serial.println("**************************************************************");
  Serial.print("Current leakage sensor value: ");Serial.println(sensorValue);
  Serial.println("**************************************************************"); 
  currentMode = -1;
  runOnce = false;
  showMenue = true;
  break;
  
  }
}

void getInput(){
  if(Serial.available() >0){
  runOnce = true; 
  }
  if(runOnce){
  while(Serial.available() > 0){
  input = Serial.parseInt();
  Serial.print("Your input is: ");
  Serial.println(input);
  }

  if(input>5 || input<0){
  Serial.println("**************************************************************");
  Serial.println("*         WRONG INPUT! Please enter value from 1 to 5        *");
  Serial.println("**************************************************************");  
  runOnce = false;
  showMenue = true;
  }
  if(input<6 && input>-1){
  currentMode = input;
  }
  switchCycle();
  }
}

void loop() {
  getInput();

  if(showMenue){
  drawMenue(); 
  }
}
