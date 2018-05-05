void Az()
{
  int pwid, dur;
  pwid = rcvstr.substring(3,4).toInt();
  dur = rcvstr.substring(5,6).toInt();
  if(rcvstr.substring(5,6)=='a') {dur = 'a';} //  MySerial.println("caught the a");}

  //az TX
  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  Serial1.print("rot "); Serial1.println(pwid); 
  delay_gps(dur*10); 
  Serial1.println("rot 0");
  digitalWrite(led_pin, LOW);  

  // average mag data to filter noise
  float amy=0, amx=0, amz=0;
  int avgs = 20;
  for(int i=0;i<avgs;i++)
  {
    imu.readMag();
    imu.readAccel(); 
    amy += imu.my;
    amx += imu.mx;
    amz +=imu.mz;
    delay_gps(10);
  }  
  amy = amy/avgs;
  amx = amx/avgs;
  amz = amz/avgs;
  printAttitude(imu.ax, imu.ay, imu.az, -(amy - myoffset)*myscale, -(amx - mxoffset)*mxscale, amz);
    
  //clear ESP serial buffer
  while(Serial1.available()>0) Serial1.read();
  
  
  if(rcvstr.substring(5,6).trim()=='a')
  {
    MySerial.read();
    elapsedMillis waiting;
    while(MySerial.available()<1)
    {
      imu.readGyro();
      imu.readMag();
      MySerial.print(calcHeading(-(imu.my - myoffset)*myscale, -(imu.mx - mxoffset)*mxscale)); MySerial.print(" ");  
      MySerial.print(imu.calcGyro(imu.gz-gzbias), 2); MySerial.println(" ");  
      delay(100);
    }
    
    digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
    Serial1.println("rot 0"); 
    digitalWrite(led_pin, LOW); 
    MySerial.println();

    //clear ESP serial buffer
    while(Serial1.available()>0) Serial1.read();
  }

}
/////////////////////////////////////////////////////////////////////////////////////
void Az_move(int pwid, int dur)
{
  if(pwid < 0) pwid = 0;
  if(pwid > 9) pwid = 9;
  if(dur < 0) dur = 0;
  if(dur > 9) dur = 9;
  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  Serial1.println("AT+CIPSTART=4,\"TCP\",\"192.168.4.2\",3333");
  delay(200);
  Serial1.println("AT+CIPSEND=4,6");
  delay(200);
  Serial1.print("pw"); Serial1.print(pwid); Serial1.print("du"); 
  if(dur>=0 && dur<=9) Serial1.println(dur);
  else Serial1.println(char(dur));
  delay(60);
  Serial1.println("AT+CIPCLOSE=4");
  delay(20);
  digitalWrite(led_pin, LOW);  
}
/////////////////////////////////////////////////////////////////////////////////////
void El()
{
  int el_min = 20;
  int el_max = 170;
  if(rcvstr.substring(3,6).toInt()>el_max) myservo.write(el_max);
  else if(rcvstr.substring(3,6).toInt()<el_min) myservo.write(el_min);
  else myservo.write(rcvstr.substring(3,6).toInt());
}

void ElSCAN()
{
  MySerial.read();
  while(MySerial.available()<1)
  {
    for(pos = 77; pos <= 103; pos += 1) 
      {                                  
        myservo.write(pos);             
        delay(100);                   
      } 
    for(pos = 103; pos >= 77; pos -= 1) 
      {                                  
        myservo.write(pos);             
        delay(100);                   
      }
  }         
}

void hd2(float heading, float elev)
{
  //Ready:>hd2 HHH EEE
  float cur_heading, cur_elev;
  float err_heading, err_elev;
  int next_servo_pos = 92;
  float cal_mx, cal_my;
  
  MySerial.println();
  MySerial.println(heading);
  MySerial.println(elev);
  
  //set heading and elevation limits
  if(elev < -12) elev = 12;
  if(elev > 12) elev = 12;
  if(heading < 0) heading = 0;
  if(heading > 359) heading = 359;

  MySerial.println();
  MySerial.println(heading);
  MySerial.println(elev);

  //set elevation first
  //get current elevation
  imu.readAccel();
  cur_elev = atan2(imu.ay, imu.az)*180.0/PI;
  MySerial.println(cur_elev);
  err_elev = elev - cur_elev;
  MySerial.println(err_elev);
  next_servo_pos = int(myservo.read());
  //start PID loop
  while(abs(err_elev) > 2.0)
  {
    if(err_elev > 0) 
    {
      next_servo_pos ++;
      if(int(myservo.read()) < 170) {myservo.write(next_servo_pos);}
      if(int(myservo.read()) >= 170) {myservo.write(170); MySerial.println("high limit"); break;}
    }
    if(err_elev < 0) 
    {
      next_servo_pos --;
      if(int(myservo.read()) > 20) {myservo.write(next_servo_pos);}
      if(int(myservo.read()) <= 20) {myservo.write(170); MySerial.println("low limit"); break; }
    }
    
    delay(50);
    imu.readAccel();
    cur_elev = atan2(imu.ay, imu.az)*180.0/PI;
    err_elev = elev - cur_elev;
    MySerial.println(err_elev); 
  }

  //now move to heading
  //get current heading
  imu.readMag();
  cal_mx = -(imu.mx - mxoffset)*mxscale;
  cal_my = -(imu.my - mxoffset)*myscale;
  if (cal_my == 0)
    cur_heading = (cal_mx < 0) ? 180.0 : 0;
  else
    cur_heading = atan2(cal_mx, cal_my)*180.0/PI;
  //if(cur_heading < 0) cur_heading = 360 + cur_heading;
  cur_heading = -1*cur_heading+180;
  MySerial.print("starting heading "); MySerial.println(cur_heading);
  //subtract target heading from current heading
  err_heading = heading - cur_heading;
  //find shorter move
  if(abs(err_heading) > 180) err_heading = -1.0*(360.0 - err_heading);
  
  //start PID loop
  while(abs(err_heading) > 2.0)
  {
    if(err_heading > 0 && err_heading <= 45) Az_move(9,1);    //little move clockwise
    if(err_heading < 0 && err_heading >= -45) Az_move(4,1);   //little move counter clockwise
    //if(err_heading > 25 && err_heading <= 45) Az_move(7,4);  //medium move clockwise
    //if(err_heading < -25 && err_heading >= -45) Az_move(2,4); //medium move counter clockwise
    if(err_heading > 45) Az_move(9,4);     //big move clockwise
    if(err_heading < -45) Az_move(4,4);    //bige move counter clockwise
    
    //wait until move is done
    imu.readGyro();
    while(abs(imu.gz) > 500) 
    {
      imu.readGyro();
      MySerial.print("gyro "); MySerial.println(imu.gz);
      delay(50);
    }
    
    //get updated heading
    imu.readMag();
    cal_mx = -(imu.mx - mxoffset)*mxscale;
    cal_my = -(imu.my - mxoffset)*myscale;
    if (cal_my == 0)
      cur_heading = (cal_mx < 0) ? 180.0 : 0;
    else
      cur_heading = atan2(cal_mx, cal_my)*180.0/PI;
    //if(cur_heading < 0) cur_heading = 360 + cur_heading;
    cur_heading = -1*cur_heading+180;;
    MySerial.println(cur_heading);  
    //subtract target heading from current heading
    err_heading = heading - cur_heading;
  }
  
  //move done
  //clear ESP8266 Serial1 buffer
  while(Serial1.available()>0) Serial1.read();
}
///////////////////////////////////////////////////////////////////////////////////////


