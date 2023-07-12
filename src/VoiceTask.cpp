#include "VoiceTask.h"

void VoiceTask::setup(){
cmd_cnt=0;
}
void VoiceTask::cleanup(){
    
}

void VoiceTask::loop()
{
  int len1;
 char bf[100];
  len1=receiveCMD();
  uint8_t i,l;
  notify_t notify;
  if(len1>0){
    lock();
    //String st;
    switch (cmd[0]){
      case 's'://system status
      //bf="\xAA\x02\x00\x0A";
      Serial2.write("\xAA\x02\x00\x0A",4);
      break;
      case 'r'://check recognizer
      Serial2.write("\xAA\x02\x01\x0A",4);
      break;
      case 't'://check trained status
      Serial2.write("\xAA\x03\x02\xFF\x0A",5);
      break;
      case 'g'://Check Signature Record
      
      if (len1<4 || len1>5) break;
      
      //Serial.println((char *)cmd);
      bf[0]=0xAA;
      bf[1]=0x03;
      bf[2]=0x03;
      bf[3]=cmd[1]-'0';
      if (len1>4){
        bf[3]=bf[3]*10+cmd[2]-'0';
      }
      bf[4]=0x0A;
      Serial2.write(bf,5);
      break;
      case 'R':
      if (cmd[1]='D'){
      Serial2.write("\xAA\x02\x10\x0A",5);
      }
      break;
      case 'm':/*Set Output IO Mode 
      0 -- Pulse mode 
      1 -- Flip mode  
      2 -- Up mode  
      3 -- Down mode
      */
      //if (len1!=7) break;
      bf[0]=0xAA;
      bf[1]=0x03;
      bf[2]=0x12;
      bf[3]=cmd[1]-'0';
      bf[4]=0x0A;
      Serial2.write(bf,5);
      //Serial2.write("\xAA\x03\x12\x01\x0A",6);
      break;
      case 'L':
      /*
      | AA| 3+n | 30 | R0 | ... | Rn | 0A |
      R0~Rn Voice Record index 
      RX-TWO DECIMAL DIGITS
      */
      l=(len1-3)/2;
      if (l<1|| l>7){
      Serial.print("Wrong lenght command l=");
      Serial.println(l);
      }
      bf[0]=0xAA;
      bf[1]=2+l;
      bf[2]=0x30;
      for (i=0;i<l;i++) bf[3+i]=(cmd[1+i*2]-'0')*10+cmd[2+i*2]-'0';
      bf[bf[1]+1]=0x0A;
      Serial2.write(bf,bf[1]+2);
     break;
      case 'G':/*New group control mode.
      ● 0-disable
      ● 1-system
      ● 2-user
      ● 3-check */
      bf[0]=0xAA;
      bf[1]=0x04;
      bf[2]=0x32;
      bf[3]=0;
      bf[4]=cmd[1]-'0';
      if (bf[4]>=3){
      bf[4]=255;
      };
     
      bf[5]=0x0A;
      Serial2.write(bf,6);
      break;
      
      case 'a':
      /*Set Power On Auto Load
      | AA| 04+n | 15 | BITMAP | R0 | ... | Rn | 0A |
      BITMAP
      00 -- zero record, disable auto load
      01 -- one record
      03 -- two records
      07 -- three records
      0F -- four records
      1F -- five records
      3F -- six record
      7F -- seven records
      */
      l=(len1-3)/2;
      
      if (l>7){
      Serial.print("Wrong lenght command l=");
      Serial.println(l);
      break;
      }

      bf[0]=0xAA;
      bf[1]=3+l;
      bf[2]=0x15;
      bf[3]=l==0?0:(1<<l)-1;
      if (l>0){
      for (i=0;i<l;i++) bf[4+i]=(cmd[1+i*2]-'0')*10+cmd[2+i*2]-'0';
      }
      bf[bf[1]+1]=0x0A;
      Serial2.write(bf,bf[1]+2);
     break;
      case 'C':
      Serial2.write("\xAA\x02\x31\x0A",4);//clear recogniser
      break;
      case 'v':
      /*
      Train One Record and Set Signature (21)
      Train one record and set a signature for it, one record one time.
      Format:
      | AA| 03+SIGLEN | 21 | RECORD | SIG | 0A | (Set signature)
      syntax vXSSS
      where xx N of record
      SSS signature
      */
      
      bf[0]=0xAA;
      bf[1]=3+len1-5;
      bf[2]=0x21;
      bf[3]=(cmd[1]-'0')*10+(cmd[2]-'0');
      memcpy((void*)(bf+4),(void*)(cmd+3),len1-5);
      bf[bf[1]+1]=0x0A;  
      //  Serial.print("Len=");
      //  Serial.println(len1-5,DEC);
      //  Serial.print("Sign=");
      //  for (i=0;i<len1-5;i++) Serial.print(bf[i+4]);
      //  Serial.println("|");
      //  for (i=0;i<=bf[1]+1;i++) {
      //  Serial.print(bf[i],HEX);
      //  Sverial.print("|");
      //  }
      Serial2.write(bf,bf[1]+2);
      break;
    case 'P':
      /* Set pulse period
      - 00 10ms
- 01 15ms
- 02 20ms
- 03 25ms
- 04 30ms
- 05 35ms
- 06 40ms
- 07 45ms
- 08 50ms
- 09 75ms
- 10 100ms
- 11 200ms
- 12 300ms
- 13 400ms
- 14 500ms
- 15 1s

      | AA| 03 | 13 | XX | 0A | (Set pulse period)
      
      where XX pulse period
      
      */
      
      bf[0]=0xAA;
      bf[1]=3;
      bf[2]=0x13;
      bf[3]=(cmd[1]-'0')*10+(cmd[2]-'0');
      bf[4]=0x0A;  
      Serial2.write(bf,bf[1]+2);
      break;
    }
   unlock();
  }
  vTaskDelay(10);
    
}

int VoiceTask::receiveCMD()
{
  int ret;
  int len;
  unsigned long start_millis;
  start_millis = millis();
  while(1){
    ret = Serial.read();
    if(ret>0){
      start_millis = millis();
      cmd[cmd_cnt] = ret;
      if(cmd[cmd_cnt] == '\n'){
        len = cmd_cnt+1;
        cmd_cnt = 0;
        return len;
      }
      cmd_cnt++;
      if(cmd_cnt == CMD_BUF_LEN){
        cmd_cnt = 0;
        return -1;
      }
    }

    if(millis() - start_millis > 100){
      cmd_cnt = 0;
      return -1;
    }
  }
}


