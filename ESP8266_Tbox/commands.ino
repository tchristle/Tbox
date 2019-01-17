void command_lookup(String str)
{
  if (str == "?") { Serial.println(help); }
  else if (str == "test") { test(); }
  //    WIFI    //////////////////////////////////////////////////////////////////////////////////////////////////
  else if (str == "wiscan") { wifi_scan(); }
  else if (str == "lc") { list_clients(); }
  else if (str == "c2t") { connect_telnet(); }
  else if (str == "sudp") { udp_mode(); }
  else if (str.substring(0,4) == "pudp") { send_udp(str.substring(5), rIP.c_str()); }
  else if (str == "gudp") { get_udp(); }
  else if (str == "ip") { get_ip(); }
  else if (str == "id") { get_id(); }
  else if (str == "sw") { stop_watch(); }
  else if (str == "cs") { check_server(); }
  else if (str == "rssi") { rssi(); }
  else if (str == "stanum") { get_STAnum(); }
  else if (str == "grIP") { get_rIP(); }
  else if (str.substring(0,4) == "srIP") { set_rIP(str.substring(5)); }
  //    TCP    ////////////////////////////////////////////////////////////////////////////////////////////////////
  else if (str.substring(0,2) == "sp") { sp(rcvstr.substring(2)); }
  //else if (str == "tel") {telnet_setup(); }
  else if (str == "ping") { ping(); }
  else if (str == "pong") { pong(); }
  //else if (str == "ss") { start_servo(); }
  else if (str.substring(0,3) == "con") { con(); }
  else if (str.substring(0,2) == "cp") { cp(str.substring(3)); }
  else if (str == "web") { web_setup(); web();}
  //    ESP    ////////////////////////////////////////////////////////////////////////////////////////////////////
  else if (str.substring(0,3) == "rst") { rst(); }
  //    EEPROM    /////////////////////////////////////////////////////////////////////////////////////////////////
  else if (str == "ee") { EEPROM_setup(); }
  else if (str == "genee") { GenEE(); }
  else if (str.substring(0,5) == "STAid") { STAid(); }
  else if (str.substring(0,5) == "STApw") { STApw(); }
  else if (str.substring(0,4) == "APid") { APid(); }
  else if (str.substring(0,4) == "APpw") { APpw(); }
  else if (str.substring(0,5) == "NODid") { NODid(); }
  else if (str.substring(0,7) == "setNODE") { setNODE(); }
  else if (str.substring(0,7) == "setHOST") { setHOST(); }
  else if (str.substring(0,7) == "setSTRT") { setStart(); }
  else if (str == "ver") { get_ver(); }
  //    NEOPIXEL    ////////////////////////////////////////////////////////////////////////////////////////////////
  else if (str == "ssw") { set_strip_white(); }
  else if (str == "udp2np") { udp2neopixel(); }
  else if (str == "sled1") { sled(1); }
  else if (str == "sled0") { sled(0); }
  else if (str == "sw1") { sw(1); }
  else if (str == "sw0") { sw(0); }
  //
  else if (str == "") { Serial.println(); }       
  else if (str != "") { Serial.println(rcvstr); Serial.println("Invalid"); rcvstr = ""; }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void test()
{
  Serial.println("\ntest ok");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void rst(){
  ESP.restart();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void get_id(){
  Serial.println(node_id_c);
  if(net_mode_c == "node") client.println(node_id_c);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void rssi(){
  int ss;
  String ssi;
  ss= WiFi.RSSI();
  ssi = String(ss);
  Serial.println(WiFi.RSSI());
  if(net_mode_c == "node") client.println(ss);
  if(net_mode_c == "host") sp(ssi);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void get_STAnum(){
  Serial.println(WiFi.softAPgetStationNum());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void get_ver(){
  Serial.println(sw_ver);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void stop_watch(){
  int waiting;
  while(Serial.available()<1){ }
  Serial.read();
  waiting = millis();
  while(Serial.available()<1){ }
  waiting = millis() - waiting;
  Serial.println(waiting);
  Serial.read();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//NEOPIXEL//
void set_strip_white() {
  int n=0;
  for(n=0;n<300;n++){
    strip.setPixelColor(n, 128, 128, 128); 
  }
  strip.show();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int udp2neopixel() {
  int n=0;
  incomingPacket[1023]=0;
  while(incomingPacket[1023]!=1){
    Serial.print("connection result: ");Serial.println(WiFi.waitForConnectResult());
    strip.begin();
    udp_mode();
    ArduinoOTA.begin();
    while(WiFi.status() == WL_CONNECTED) {
      ArduinoOTA.handle();
      int packetSize = Udp.parsePacket();
      if (packetSize)
      { 
        digitalWrite(LED_BUILTIN, HIGH);
        // receive incoming UDP packets
        Serial.printf("Received %d bytes from %s, port %d\n\r", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
        int len = Udp.read(incomingPacket, 1024);
        if (len > 0) {incomingPacket[len] = 0;}
        //Serial.printf("UDP packet contents: %s\n", incomingPacket);
        for(n=0;n<300;n++){
          strip.setPixelColor(n, incomingPacket[n*3+0], incomingPacket[n*3+1], incomingPacket[n*3+2]);
          }
        strip.show();
        if(incomingPacket[1023]==255){
          incomingPacket[1023]=0;
          Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
          //Udp.beginPacket(IPAddress(192,168,1,20), 5006);
          Udp.write(ESP.getSketchMD5().c_str());
          Udp.endPacket();
          }
        digitalWrite(LED_BUILTIN, LOW);
        if(incomingPacket[1023]==1) { Udp.stop(); return 0; }
      }      
    }
    //if(incomingPacket[1023]==1) return 0;
  }
}

/*
//backup

int udp2neopixel() {
  int n=0;
  incomingPacket[1023]=0;
  while(incomingPacket[1023]!=1){
    Serial.print("connection result: ");Serial.println(WiFi.waitForConnectResult());
    strip.begin();
    udp_mode();
    ArduinoOTA.begin();
    while(WiFi.status() == WL_CONNECTED) {
      ArduinoOTA.handle();
      int packetSize = Udp.parsePacket();
      if (packetSize)
      { 
        digitalWrite(LED_BUILTIN, HIGH);
        // receive incoming UDP packets
        Serial.printf("Received %d bytes from %s, port %d\n\r", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
        int len = Udp.read(incomingPacket, 1024);
        if (len > 0) {incomingPacket[len] = 0;}
        //Serial.printf("UDP packet contents: %s\n", incomingPacket);
        for(n=0;n<300;n++){
          strip.setPixelColor(n, incomingPacket[n*3+0], incomingPacket[n*3+1], incomingPacket[n*3+2]);
          }
        strip.show();
        if(incomingPacket[1023]==255){
          incomingPacket[1023]=0;
          Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
          Udp.write(ESP.getSketchMD5().c_str());
          Udp.endPacket();
          }
        digitalWrite(LED_BUILTIN, LOW);
        if(incomingPacket[1023]==1) { Udp.stop(); return 0; }
      }      
    }
    //if(incomingPacket[1023]==1) return 0;
  }
}

*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void sled(int state){
  digitalWrite(13, state);
}
//
void sw(int state){
  digitalWrite(12, state);
}

