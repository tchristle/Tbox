void wifi_setup(){
  if(net_mode_c == "host") WiFi.mode(WIFI_AP_STA);
  else WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_STA_c.c_str(), password_STA_c.c_str());
  if(net_mode_c == "host") WiFi.softAP(ssid_AP_c.c_str(), password_AP_c.c_str());
  Serial.print("connection result: ");Serial.println(WiFi.waitForConnectResult());
//  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
//    Serial.println("Connection Failed! Rebooting...");
//    delay(5000);
//    ESP.restart();
//  }
}
///////////////////////////////////////////////////////////////////////////////////

void wifi_scan() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  //delay(5000);
}
////////////////////////////////////////////////////////////////////////////////////////////
void JAP() {

  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
}
////////////////////////////////////////////////////////////////////////////////////////////
void list_nodes(){
  
}
///////////////////////////////////////////////////////////////////////////////////////////
void get_ip(){
  Serial.println(WiFi.localIP());
}
///////////////////////////////////////////////////////////////////////////////////////////
void phy_mode(){
  WiFi.setPhyMode(WIFI_PHY_MODE_11B);
}
///////////////////////////////////////////////////////////////////////////////////////////
void udp_mode(){
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  Serial.println();
}
///////////////////////////////////////////////////////////////////////////////////////////
void get_udp(){
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 1025);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    //Serial.print("UDP packet len"); Serial.println(len);
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void send_udp(String pkt, String rip){
    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(rip.c_str(), Udp.remotePort());
    Udp.write(pkt.c_str());
    Udp.endPacket();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void set_rIP(String ip){
  rIP = ip;
}
void get_rIP(){
  Serial.println(rIP);
}
/////////////////////////////////////////////////////////////////////////////////////////////
