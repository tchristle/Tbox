void check_uart(){
  if (Serial.available ())
  {
    incomingByte = Serial.read();
    rcvstr += incomingByte;
    if (incomingByte==127 && rcvstr.length()>1) rcvstr = rcvstr.substring(0,rcvstr.length()-2);
    if (incomingByte != 13 || incomingByte != 10) Serial.print(incomingByte);
    if (incomingByte == 27) {rcvstr = last_rcvstr; Serial.print(rcvstr); }
    if (incomingByte == 13 || incomingByte == 10)
      {
        rcvstr.trim(); 
        Serial.println();
        command_lookup(rcvstr); 
        Serial.print("ESP:>");
        last_rcvstr = rcvstr;
        rcvstr = "";
      }
  }
}
