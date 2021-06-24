#include <ESP8266WiFi.h>       // ESP8266WiFi library
#include <ESP8266WebServer.h>   //ESP8266WebServer library
#include <Servo.h>    // Servo library
const char* ssid = "Network name";  // Enter SSID here
const char* password = "****";  //Enter Password here
ESP8266WebServer server(80);  //Http port80 is defined
Servo myservo;  // create servo object to control a servo
int float_sensor = 4;     //Initialize pin for float sensor
int pos = 0;    // variable to store the servo position
int ldrPin = A0;    // select the input pin for the Ldr
int sensorValue = 0;  // variable to store the value coming from the sensor


IPAddress local_IP(192, 168, 1, 184);         // Set your Static IP address to 192, 168, 1, 184

IPAddress gateway(192, 168, 1, 1);            // Set your Gateway IP address (check in network configuration)
IPAddress subnet(255, 255, 0, 0);             // Set subnet mask    (check in network configuration)


void setup()
{
Serial.begin(115200);   //Set baude rate for serial communication
delay(100);               // Set delay of 100 ms
//WiFi.enableInsecureWEP();  //Enable WEP if hotspot is WEP type. Comment this line if hotspot is WPA type.
if (!WiFi.config(local_IP, gateway, subnet))      //If Configuration of static IP address is no successful
{
    Serial.println("Failed to configure the station");    //Print Failed to configure the station
}
Serial.println("Connecting to "); //Print Connecting to 
Serial.println(ssid);   //Print SSID on Serial monitor
myservo.attach(13);  // attach the servo on pin D7 to the servo object (pin 13 in software is pin d7 on wemos arduino map)
pinMode(float_sensor, INPUT); // Set the float sensor pin in the input mode
pinMode(ldrPin, INPUT);     // Set the ldrPin pin in the input mode
pinMode(2, OUTPUT);          //Set the pin in output mode for buzzer
digitalWrite(2, LOW);   // Initially set buzzer low
WiFi.begin(ssid, password);  //Connect to the Wifi using ssid and password
while (WiFi.status() != WL_CONNECTED) {  //Wait till WiFi is connected
  delay(1000);            // Delay of 1 second
  Serial.print(".");    //Print ... till Wifi not connected
  }                     // End of while loop
  Serial.println("");     //Print blank space
  Serial.println("WiFi connected..!");   //Print when connected to wifi
}
server.on("/", handle_OnConnect);       //when a server receives an HTTP request on the root (/) path,it will trigger this function
server.onNotFound(handle_NotFound);      //respond with an HTTP status 404 (Not Found)

server.begin();                         //start the server
Serial.println("HTTP server started");  //Print HTTP server started

void loop() {
  int buttonState = digitalRead(float_sensor);        //store value read from float sensor into variable
  sensorValue = analogRead(ldrPin);                   //store value read from ldr sensor into variable
  if(buttonState== LOW || sensorValue<1 )              // if buttonstate is low and sensorvalue is < 1
  {
    digitalWrite(2, HIGH);                              //Turn on the buzzer
    delay(1000);                                      //Delay of 1000 ms
      for (pos = 157; pos <= 165; pos += 1) { // goes from 157 degrees to 165 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }
    else
    {
     digitalWrite(2, LOW);
     for (pos = 165; pos >= 157; pos -= 1) { // goes from 165 degrees to 157 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
     }
Serial.println(buttonState);        
server.handleClient();             //handle the actual incoming HTTP requests
delay(1);                           //Delay of 1 ms
}

void handle_OnConnect()         //function created to attach to root (/) URL
{

 float sensor = digitalRead(float_sensor);      //read the values of float sensor
  server.send(200, "text/html", SendHTML(sensor));    //send the code 200,  “text/html“ and call SendHTML(sensor)

void handle_NotFound()        //function created if page not found
{   
  server.send(404, "text/plain", "Not found");      //send the code 404,  “text/plain“ and display Not found on the page
}

String SendHTML(float sensorstat)               //SendHTML() function generates a web page whenever the ESP8266 web server gets a request from a web client. 
{
  String ptr = "<!DOCTYPE html> <html>\n";      //<!DOCTYPE> indicates HTML code is sent.
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";     //make webpage responsive 
  ptr +="<title>Sensor data</title>\n";                                                                              //add title to the webpage
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";      // change the font and appearance of the webpage
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Bed number 1: Saline is full</h1>\n";      //Heading of web page
 
  ptr +="<p>Value: ";                         //dynamically display value of float sensor
  ptr +=(int)sensorstat;
 
 
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}