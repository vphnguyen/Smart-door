#include <TIMER1_SERVO.h>

#include <EEPROM.h>

#include <Keypad.h>
 
 
const byte rows = 4; //số hàng
const byte columns = 4; //số cột
const byte ON = HIGH;
const byte OFF = LOW;
 const byte hangmode = 1;
 const byte readmode = 2;
 const byte changemode = 3;
  const byte ledred=11;
  const byte ledblue=12;
  const byte speaker=13;
 byte mode;
 bool passstatus;
 ///////////////////////////// KEYBOARD
int holdDelay = 700; //Thời gian trễ để xem là nhấn 1 nút nhằm tránh nhiễu
int n = 3; // 
int state = 0; //nếu state =0 ko nhấn,state =1 nhấn thời gian nhỏ , state = 2 nhấn giữ lâu

 
//Định nghĩa các giá trị trả về
char keys[rows][columns] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};
char pass[4],pass1[4],pass2[4];
char key = 0; 
byte rowPins[rows] = {9,8,7,6}; //Cách nối chân với Arduino
byte columnPins[columns] = {5,4,3,2};
 
//cài đặt thư viện keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);
void setup() {
  mode=1;
//  EEPROM.write(0,'1');
//  EEPROM.write(1,'2');
//  EEPROM.write(2,'3');
//  EEPROM.write(3,'4');
  pinMode(ledred,OUTPUT);
  pinMode(ledblue,OUTPUT);
  pinMode(speaker,OUTPUT);
  port_attach(PB2);
  Serial.begin(9600);//bật serial, baudrate 9600
  Serial.println(EEPROM.read(0));
  Serial.println(EEPROM.read(1));
  Serial.println(EEPROM.read(2));
  Serial.println(EEPROM.read(3));
  correct_write(PB2,map(80,0,180,8480,38550));
}
bool checkpass(char pass[]){
  if(pass[0]!=EEPROM.read(0))return false;
  if(pass[1]!=EEPROM.read(1))return false;
  if(pass[2]!=EEPROM.read(2))return false;
  if(pass[3]!=EEPROM.read(3))return false;
  return true;
}
bool recheck(char p1[], char p2[]){
  if(p1[0]!=p2[0])return false;
  if(p1[1]!=p2[1])return false;
  if(p1[2]!=p2[2])return false;
  if(p1[3]!=p2[3])return false;
  return true;
}
unsigned long now(){
  return millis();;
}
void speaker_double(){
     //INTRO
        digitalWrite(speaker,ON);
        delay(100);
        digitalWrite(speaker,OFF);
        delay(50);
        digitalWrite(speaker,ON);
        delay(100);
        digitalWrite(speaker,OFF);
}
void speaker_triple(){
     //INTRO
        digitalWrite(speaker,ON);
        delay(100);
        digitalWrite(speaker,OFF);
        
        delay(50);
        
        digitalWrite(speaker,ON);
        delay(100);
        digitalWrite(speaker,OFF);
        
        delay(50);
        
        digitalWrite(speaker,ON);
        delay(100);
        digitalWrite(speaker,OFF);
}
void speaker_long_triple(){
     //INTRO
        digitalWrite(speaker,ON);
        delay(200);
        digitalWrite(speaker,OFF);
        delay(50);
        digitalWrite(speaker,ON);
        delay(200);
        digitalWrite(speaker,OFF);
        delay(50);
        digitalWrite(speaker,ON);
        delay(200);
        digitalWrite(speaker,OFF);
}
int getK_status(){
  if(analogRead(A0)<=512)return 0;
  else return 1;
}
void loop() { 
  Serial.println("====== LOOP =====");
if(mode ==hangmode)  {
  delay(1000);
  correct_write(PB2,map(80,0,180,8480,38550));
  digitalWrite(speaker,ON);
  delay(100);
  digitalWrite(speaker,OFF);
  digitalWrite(ledred,OFF);
  digitalWrite(ledblue,OFF);
  Serial.println("hang");
                while(true){
                    char temp = keypad.getKey();
                    if ((int)keypad.getState() ==  PRESSED) {
                    if (temp != 0) {
                      key = temp;
                       if(key =='A')digitalWrite(ledred,ON);
                    }
                  }
                  if ((int)keypad.getState() ==  HOLD) {
                    state++;
                    state = constrain(state, 1, n-1);
                    delay(holdDelay);
                  }
                 
                  if ((int)keypad.getState() ==  RELEASED) {
                    
                    state = 0;
                    //Xuất lên Máy tính để xem kết quả
                    Serial.print("===[HANG]   :");
                    Serial.println(key);
                 if(key =='A'){
                                digitalWrite(ledred,OFF);
                                mode =readmode;
                                break;
                              }
                  }
                  delay(100);    
              }
}
delay(100);
  if(mode ==readmode)  {
  digitalWrite(ledblue,ON);
  speaker_double();
  Serial.println("read mode");
  byte count =0;
                     while(count <4){
                      char temp= keypad.getKey();
                    if ((int)keypad.getState() ==  PRESSED) {
                    if (temp != 0) {
                      digitalWrite(ledred,ON);
                      digitalWrite(speaker,ON);
                      key = temp;
                    }
                  }
                  if ((int)keypad.getState() ==  HOLD) {
                    state++;
                    state = constrain(state, 1, n-1);
                    delay(holdDelay);
                    digitalWrite(speaker,OFF);
                    if(key =='A'){
                                    mode =hangmode;
                                    break;
                                  }
                  }
                  if ((int)keypad.getState() ==  RELEASED) {
                    state = 0;
                    if(key=='C') count =0; 
                    digitalWrite(ledred,OFF);
                    digitalWrite(speaker,OFF);
                    Serial.print("===[READ]   :");
                    pass[count]=key;
                    count ++;
                    Serial.print(pass[count -1]);
                    Serial.print(" -> Count:");
                    Serial.println(count); 
                    
                     
                     if(key =='C'){
                                    count =0;pass[0]=NULL;pass[1]=NULL;pass[2]=NULL;pass[3]=NULL;
                                    digitalWrite(speaker,ON);
                                    delay(500);
                                    digitalWrite(speaker,OFF);
                                    //break;
                                  }
                      if(key =='B'){
                        break;
                      }
                  }
                  delay(100);
                  }
                  ///
              if(checkpass(pass)==false || count <4){
                Serial.println("==== WRONG PASS ====");
                speaker_long_triple();
                 mode =hangmode;
                 count =0;
              }else{
                speaker_triple();
                delay(500);
                Serial.println("==== OKE ! OPENING LOCK ====");
                for(int i=80;i>=0;i--){
                  correct_write(PB2,map(i,0,180,8480,38550));
                  delay(15);
                }
                delay(2000);
                

                Serial.println("====     DOOR OPENED   ====");
                digitalWrite(ledred,ON);
                unsigned long mok=now();
                Serial.println(mok);
                Serial.println(now());
                while(true){
                  if((unsigned long)(now()-mok)>10000)break;
                  else {
                    Serial.print((unsigned long)(now()-mok));
                    Serial.println(" :LOOPPPPPPING~~~~~");
                                char temp= keypad.getKey();
                                if ((int)keypad.getState() ==  PRESSED) {
                                      if (temp != 0) {
                                        digitalWrite(ledred,ON);
                                        key = temp;
                                      }
                              }
                              if ((int)keypad.getState() ==  RELEASED) {
                                state = 0;
                                Serial.println(key);
                                 if(key =='D'){
                                                digitalWrite(ledred,OFF);
                                                mode =changemode;
                                                break;
                                              }
                                  }
                                if(getK_status()==0)  {
                                  Serial.println("==== OKE ! CLOSING LOCK ====");
                                    
                                    for(int i=0;i<=80;i++){
                                      correct_write(PB2,map(i,0,180,8480,38550));
                                      delay(15);
                                    }
                                    mode =changemode;
                                    break;
                                }
                                
                              }
                  }
                }
               
               digitalWrite(ledred,OFF);
               if(mode!=changemode)mode =hangmode; 
              
  
                  ///
  }
   delay(100);
   if(mode ==changemode){
    digitalWrite(speaker,OFF);
    digitalWrite(ledred,ON);
    digitalWrite(ledblue,OFF);
    Serial.println("Change mode");
    speaker_double();
    byte count1 =0;
    byte count2 =0;
    //////// Step1
               if(mode ==changemode)
               while(count1 <4){
                      char temp= keypad.getKey();
                    if ((int)keypad.getState() ==  PRESSED) {
                    if (temp != 0) {
                      digitalWrite(ledblue,ON);
                      digitalWrite(speaker,ON);
                      key = temp;
                    }
                  }
                  if ((int)keypad.getState() ==  HOLD) {
                    state++;
                    state = constrain(state, 1, n-1);
                    delay(holdDelay);
                    digitalWrite(speaker,OFF);
                    if(key =='A'){
                                    mode =hangmode;
                                    break;
                                  }
                  }
                  if ((int)keypad.getState() ==  RELEASED) {
                    state = 0;
                    if(key=='C') count1 =0; 
                    digitalWrite(ledblue,OFF);
                    digitalWrite(speaker,OFF);
                    Serial.print("===[CHANGE 1]   :");
                    pass1[count1]=key;
                    count1 ++;
                    Serial.print(pass1[count1 -1]);
                    Serial.print(" -> Count 1:");
                    Serial.println(count1); 
                     if(key =='C'){
                                    count1 =0;pass1[0]=NULL;pass1[1]=NULL;pass1[2]=NULL;pass1[3]=NULL;
                                    digitalWrite(speaker,ON);
                                    delay(500);
                                    digitalWrite(speaker,OFF);
                                    //break;
                                  }
                      if(key =='B'){
                        break;
                      }
                  }
                  delay(100);
                  }
     ///////// Step2  
     speaker_double();           
                  if(mode ==changemode)
                  while(count2 <4){
                      char temp= keypad.getKey();
                    if ((int)keypad.getState() ==  PRESSED) {
                    if (temp != 0) {
                      digitalWrite(ledblue,ON);
                      digitalWrite(speaker,ON);
                      key = temp;
                    }
                  }
                  if ((int)keypad.getState() ==  HOLD) {
                    state++;
                    state = constrain(state, 1, n-1);
                    delay(holdDelay);
                    digitalWrite(speaker,OFF);
                    if(key =='A'){
                                    mode =hangmode;
                                    break;
                                  }
                  }
                  if ((int)keypad.getState() ==  RELEASED) {
                    state = 0;
                    if(key=='C') count2 =0; 
                    digitalWrite(ledblue,OFF);
                    digitalWrite(speaker,OFF);
                    Serial.print("===[CHANGE 2]   :");
                    pass2[count2]=key;
                    count2 ++;
                    Serial.print(pass2[count2 -1]);
                    Serial.print(" -> Count 2: ");
                    Serial.println(count2); 
                    
                     
                     if(key =='C'){
                                    count2 =0;pass2[0]=NULL;pass2[1]=NULL;pass2[2]=NULL;pass2[3]=NULL;
                                    digitalWrite(speaker,ON);
                                    delay(500);
                                    digitalWrite(speaker,OFF);
                                    //break;
                                  }
                      if(key =='B'){
                        break;
                      }
                  }
                  delay(100);
                  }  
                  
   //////////check
    if(recheck(pass1,pass2)==true) {


      ///EEPROM...............
      EEPROM.write(0,pass1[0]);
      EEPROM.write(1,pass1[1]);
      EEPROM.write(2,pass1[2]);
      EEPROM.write(3,pass1[3]);
      Serial.println("+++++++++++ [CHANGED] +++++++++++++++"); 
    
    mode=hangmode;          
   }
   delay(100);
}
