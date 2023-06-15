#include "Uart2Task.h"

void Uart2Task::setup(){

}

void Uart2Task::loop(){

  int len=0;
  char buff[BUFFER_SIZE];
  len=Serial2.read(buff,BUFFER_SIZE);
  uint8_t cnt;
  bool valid=false;
  if (len>0){
  valid = buff[0]==0xAA && buff[1]>1 && buff[len-1]==0x0A;
  
    lock();
    if (valid){
    //Serial.println("Valid command!");                    
    switch (buff[2]) 
    {
        case '\x00':
            if (buff[1]!=8) {
                Serial.println("Error Read status!");            
                break;
            }
            Serial.println("Check System Settings");        
            Serial.print("Trained:");
             switch (buff[3]){
                 case 0:
                 Serial.println("untrained.");        
                 break;
                 case 1:
                 Serial.println("trained.");        
                 break;
                 case 0xff:
                 Serial.println("out of range.");
                 break;
             };

             Serial.print("Baud rate:");
             switch (buff[4]){
                 case 0:
                 case 3:
                 Serial.println("9600.");        
                 break;
                 case 1:
                 Serial.println("2400.");        
                 break;
                 case 2:
                 Serial.println("4800.");        
                 break;
                 case 4:
                 Serial.println("19200.");        
                 break;
                 case 5:
                 Serial.println("38400.");        
                 break;
             }
             Serial.print("IO Mode:");
             switch (buff[5]){
                 case 0:
                 Serial.println("Pulse.");        
                 break;
                 case 1:
                 Serial.println("Toggle.");        
                 break;
                 case 2:
                 Serial.println("Clear.");        
                 break;
                 case 3:
                 Serial.println("Set.");        
                 break;
             }
             Serial.print("Pulse Width:");        
             switch (buff[6]){
                 case 0:
                 Serial.println("10ms");        
                 break;
                 case 1:
                 Serial.println("15ms");        
                 break;
                 case 2:
                 Serial.println("20ms");        
                 break;
                 case 3:
                 Serial.println("25ms");        
                 break;
                 case 4:
                 Serial.println("30ms");        
                 break;
                 case 5:
                 Serial.println("35ms");        
                 break;
                 case 6:
                 Serial.println("40ms");        
                 break;
                 case 7:
                 Serial.println("45ms");        
                 break;
                 case 8:
                 Serial.println("50ms");        
                 break;
                 case 9:
                 Serial.println("75ms");        
                 break;
                 case 10:
                 Serial.println("100ms");        
                 break;
                 case 11:
                 Serial.println("200ms");        
                 break;
                 case 12:
                 Serial.println("300ms");        
                 break;
                 case 13:
                 Serial.println("400ms");        
                 break;
                 case 14:
                 Serial.println("500ms");        
                 break;
                 case 15:
                 Serial.println("1s");        
                 break;
             }
             Serial.print("Group control:");        
              switch (buff[8]){
                 case 0:
                 Serial.println("disable");        
                 break;
                 case 1:
                 Serial.println("system");        
                 break;
                 case 2:
                 Serial.println("user");          
                 break;
              }

             Serial.print("Auto load:");
             
             switch (buff[7]){
                 case 0:
                 Serial.println("DISABLE");        
                 break;
                 case 1:
                 Serial.println("ENABLE");        
                 break;   
                    default:
                 Serial.println(buff[7],HEX);        
                 break;
             }
             
        break;
         case '\x01':
            if (buff[1]!=0xD) {
                Serial.println("Error check recogniser!");            
                break;
            }
            Serial.println("Check Recognizer");        
            Serial.print("Total commands:");
            Serial.println(buff[3],DEC);        
            Serial.print("Commands:");
            for (int8_t j=0;j<7;j++) {
            Serial.print(buff[4+j],DEC);
            if (j==6) Serial.println(); else Serial.print(", ");
            }
            Serial.print("Total records:");
            Serial.println(buff[11],DEC);    
            Serial.print("Valid comm. bitmap:");
            Serial.println(buff[12],DEC);  
             Serial.print("Group mode:");
            Serial.println(buff[13],HEX);  
            break;

        case '\x02':
        Serial.println("Check Record train status");        
        Serial.print("Trained recirds:");    
        cnt=buff[3];
        Serial.println(cnt,HEX);    
        for (int j=0;j<cnt;j++){
            Serial.printf("R %02d:%s\n",buff[4+j*2],buff[4+j*2+1]==0?"TR":buff[4+j*2+1]==1?"UT":"ERR");
        }
        break;

        case '\x03':
        Serial.print("Signature of one Record #");    
        cnt=buff[3];
        Serial.print(cnt,DEC);    
        for (int j=0;j<cnt;j++){
            Serial.print(buff[j]);    
        }
        Serial.println("");
        break;
        
        case '\x12':
            Serial.print("Mode updated.");        
            // switch (buff[3]){
            //     case 0:
            //     Serial.println("Pulse");        
            //     break;
            //     case 1:
            //     Serial.println("Flip");        
            //     break;
            //     case 2:
            //     Serial.println("Up");        
            //     break;
            //     case 3:
            //     Serial.println("Down");        
            //     break;
            // }
        break;
        default:
            Serial.println("Other valid command!");            
            printBuff(buff,len);
        break;
    }
    }
    else{
    Serial.println("Invalid command!");            
    printBuff(buff,len);
    }
    unlock();
    
    }
    
 
    
    vTaskDelay(1);
    

}

void Uart2Task::printBuff(char * buff, uint16_t length){
    Serial.print("Length=");
        Serial.println(length);
            for (int j=0;j<length;j++)
            {
            if ((buff[j])<=16) Serial.print('0');
            Serial.print(buff[j],HEX);
            if (j<length-1) Serial.print("|");
            else Serial.println(".");
            }

}

void Uart2Task::cleanup(){
    
}