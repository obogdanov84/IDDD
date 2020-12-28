
#define SESSION_1                    0
#define SESSION_2                    1
#define SESSION_3                    2

#define MAX_EVENT_COUNT              15
#define RESERVED_MEMORY_SIZE         10
#define RESERVED_MEMORY_START_ADD    0

// utilised memory definitions
#define MEMORY_SIZE 64000

uint8_t sessionEvenetCounter = 0;


// dataArrayindex definitions for logged events

#define DATA        0
#define HOUR        1
#define MINUTE      2
#define SECOND      3
#define SESSIONID   4
#define EVENTID     5

#define DATASIZE    6



/*
struct eventLog
{
            uint8_t     data;
            uint8_t     hour;
            uint8_t     minute;
            uint8_t     second;
            uint8_t     sessionID;
            uint8_t     eventID; 
};
*/
// ---------------Function declaration--------------------------
uint16_t sessionMemoryAddressStart (uint8_t sessionID, uint8_t eventCounter);
void writeEvent (uint8_t dataArray[DATASIZE], uint8_t sessionID);


