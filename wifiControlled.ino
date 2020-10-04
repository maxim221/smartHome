#include <ESP8266WiFi.h>

const char* ssid = "geek";  // access point SSID
const char* password = "xdx415xd";  // PASS

int switchDelay = 200;  //time needed to turn the valve ON/OFF in milliseconds

int switchOnePin = 4; // digitalPin #1
int switchTwoPin = 0; // digitalPin #2
int relayOnePin = 5; // digitalPin #3
int relayTwoPin = 2; // digitalPin #4 

WiFiServer server(80);  //  web server port

void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(switchOnePin, OUTPUT);
  pinMode(switchTwoPin, OUTPUT);
  pinMode(relayOnePin, OUTPUT);
  pinMode(relayTwoPin, OUTPUT);

  digitalWrite(switchOnePin, LOW);  //complementary to 5
  digitalWrite(switchTwoPin, LOW);   
  digitalWrite(relayOnePin, LOW);   //complementary to 2
  digitalWrite(relayTwoPin, LOW); 

  // connect to wifi

  Serial.println();               //serial verbouse
  Serial.println();
  Serial.print("Connecting to ");  
  Serial.println(ssid);
  WiFi.begin(ssid, password); 

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");     

  // server up

  server.begin();
  Serial.println("Server started");  
  Serial.print("Use this URL to connect: ");  
  Serial.print("http://");
  Serial.print(WiFi.localIP());               // IP adress to connect
  Serial.println("/");
}

void loop() {

  WiFiClient client = server.available();   //check if connected
  if (!client) {
    return;
  }
  Serial.println("new client"); 
  while(!client.available()){
  delay(1);
  }

  String request = client.readStringUntil('\r');  //read first request string
  Serial.println(request);
  client.flush();

  // hardware setup and switching
  
  int value1 = LOW;
  int value2 = LOW;
  if (request.indexOf("/1valve=OPEN") != -1) {
    digitalWrite(switchOnePin, HIGH);
    digitalWrite(relayOnePin, LOW);
    value1 = HIGH;
    delay(switchDelay);
    digitalWrite(switchOnePin, LOW);    // powerDown first switch  
  } 

  if (request.indexOf("/1valve=CLOSE") != -1){
    digitalWrite(switchOnePin, HIGH);
    digitalWrite(relayOnePin, HIGH);
    value1 = LOW;
    delay(switchDelay);
    digitalWrite(switchOnePin, LOW);    // powerDown first switch  
  }
  if (request.indexOf("/2valve=OPEN") != -1) {
    digitalWrite(switchTwoPin, HIGH);
    digitalWrite(relayTwoPin, LOW);
    value2 = HIGH;
    delay(switchDelay);
    digitalWrite(switchTwoPin, LOW);    // powerDown second switch
  } 

  if (request.indexOf("/2valve=CLOSE") != -1){
    digitalWrite(switchTwoPin, HIGH);
    digitalWrite(relayTwoPin, HIGH);
    value2 = LOW;
    delay(switchDelay);
    digitalWrite(switchTwoPin, LOW);    // powerDown second switch  
  }

  //HTTP update - response

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");  
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("1 valve is now: ");

  if(value1 == HIGH) {
    client.print("Opened");
  } else {
    client.print("Closed");
  }
  client.println("<br><br>");
  client.print("2 valve is now: ");

  if(value2 == HIGH) {
    client.print("Opened");
  } else {
    client.print("Closed");
  }
  client.println("<br><br>");
  client.println("Click <a href=\"/1valve=OPEN\">here</a> to open 1 valve <br>");
  client.println("Click <a href=\"/1valve=CLOSE\">here</a> to close 1 valve<br>");
  client.println("<br><br>");
  client.println("Click <a href=\"/2valve=OPEN\">here</a> to open 2 valve <br>");
  client.println("Click <a href=\"/2valve=CLOSE\">here</a> to close 2 valve<br>");
  client.println("</html>");

  delay(1);

  Serial.println("Client disconnected");
  Serial.println("");
}
