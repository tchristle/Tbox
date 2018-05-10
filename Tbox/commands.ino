void command_lookup()
{
  if (rcvstr == "gps") { GPS_update(); }
  else if (rcvstr == "test") { test(); }
  else if (rcvstr.substring(0,9) == "text2wifi") { text2wifi(); }
  else if (rcvstr == "latlon") { latlon(); }
  else if (rcvstr == "getalt") { getalt(); }      
  else if (rcvstr == "imu") { IMU(); }
  else if (rcvstr == "lvl") { lvl(); }
  else if (rcvstr == "enc") { enc(); }
  else if (rcvstr == "calgyro") { calgyro(); }
  else if (rcvstr == "mag") { magdata(); }
  else if (rcvstr == "tmp") { imutmp(); }
  else if (rcvstr == "telem2ser") { telem2ser();}        
  else if (rcvstr == "calmag") { calmag(); }
  else if (rcvstr == "store_magcal") { store_magcal(); }
  else if (rcvstr == "recall_magcal") { recall_magcal(); }
  else if (rcvstr.substring(0,2) == "wi") { wi(); }
  else if (rcvstr == "bt") { bt(); }
  else if (rcvstr == "espon") { en_esp(); }
  else if (rcvstr == "esprst") { rst_esp(); }
  else if (rcvstr == "espcrnl") { espcrnl(); }
  else if (rcvstr == "fesp") { fesp(); }
  else if (rcvstr == "b2e") { b2e(); }
  else if (rcvstr == "u2e") { u2e(); }
  else if (rcvstr == "showESPio") { showESPio(); }
  else if (rcvstr == "clr") { clr(); }
  else if (rcvstr.substring(0,2) == "az") { MySerial.println(); MySerial.println("moving"); Az(); }
  else if (rcvstr == "elscan") { MySerial.println(); MySerial.println("moving"); ElSCAN(); }
  else if (rcvstr.substring(0,3) == "el ") { MySerial.println(); MySerial.println("moving"); El(); }
  else if (rcvstr.substring(0,3) == "hd2") { /*Ready:>hd2 HHH EEE*/  hd2(rcvstr.substring(4,7).toInt(), rcvstr.substring(8,11).toInt()); }
  else if (rcvstr == "") { MySerial.println(); }       
  else if (rcvstr != "") { MySerial.println(); MySerial.println(rcvstr); MySerial.println("Invalid"); rcvstr = ""; }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void test()
{
  Serial.println("\ntest ok");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clr()
{
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bt()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("BT MASTER MODE");
  display.display(); 
  
  while(digitalRead(2==HIGH)){
    if (Serial.available() > 0) {
      Serial3.write(Serial.read());
    }
    if (Serial3.available() > 0) {
      Serial.write(Serial3.read());
    }
  }

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.clearDisplay();
  display.display();
  digitalWrite(6, LOW);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void enc()
{
  while(MySerial.available()==0){
    long newPosition = myEnc.read();
    if (newPosition != oldPosition) {
      oldPosition = newPosition;
      Serial.println(newPosition);
    }
    
    display.clearDisplay(); 
    display.setCursor(0,0);
    display.print(newPosition/4);
    display.display();
    
    delay(50);
  }
  MySerial.println();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OLEDln(String txt)
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.print(txt);
  display.display();
}

