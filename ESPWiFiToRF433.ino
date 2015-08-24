#include <ESP8266WiFi.h>
const char* ssid = "ssid";
const char* password = "password";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();


void setup() {
  Serial.begin(115200);
  delay(1);
  
  mySwitch.enableTransmit(2);
  mySwitch.setPulseLength(180);
  
  // Connect to WiFi network
  Serial.println();
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
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;

  long switchCode;

  switchCode=req.substring(12).toInt();

  mySwitch.send(switchCode, 24);  
 
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n<title>Home Automation</title><body><ul><li><a href=\"/switch/5575987\">Rancilio Silvia On</a></li><li><a href=\"/switch/5575996\">Rancilio Silvia Off</a></li><li><a href=\"/switch/5576131\">Office Light On</a></li><li><a href=\"/switch/5576140\">Office Light Off</a></li><li><a href=\"/switch/5576451\">Socket 3 On</a></li><li><a href=\"/switch/5576460\">Socket 3 Off</a></li></ul><p></body>";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disconnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

