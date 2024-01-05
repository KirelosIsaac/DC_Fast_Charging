#ifndef Rashed
#define Rashed


#define BAUDRATE            9600
#define PINTX                2
#define PINRX                1
#define MAX_STRING_LENGTH    20
// define the number of precicions used in the system and 
// the remaining of 20 will be the digits that can be decimal
#define Presicions 15

void API_PID_SetDutyCycle(double *Local_doubleDutyCycle);
void API_Initialize(void);
double API_receiveData(void);

#endif
