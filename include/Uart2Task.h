#ifndef _UART2TASK_h
#define _UART2TASK_h

#include "Task.h"

#define BUFFER_SIZE 1024
class Uart2Task: public Task{
public:    
Uart2Task(const char *name, uint32_t stack, QueueHandle_t q):Task(name, stack){que=q;};

protected:
void  printBuff(char * buff, uint16_t length);
void cleanup() override;
void setup() override;
void loop() override;
QueueHandle_t que;
 
};


#endif 