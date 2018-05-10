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

