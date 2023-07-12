#ifndef _VOICETASK_h
#define _VOIICETASK_h

#include <Task.h>
#define CMD_BUF_LEN      64+1

class VoiceTask: public Task{
public:    
VoiceTask(const char *name, uint32_t stack, QueueHandle_t q):Task(name, stack){que=q;};

protected:
int receiveCMD();

void cleanup() override;
void setup() override;
void loop() override;
QueueHandle_t que;
uint8_t cmd_cnt;
uint8_t cmd[CMD_BUF_LEN]; 
};


#endif 