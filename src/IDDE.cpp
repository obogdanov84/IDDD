#include <I2C_eeprom.h>
#include "IDDE.hpp" 

// generate a base address  to write first byte of data based on the session ID and key press counter. Memory bounding included
uint16_t sessionMemoryAddressStart (uint8_t sessionID, uint8_t eventCounter)
{
    
    uint16_t calcaulatedaddress = (RESERVED_MEMORY_START_ADD+RESERVED_MEMORY_SIZE)+(sessionID*MAX_EVENT_COUNT)+eventCounter*DATASIZE;
    
    if (calcaulatedaddress <= (RESERVED_MEMORY_START_ADD+RESERVED_MEMORY_SIZE) || calcaulatedaddress > MEMORY_SIZE)
    {
        return(calcaulatedaddress);
    }
    else
    {
        printf("out of bounds memory adddress");
        return(0);
    }
}


// Write an event in externl memory based on the dataArray, its length and the session that it is currently working withing,
// session id will place the arrays into relevant adressed chunk
void writeEvent (uint16_t dataArray[DATASIZE], uint16_t sessionID) // dataarray how do we pass it in?
{
    // create variable for what the pressed event is, pull that out of dataArray and use for subsequent checks
    uint8_t event =  dataArray[EVENTID];

    // check if the button has not been pressed in session already, button presses are not necessarily sequential

         

        //use session event counter to drop back the session count times in order to check if even occured

            // if entry exists, beep and state that this entry exists already          
            
            // if it doesnt exist and you are withing boundary of session memory addresses

                // write sequentially increasing bytes for the whole data array of the event and its associated items into eeprom
                for (int i = 0; i<= DATASIZE; i++)
                {
                    //clear exisiting entries in address byte
                    I2C_eeprom::writeByte(sessionMemoryAddressStart(sessionID+i, 0));
                    //write relevant section of dataArray into the address
                    I2C_eeprom::writeByte(sessionMemoryAddressStart(sessionID+i, dataArray[i]));
                    // Check if written value matches what has been written in
                   if ( I2C_eeprom::readByte(sessionMemoryAddressStart(sessionID+i, dataArray[i])) != dataArray[i])
                   {
                       printf("You donned didnt write the value to EEProm properly");
                   }                    
                }
     


    
   
    // increment the sessionEvent counter if you are still within bounds of max allocated entries in session
    if(sessionEvenetCounter < MAX_EVENT_COUNT)
    {
        sessionEvenetCounter++;
    }
    else if(sessionEvenetCounter == MAX_EVENT_COUNT)
    {
        sessionEvenetCounter = 0;
        printf("session has ended you fool, youre out of memory allocation");
    }
    else
    {
        printf("sessionEvent counter has shat itself");
    }
    
}

