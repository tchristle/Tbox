//ESP8266 
////////////////////////////////////////////////////////////////////////////////////////////////
void wi()
{
  bool timeout = true;
  MySerial.println();
  while(Serial1.available()>0) Serial1.read();
  //Serial1.println(rcvstr.substring(7,rcvstr.length()));
  Serial1.println(rcvstr.substring(2));
  delay(100);
  elapsedMillis waiting;     // "waiting" starts at zero
  while (waiting < 2500) {
    if (Serial1.available()) {
      timeout = false;
      MySerial.print(char(Serial1.read()));
      waiting = 2000;     // advance waiting
    }
  }
  if(timeout) MySerial.println("timed out");
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void espcrnl()
{
  MySerial.println();
  Serial1.println();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void en_esp(){
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(4, HIGH);  //set RST high
  digitalWrite(2, HIGH);  //set CE high
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void rst_esp(){
  digitalWrite(ESP_RST, LOW);  //set RST
  digitalWrite(ESP_CE, LOW);   //set PD
  delay(100);
  digitalWrite(ESP_RST, HIGH); 
  digitalWrite(ESP_CE, HIGH); 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void fesp(){

  en_esp();
  pinMode(2, INPUT_PULLUP);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("ESP8266 FLASH MODE");
  display.display();  
  
  digitalWrite(3, LOW);  //set GPIO0
  delay(500);
  digitalWrite(4, LOW);  //set RST 
  delay(500);
  digitalWrite(4, HIGH);  //reset RST
  delay(500);
  digitalWrite(3, HIGH);  //boot serial
  delay(500);
  
  while(1){//digitalRead(2==HIGH)){
    if (Serial.available() > 0) {
      Serial1.write(Serial.read());
    }
    if (Serial1.available() > 0) {
      Serial.write(Serial1.read());
    }
  }

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.clearDisplay();
  display.display();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void b2e(){

  en_esp();
  pinMode(3, INPUT_PULLUP);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("ESP8266<->HC-05");
  display.display();  
      
  while(1){//digitalRead(3==HIGH)){
    if (Serial3.available() > 0) {
      Serial1.write(Serial3.read());
    }
    if (Serial1.available() > 0) {
      Serial3.write(Serial1.read());
    }
    if (digitalRead(BTNA)==LOW){
      return;
    }
    if (digitalRead(BTNB)==LOW){
      rst_esp();
      return;
    }    
  }

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.clearDisplay();
  display.display();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void b2el(){

  String esp_data;
  int n=0;

  en_esp();
  pinMode(3, INPUT_PULLUP);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("ESP8266<->HC-05");
  display.display();  
      
  while(1){//digitalRead(3==HIGH)){
    if (Serial3.available() > 0) {
      Serial1.write(Serial3.read());
    }
    if (Serial1.available() > 0) {
      esp_data = Serial1.read();
      Serial3.print(esp_data);
      display.print(esp_data);
      display.display();
      n++;
      if(n>167){
        n=0;
        display.clearDisplay();
        display.setCursor(0,0);
      }
    }
    if (digitalRead(BTNB)==LOW){
      rst_esp();
      return;
    }
  }

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.clearDisplay();
  display.display();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void u2e(){
  
  en_esp();
  pinMode(2, INPUT_PULLUP);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("ESP8266<->USB_ComPort");
  display.display();  
      
  while(1){//digitalRead(2==HIGH)){
    if (Serial.available() > 0) {
      Serial1.write(Serial.read());
    }
    if (Serial1.available() > 0) {
      Serial.write(Serial1.read());
    }
  }

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.clearDisplay();
  display.display();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void bu2e(){

  
  //Serial - USB
  //Serial1 - ESP8266
  //Serial3 - HC-05
  
  int msg;
  
  en_esp();
  pinMode(2, INPUT_PULLUP);

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("ESP8266<->USB_ComPort");
  display.println("        |<->HC-05");
  display.display();  
      
  while(1){//digitalRead(2==HIGH)){
    if (Serial.available() > 0) {
      msg = Serial.read();
      Serial1.write(msg);
      //Serial3.write(msg);
    }
    if (Serial1.available() > 0) {
      msg = Serial1.read();
      Serial.write(msg);
      Serial3.write(msg);
    }
    if (Serial3.available() > 0) {
      msg = Serial3.read();
      //Serial.write(msg);
      Serial1.write(msg);
    }
  }

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.clearDisplay();
  display.display();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void showESPio()
{
  pinMode(16, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  int GPIO0 = 0, GPIO2 = 0;
  
  while(MySerial.available()==0){
    while (Serial1.available() > 0) MySerial.write(Serial1.read());
    GPIO2 = digitalRead(16);
    GPIO0 = digitalRead(3);
    display.clearDisplay(); 
    display.setCursor(0,0);
    display.print("GPIO0 = "); display.print(GPIO0); display.print(", GPIO2 = "); display.print(GPIO2);
    display.display();       
  }
}

