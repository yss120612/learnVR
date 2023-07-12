#include <Arduino.h>
#include "Uart2Task.h"
#include "VoiceTask.h"
#include <LEDTask.h>
#include "Settings.h"





SemaphoreHandle_t btn_semaphore;
QueueHandle_t queue;
EventGroupHandle_t flags;
MessageBufferHandle_t uart2_message;
LEDTask * leds;

Uart2Task * uart2;
VoiceTask * voice;
uint8_t cmd_cnt;

//alarm_messages=xMessageBufferCreate(SSTATE_LENGTH+4);//=length label
//web_messages=xMessageBufferCreate(SSTATE_LENGTH+4);


void setup(void)
{

  queue= xQueueCreate(16,sizeof(event_t));
 // uart2_message=xMessageBufferCreate(UART_MESSAGE_LENGTH+4);

  Serial.begin(115200);
  //Serial.println(F("Elechouse Voice Recognition V3 Module \"train\" sample."));
  Serial2.begin(9600);
  //Serial2.write("\xAA\x02\x01\x0A",4);
  uart2= new Uart2Task("UART2",3072,queue);  
  uart2->resume();
  voice= new VoiceTask("VOICE",3072,queue);  
  voice->resume();

  leds= new LEDTask("leds",4096,queue,lpins,true);
  leds->resume();
  notify_t notify;
  notify.title=LEDBRIGHTNESS1;
  notify.packet.value=64;
  notify.packet.var=0;
  leds->notify(notify);
  vTaskDelay(pdMS_TO_TICKS(100));
  notify.title=LEDBRIGHTNESS2;
  notify.packet.value=255;
  notify.packet.var=0;
  leds->notify(notify);
  vTaskDelay(pdMS_TO_TICKS(100));
  notify.title=LEDBRIGHTNESS3;
  notify.packet.value=0;
  notify.packet.var=0;
  leds->notify(notify);
  vTaskDelay(pdMS_TO_TICKS(100));

  notify.title=LEDMODE1;
  notify.packet.value=BLINK_ON;;
  notify.packet.var=0;
  leds->notify(notify);
  vTaskDelay(pdMS_TO_TICKS(100));
  notify.title=LEDMODE2;
  notify.packet.value=BLINK_4HZ;
  notify.packet.var=0;
  leds->notify(notify);
  vTaskDelay(pdMS_TO_TICKS(100));
  notify.title=LEDMODE3;
  notify.packet.value=BLINK_ON;
  notify.packet.var=0;
  leds->notify(notify);
  

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
  
  vTaskDelay(100);
    
}

