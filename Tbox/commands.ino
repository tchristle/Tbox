void command_lookup()
{
  if (rcvstr == "gps") { GPS_update(); }
  else if (rcvstr == "test") { test(); }
  else if (rcvstr == "gps2wifi") { gps2wifi(); }
  else if (rcvstr == "telem2wifi") { telem2wifi(); }
  else if (rcvstr.substring(0,9) == "text2wifi") { text2wifi(); }
  else if (rcvstr == "latlon") { latlon(); }
  else if (rcvstr == "getalt") { getalt(); }
  else if (rcvstr == "getgpsrssi") { getgpsrssi(); }
  else if (rcvstr == "sendgpsrssi") { sendgpsrssi(); }       
  else if (rcvstr == "imu") { IMU(); }
  else if (rcvstr == "calgyro") { calgyro(); }
  else if (rcvstr == "mag") { magdata(); }
  else if (rcvstr == "tmp") { imutmp(); }
  else if (rcvstr == "telem2ser") { telem2ser();}        
  else if (rcvstr == "calmag") { calmag(); }
  else if (rcvstr == "store_magcal") { store_magcal(); }
  else if (rcvstr == "recall_magcal") { recall_magcal(); }
  else if (rcvstr.substring(0,2) == "wi") { ATCMD(); }
  else if (rcvstr == "espon") { en_esp(); }
  else if (rcvstr == "espcrnl") { espcrnl(); }
  else if (rcvstr == "fesp") { fesp(); }
  else if (rcvstr == "cipserver") { cipserver(); }
  else if (rcvstr == "webserver") { webserver(); }
  else if (rcvstr == "srvhtml") { srvhtml(); }
  else if (rcvstr == "clr") { clr(); }
  else if (rcvstr == "RSSI") { MySerial.println(); MySerial.println(getRSSI()); }
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
  float lat1 = 44.79240;
  float lon1 = -93.24369;

  float lat2 = 44.79300;
  float lon2 = -93.14400;

  MySerial.println();
  MySerial.println(getdist(lat1, lon1, lat2, lon2));
  MySerial.println(findheading(lat1, lon1, lat2, lon2));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clr()
{
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
}

