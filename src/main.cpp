  #include "Arduino.h"
#include "Uart2Task.h"


QueueHandle_t queue;
SemaphoreHandle_t btn_semaphore;
EventGroupHandle_t flags;
MessageBufferHandle_t uart2_message;

Uart2Task * uart2;
#define CMD_BUF_LEN      64+1
uint8_t cmd[CMD_BUF_LEN];
uint8_t cmd_cnt;

//alarm_messages=xMessageBufferCreate(SSTATE_LENGTH+4);//=length label
//web_messages=xMessageBufferCreate(SSTATE_LENGTH+4);


void setup(void)
{

  queue= xQueueCreate(16,sizeof(event_t));
  uart2_message=xMessageBufferCreate(UART_MESSAGE_LENGTH+4);

  Serial.begin(115200);
  //Serial.println(F("Elechouse Voice Recognition V3 Module \"train\" sample."));
  Serial2.begin(9600);
  //Serial2.write("\xAA\x02\x01\x0A",4);
  uart2= new Uart2Task("UART2",3072,queue);  
  uart2->resume();


}


int receiveCMD()
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

/**
 * @brief   compare two commands, case insensitive.
 * @param   para1  -->  command buffer 1
 * para2  -->  command buffer 2
 * len    -->  buffer length
 * @retval  0  --> equal
 * -1  --> unequal
 */


void loop(void)
{
  int len1;
 char bf[100];
  len1=receiveCMD();
  
  if(len1>0){
    String st;
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
      Serial.println("Len=");
      Serial.println(len1);
      //bf="\xAA\x03\x03\x00\x0A\x00";
      //st=String(cmd[1]);
      //if (len1>2) st+=cmd[2];
      Serial.println((char *)cmd);
      bf[0]=0xAA;
      bf[1]=0x03;
      bf[2]=0x03;
      bf[3]=cmd[1]-'0';
      if (len1>2){
        bf[3]=bf[3]*10+cmd[2]-'0';
      }
      bf[4]=0x0A;
      Serial.println(bf);
      //Serial.print(bf);
      //Serial2.write(cmd+1,1);
      //Serial2.write("\x0A",1);grefger
      break;
      case 'R':
      Serial2.write("\xAA\x02\x10\x0A",5);
      break;
      case 'm':
      Serial2.write("\xAA\x03\x12\x01\x0A",6);
      break;
      case 'a':
      Serial2.write("\xAA\x06\x15\x07\x00\x01\x02\x0A",8);
      break;
      case 'v':
      Serial2.write("\xAA\x08\x21\x00\x4C\x69\x67\x68\x74\x0A",10);
      break;
      case 'l':
      Serial2.write("\xAA\x03\x30\x00\x0A",5);
      break;
      case 'c':
      Serial.print("Check group mode 6th byte (00-disable,01-system,02-user,FF-check):");
      Serial2.write("\xAA\x04\x32\x00\xFF\x0A",6);
      break;
    }
    //Serial2.write("\xAA\x02\x01\x0A",4);
  // Serial2.write(cmd,len1);
   // Serial.write(cmd,len1);
  }
  vTaskDelay(100);
    
}

