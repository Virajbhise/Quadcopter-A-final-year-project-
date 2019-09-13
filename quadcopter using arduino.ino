#include <ESP8266WiFi.h>
#include <Servo.h>
Servo ESC1, ESC2, ESC3, ESC4; 
int currentSpeed = 0;

int normalize(int a){
  if(a < 0)
    a = 0;
  else if(a > 255)
    a = 255;

  int c1 = a - (a % 5);
    int c2 = (a + 5) - (a % 5);
    if (a - c1 > c2 - a)
        return c2;
    else
        return c1;
}

void arm(){
  ESC1.write(255);
  ESC2.write(255);
  ESC3.write(255);
  ESC4.write(255);
  delay(5000);

  ESC1.write(0);
  ESC2.write(0);
  ESC3.write(0);
  ESC4.write(0);
  delay(5000);
}
void setSpeed(int speed){
  while(currentSpeed != speed){
    if(speed >= currentSpeed){
      currentSpeed += 5;
      int angle = currentSpeed;
      angle = map(currentSpeed, 0, 255, 0, 178); //Sets servo positions to different speeds 

      ESC1.write(angle);
      ESC2.write(angle);
      ESC3.write(angle);
      ESC4.write(angle);

      delay(500);
    }
    else if(speed <= currentSpeed){
      currentSpeed -= 5;
      int angle = currentSpeed;
      angle = map(currentSpeed, 0, 255, 0, 178); //Sets servo positions to different speeds 

      ESC1.write(angle);
      ESC2.write(angle);
      ESC3.write(angle);
      ESC4.write(angle);

      delay(500);
    }
  }
}

void setup() {
  ESC1.attach(D1); //Adds ESC to certain pin.
  ESC2.attach(D2);
  ESC3.attach(D3);
  ESC4.attach(D4);
  
  Serial.begin(115200);
  delay(10);
    
    WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
    delay(500);
  
  server.begin();
  
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    while (!client.available()) {
      delay(1);
  }

  String request = client.readStringUntil('\r');
  client.flush();
  Serial.println("Request: " + request);
  if( request.indexOf("/Action=i") != -1 ){
    Serial.println("Arming");
    arm();
    Serial.println("Årmed");
  }
  else if( request.indexOf("/Throttle=") != -1 ){
    //get throttle
      int throttle = request.substring(request.indexOf("=") + 1).toInt();
      Serial.print("Setting Throttle to: ");
      Serial.println(throttle);
      throttle = normalize(throttle);
      setSpeed( throttle );
    }
    else{
      //get esc1
    if(request.indexOf("ESC1") != -1){
      int esc1Throttle = request.substring(request.indexOf("=") + 1 ).toInt();
      esc1Throttle = normalize(esc1Throttle);
      ESC1.write(map(esc1Throttle, 0, 255, 0, 178));
      Serial.print("Setting ESC1 Throttle to: ");
      Serial.println(esc1Throttle);
    }

    //get esc2
    if(request.indexOf("ESC2") != -1){
      int esc2Throttle = request.substring(request.indexOf("=") + 1 ).toInt();
      esc2Throttle = normalize(esc2Throttle);
      ESC2.write(map(esc2Throttle, 0, 255, 0, 178));
      Serial.print("Setting ESC2 Throttle to: ");
      Serial.println(esc2Throttle);
    }
      
    //get esc2
    if(request.indexOf("ESC3") != -1){
      int esc3Throttle = request.substring(request.indexOf("=") + 1 ).toInt();
      esc3Throttle = normalize(esc3Throttle);
      ESC3.write(map(esc3Throttle, 0, 255, 0, 178));
      Serial.print("Setting ESC3 Throttle to: ");
      Serial.println(esc3Throttle);
    }

    //get esc2
    if(request.indexOf("ESC4") != -1){
      int esc4Throttle = request.substring(request.indexOf("=") + 1 ).toInt();
      esc4Throttle = normalize(esc4Throttle);
      ESC4.write(map(esc4Throttle, 0, 255, 0, 178));
      Serial.print("Setting ESC4 Throttle to: ");
      Serial.println(esc4Throttle);
    }
    }
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("1<br>");
    client.println("</html>");
  
    delay(1);
  }
}