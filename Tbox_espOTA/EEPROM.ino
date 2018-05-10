/*
 *  0x00 <len> ssid_STA
 *  0x40 <len> password_STA
 *  0x80 <len> ssid_AP
 *  0xc0 <len> password_AP
 *  0x100 <len> node_id
 *  0x120 net_mode host/node
 *  0x140 start script
 *  .
 *  .
 *  .
 *  0x400 end
 *
 */
void EEPROM_setup(){

  EEPROM.begin(1024);
  //put_eeprom_str(node_id, "charlie");
  //put_eeprom_str(net_mode, "node");
  //put_eeprom_str(ssid_STA, "NETGEAR11");
  //put_eeprom_str(password_STA, "niftypotato037");
  //put_eeprom_str(ssid_AP, "targy1");
  //put_eeprom_str(password_AP, "12345678");
  Serial.print("node_id: ");Serial.println(get_eeprom_str(node_id));
  Serial.print("net_mode: ");Serial.println(get_eeprom_str(net_mode));
  Serial.print("ssid_STA: ");Serial.println(get_eeprom_str(ssid_STA));
  Serial.print("password_STA: ");Serial.println(get_eeprom_str(password_STA));
  Serial.print("ssid_AP: ");Serial.println(get_eeprom_str(ssid_AP));
  Serial.print("password_AP: ");Serial.println(get_eeprom_str(password_AP));  
  Serial.print("start script: ");Serial.println(get_eeprom_str(starts));  
  
  node_id_c = get_eeprom_str(node_id);
  net_mode_c = get_eeprom_str(net_mode);
  ssid_STA_c = get_eeprom_str(ssid_STA);
  password_STA_c = get_eeprom_str(password_STA);
  ssid_AP_c = get_eeprom_str(ssid_AP);
  password_STA_c = get_eeprom_str(password_STA);
  strtscr = get_eeprom_str(starts);
  
  //EEPROM.end();
  
}
//--------------------------------------
void GenEE(){
  put_eeprom_str(node_id, "charlie");
  put_eeprom_str(net_mode, "node");
  put_eeprom_str(ssid_STA, "NETGEAR11");
  put_eeprom_str(password_STA, "niftypotato037");
  put_eeprom_str(ssid_AP, "targy1");
  put_eeprom_str(password_AP, "12345678");
  put_eeprom_str(starts, " ");
}
 
//--------------------------------------

String get_eeprom_str(int ADDR){
  int j=0, k=0;
  String str="";
  j = EEPROM.read(ADDR);
  while(k<j+1){
    str += char(EEPROM.read(ADDR+k+1));
    //str[k] = EEPROM.read(ADDR+k+1);
    k++;
  }
  return str;
 }
//--------------------------------------
void put_eeprom_str(int ADDR, String str){
  int j=0, k=0;
  j = str.length();
  //Serial.println(j);
  EEPROM.write(ADDR, j);
  while(k<(j+1)){
    EEPROM.write(ADDR+k+1, str[k]);
    //Serial.println(str[k]);
    k++;
  }
  EEPROM.commit();
 }
//---------------------------------------------
void STAid(){
  String txt = rcvstr.substring(5+1); 
  Serial.print('\n');
  Serial.println(txt);
  put_eeprom_str(ssid_STA, txt.c_str());
}
//---------------------------------------------
void STApw(){
  String txt = rcvstr.substring(5+1); 
  Serial.print('\n');
  Serial.println(txt);
  put_eeprom_str(password_STA, txt.c_str());
}
//---------------------------------------------
void APid(){
  String txt = rcvstr.substring(4+1); 
  Serial.print('\n');
  Serial.println(txt);
  put_eeprom_str(ssid_AP, txt.c_str());
}
//---------------------------------------------
void APpw(){
  String txt = rcvstr.substring(4+1); 
  Serial.print('\n');
  Serial.println(txt);
  put_eeprom_str(password_AP, txt.c_str());
}
//---------------------------------------------
void NODid(){
  String txt = rcvstr.substring(5+1); 
  Serial.print('\n');
  Serial.println(txt);
  put_eeprom_str(node_id, txt.c_str());
}
//---------------------------------------------
void setNODE(){
  Serial.print('\n');
  put_eeprom_str(net_mode, "node");
}
//---------------------------------------------
void setHOST(){
  Serial.print('\n');
  put_eeprom_str(net_mode, "host");
}
//---------------------------------------------
void setStart(){
  String txt = rcvstr.substring(7+1);
  Serial.print('\n');
  put_eeprom_str(starts, txt.c_str());
}
