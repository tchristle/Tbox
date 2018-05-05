// GPS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void GPS_update()
{
    MySerial.println();
    MySerial.println("Acquired Data");
    MySerial.println("-------------");
    gpsdump(gps);
    MySerial.println("-------------");
}

void gpsdump(TinyGPS &gps)
{
  long lat, lon;
  float flat, flon;
  unsigned long age, date, time, chars;
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned short sentences, failed;

  gps.get_position(&lat, &lon, &age);
  MySerial.print("Lat/Long(10^-5 deg): "); MySerial.print(lat); MySerial.print(", "); MySerial.print(lon); 
  MySerial.print(" Fix age: "); MySerial.print(age); MySerial.println("ms.");
  
  // On Arduino, GPS characters may be lost during lengthy MySerial.print()
  // On Teensy, Serial prints to USB, which has large output buffering and
  //   runs very fast, so it's not necessary to worry about missing 4800
  //   baud GPS characters.

  gps.f_get_position(&flat, &flon, &age);
  MySerial.print("Lat/Long(float): "); printFloat(flat, 5); MySerial.print(", "); printFloat(flon, 5);
  MySerial.print(" Fix age: "); MySerial.print(age); MySerial.println("ms.");

  gps.get_datetime(&date, &time, &age);
  MySerial.print("Date(ddmmyy): "); MySerial.print(date); MySerial.print(" Time(hhmmsscc): ");
    MySerial.print(time);
  MySerial.print(" Fix age: "); MySerial.print(age); MySerial.println("ms.");

  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  MySerial.print("Date: "); MySerial.print(static_cast<int>(month)); MySerial.print("/"); 
    MySerial.print(static_cast<int>(day)); MySerial.print("/"); MySerial.print(year);
  MySerial.print("  Time: "); MySerial.print(static_cast<int>(hour)); MySerial.print(":"); 
    MySerial.print(static_cast<int>(minute)); MySerial.print(":"); MySerial.print(static_cast<int>(second));
    MySerial.print("."); MySerial.print(static_cast<int>(hundredths));
  MySerial.print("  Fix age: ");  MySerial.print(age); MySerial.println("ms.");

  MySerial.print("Alt(cm): "); MySerial.print(gps.altitude()); MySerial.print(" Course(10^-2 deg): ");
    MySerial.print(gps.course()); MySerial.print(" Speed(10^-2 knots): "); MySerial.println(gps.speed());
  MySerial.print("Alt(float): "); printFloat(gps.f_altitude()); MySerial.print(" Course(float): ");
    printFloat(gps.f_course()); MySerial.println();
  MySerial.print("Speed(knots): "); printFloat(gps.f_speed_knots()); MySerial.print(" (mph): ");
    printFloat(gps.f_speed_mph());
  MySerial.print(" (mps): "); printFloat(gps.f_speed_mps()); MySerial.print(" (kmph): ");
    printFloat(gps.f_speed_kmph()); MySerial.println();

  gps.stats(&chars, &sentences, &failed);
  MySerial.print("Stats: characters: "); MySerial.print(chars); MySerial.print(" sentences: ");
    MySerial.print(sentences); MySerial.print(" failed checksum: "); MySerial.println(failed);
}

