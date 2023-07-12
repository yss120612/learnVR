#include "Uart2Task.h"

void Uart2Task::setup(){

}
void Uart2Task::cleanup(){
    
}

void Uart2Task::loop(){

  int len=0,i=0;
  char buff[BUFFER_SIZE];
  //len=Serial2.read(buff,1);
  len=Serial2.readBytesUntil('0xAA',buff,BUFFER_SIZE);
//   if (len>0 && buff[0]==0xAA){
//      do {
//      i++;   
//      vTaskDelay(pdTICKS_TO_MS(10));
//      Serial2.read(buff+i,1); 
//      } while (buff[i]!=0x0A);
//      len=i+1;
//   }else{
//     Serial2.flush(false);g
//     len=0;
//     buff[0]=0;
//   }
  uint8_t cnt;
  bool valid=false;
  if (len>0){
  valid = buff[0]==0xAA && buff[1]>1;
  
    lock();
    if (valid){
    //Serial.println("Valid command!");                    
    switch (buff[2]) 
    {
        case 0:
            if (buff[1]!=8) {
                Serial.println("Error Read status!");            
                break;
            }
            lock();
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
             unlock();
        break;
         case 1:
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

        case 2:
        Serial.println("Check Record train status");        
        Serial.print("Trained recirds:");    
        cnt=buff[3];
        Serial.println(cnt,HEX);    
        for (int j=0;j<cnt;j++){
            Serial.printf("R %02d:%s\n",buff[4+j*2],buff[4+j*2+1]==0?"UT":buff[4+j*2+1]==1?"TR":"ERR");
        }
        break;

        case 3:
        Serial.print("Signature of one Record #");    
        Serial.print(buff[3],DEC);    
        Serial.print(" Lenngth=");
        cnt=buff[4];
        Serial.print(cnt,DEC);
        //Serial.print(" Lmes=");
        // Serial.print(buff[1]);
        Serial.print(" ");
        buff[cnt+5]=0;
        //for (int j=0;j<cnt;j++){
        //    Serial.print(buff[j+5]);    
        //}
        Serial.println((char *)buff+5);
        break;
        case '\x10':
           if (buff[3]==0) {
            Serial.println("System Settings restored!");        
           }
        break;    
        case '\x12':
            Serial.println("Mode updated.");        
            // switch (buff[3]){
            //     case 0:
            //     Serial.println("Pulse.");        
            //     break;
            //     case 1:
            //     Serial.println("Flip.");        
            //     break;
            //     case 2:
            //     Serial.println("Up.");        
            //     break;
            //     case 3:
            //     Serial.println("Down.");        
            //     break;
            // }
        break;
         case '\x13':
            Serial.println("Pulse period updated.");        
            
        break;
        case '\x15':
            /*
            Set Power On Auto Load (15)
            Use this command to enable or disable "Power On Auto Load" function.
            | AA| 05+n | 15 | 00 |BITMAP | R0 | ... | Rn | 0A | (set auto load)

            BITMAP Record bitmap:
            ● 00 -- zero record, disable auto load
            ● 01 -- one record
            ● 03 -- two records
            ● 07 -- three records
            ● 0F -- four records
            ● 1F -- five records
            ● 3F -- six record
            ● 7F -- seven records
            R0~Rn Record
            */
            if (buff[4]==0){
                Serial.println("AutoLoad DISABLED");
            }else{
                Serial.println("AutoLoad ENABLED");
                for (i=0;i<buff[1]-4;i++)
                {
                    Serial.print(i,DEC);
                    Serial.print(":");
                    Serial.println(buff[5+i],DEC);
                }
            }
        break;

        case '\x0A':
            
            switch (buff[10]){
                case 'n':
                 Serial.println("Speak now");
                break;
                case 'a':
                Serial.println("Speak again");
                break;
                case 's':
                Serial.print("Record ");
                Serial.print(buff[3],DEC);
                Serial.println(" Success");
                break;
            }
        break;
        case '\x21':
            Serial.print("Success records trained N:");
            Serial.print(buff[3],DEC);
            Serial.print(" RECORD:");
            Serial.println(buff[4],DEC);
        break;
        case '\x30':
        /*
        | AA| 3+2n | 30 | N | R0 | STA0 | ... | Rn | STAn | 0A |
        R0~Rn Voice Record index
        STA0~STAn Load result
            ● 00 -- Success
            ● FF -- Record value out of range
            ● FE -- Record untrained
            ● FD -- Recognizer full
            ● FC -- Record already in recognizer
            N Number of successful training voice commands
        */
        Serial.print("Records processed:");
        Serial.println(buff[3]);           
        for (i=0;i<buff[3];i++){
            Serial.print("Record ");
            Serial.print(buff[4+i*2],DEC);
            Serial.print(":");
            Serial.println(buff[5+i*2],HEX);
        }
        break;
        case '\x31':
            Serial.println("Recogniser cleared");
        break;
        case '\x32':
        
        if (buff[1]==3){
            Serial.println("Group selected");
        }
        else{
            Serial.print("Group is ");
            Serial.println(buff[5],DEC);
        }
        
        break;
        case '\xFF':
        /*
        | AA | 03 | FF | ECODE | 0A |
        ECODE error code
            ● FF -- command undefined
            ● FE -- command length error
            ● FD -- data error
            ● FC -- subcommand error
            ● FB -- command usage error
        */
           Serial.print("ERROR: ");
        switch (buff[3]){
            case 0xFB:
            Serial.println("command usage error");
            break;
            case 0xFC:
            Serial.println("subcommand error");
            break;
            case 0xFD:
            Serial.println("data error");
            break;
            case 0xFE:
            Serial.println("command length error");
            break;
            case 0xFF:
            Serial.println("command undefined");
            break;
            default:
            Serial.println("other error");
            break;
        }
        break;
        case 0x0D:
           /*
           Voice Recognized (0D)
           | AA | 07 | 0D | 00 | GRPM | R | RI | SIGLEN | SIG | 0A |
            GRPM Group mode indicate
                ● FF: not in group mode
                ● 00~0A: system group mode
                ● 80~87: user group mode
            R record which is recognized.
            RI recognizer index value for recognized record.
            SIGLEN signature length of the recognized record, 0 means on signature, on SIG area
            SIG signature content
            */ 
        Serial.print("Group mode:");    
        Serial.println(buff[4],HEX);
        Serial.print("Record which is recognized:");
        Serial.println(buff[5],HEX);
        Serial.print("Recognizer index:");
        Serial.println(buff[6],HEX);
        Serial.print("Description:");
        if (buff[7]>0){
            buff[8+buff[7]]=0;   
            Serial.println((char *)buff+8);
        }else{
            Serial.println("NONE");
        }

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