void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0) {
     MySerial.print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  MySerial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    MySerial.print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    int toPrint = int(remainder);
    MySerial.print(toPrint);
    remainder -= toPrint;
  }
}
////////////////////////////////////////////////////////////////////////////////////
void latlon()
{
  float flat, flon;
  unsigned long age;
  
  gps.f_get_position(&flat, &flon, &age);
  printFloat(flat, 6); MySerial.print(", "); printFloat(flon, 6);
  MySerial.println();
}
////////////////////////////////////////////////////////////////////////////////
float getalt()
{
  MySerial.println();
  MySerial.println(gps.altitude());
  return gps.altitude();
}
/////////////////////////////////////////////////////////////////////////////
float findheading(float lat1, float lon1, float lat2, float lon2)
{
  //caculate heading angle between two gps coordinates
  //flat earth approximation used
  //257972 ft/(deg lat)
  //368110 ft/(deg lon)
  float angle, delta_lat, delta_lon;
  delta_lat = (lat2-lat1)*257972;
  delta_lon = (lon2-lon1)*368110;
  
  if(delta_lat >= 0 && delta_lon > 0)
  {
    angle = 90-atan(delta_lat/delta_lon)*180/3.14159;
  }
  else if(delta_lat >= 0 && delta_lon < 0)
  {
    angle = 270-atan(delta_lat/delta_lon)*180/3.14159;    
  }
  else if(delta_lat <= 0 && delta_lon < 0)
  {
    angle = 180+atan(delta_lat/delta_lon)*180/3.14159;    
  }
  else if(delta_lat <= 0 && delta_lon > 0)
  {
    angle = 180+atan(delta_lat/delta_lon)*180/3.14159;    
  }
  else if (delta_lon == 0 && delta_lat > 0) angle = 0.0;
  else if (delta_lon == 0 && delta_lat < 0) angle = 180.0;
  else angle = 0.0;
  
  return angle;
}
////////////////////////////////////////////////////////////////////////////
float getdist(float lat1, float lon1, float lat2, float lon2)
{
  //caculate heading angle between two gps coordinates
  //flat earth approximation used
  //257972 ft/(deg lat)
  //368110 ft/(deg lon)

  float dist, delta_lat, delta_lon;
  delta_lat = (lat2-lat1)*257972;
  delta_lon = (lon2-lon1)*368110;
  
  dist = powf((powf(delta_lat,2) + powf(delta_lon,2)),0.5);
  
  return dist;
}
///////////////////////////////////////////////////////////////////////////////
void delay_gps(unsigned int delms)
{
  elapsedMillis waiting;
  while(waiting < delms)
  {
    if(Uart.available())
      {
        digitalWrite(led_pin, HIGH);
        char c = Uart.read();
        gps.encode(c);
        digitalWrite(led_pin, LOW);
      }
  }
}
/////////////////////////////////////////////////////////////////////////////////
void sendgpsrssi()
{
  float lat, lon;
  int rssi;
  String cipmsg;

  MySerial.println();

  //connect to  wifu2
  MySerial.println("connecting to AP...");
  //Serial1.println("AT+CWJAP=\"wifu2\",\"1234\"");
  //Serial1.println("AT+CWJAP=\"NETGEAR11\",\"niftypotato037\"");
  delay_gps(5000);
  MySerial.println("connected");
  
  MySerial.read();
  while(MySerial.available()<1)  //break loop on keystroke
  {
    //get gps and RSSI data
    MySerial.print("getting RSSI...");
    rssi = getRSSI();
    MySerial.println(rssi);
    MySerial.print("getting gps...");
    gps.f_get_position(&lat, &lon);
    printFloat(lat,5); MySerial.print(" "); printFloat(lon,5); MySerial.println();
    cipmsg = String(lat,5) + " " + String(lon,5) + " " + String(rssi);
    MySerial.println(cipmsg);
    
    //send gps and rssi data if wifi is connected
    if(rssi>-200)
    {
      MySerial.println("sending data over port 23...");
      Serial1.println("AT+CIPSTART=0,\"TCP\",\"192.168.4.1\",23");
      Serial1.print("AT+CIPSEND=0,"); Serial1.println(cipmsg.length());
      Serial1.print(cipmsg);      
    }

    //wait 5 seconds and display solid LED id connection lost
    delay_gps(2500);
    if(rssi == -200)
    {
      digitalWrite(led_pin, HIGH);
      delay(2500);      
    }
    else delay_gps(2500);
  }

}
////////////////////////////////////////////////////////////////////////////////
void getgpsrssi()
{
  float lat, lon;
  //get AP gps data and print to serial
  gps.f_get_position(&lat, &lon);
  MySerial.println();
  MySerial.print(lat); MySerial.print(" "); MySerial.print(lon); MySerial.print(" ");
  MySerial.println(calcHeading(-(imu.my - myoffset)*myscale, -(imu.mx - mxoffset)*mxscale));

  //setup listen on port 23
  Serial1.println("AT+CIPMUX=1");
  delay_gps(500);
  Serial1.println("AT+CIPSERVER=1,23");
  delay_gps(500);
  
  MySerial.read();
  while(MySerial.available()<1)  //break loop on keystroke
  {
    //check for ESP8266 data every 100ms
    delay_gps(100);
    elapsedMillis waiting;     // "waiting" starts at zero
    while (waiting < 20) 
      {
      if(Serial1.available()>0) 
        { 
          MySerial.print(char(Serial1.read())); 
        }
      }
  }
}
////////////////////////////////////////////////////////////////////////////////
void hdtrack()
{
  float lat_ant, lon_ant, lat_client, lon_client, dist, heading;
  String espmsg = "", client_data, num;
  char espchar;
  int j;

  //get local gps data
  gps.f_get_position(&lat_ant, &lon_ant);
  //scan for connected wifi for track
  //setup for listen on port 23

  MySerial.read();
  while(MySerial.available()<1)
  {
    //wait for valid data 
    if(Serial1.available()>0) 
    {
      espchar = char(Serial1.read());
      espmsg += espchar;
    }
    if(espchar == 10)
    {
      //store recvd gps data to lat_client, lon_client
      client_data = espmsg.trim();
      j=0;
      for( int i = 0; espmsg.length(); i++)
      {
        if(espmsg[i] == 32 && j == 0) {lat_client = num.toFloat(); num=""; j++;}
        if(espmsg[i] == 32 && j == 1) {lon_client = num.toFloat(); num=""; j++;}
        num += espmsg[i];
      }
    }
    
    dist = getdist(lat_ant, lon_ant, lat_client, lon_client);
    heading = findheading(lat_ant, lon_ant, lat_client, lon_client);
    if(dist>30) hd2(heading, 0);
  }
}

