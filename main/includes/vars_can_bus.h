#include <hardware/flash.h>
#include "mcp2515.h"
#include <map>

// Unique identifiers for inter-core communication commands
enum InterCoreCmd {
    ReadData =  1,   // From core 1 to core 0: contains data read (16 bit)
    WriteData = 2,   // From core 0 to core 1: contains data to write (16 bit)
    ErrorData = 3    // From core 1 to core 0: contains regs CANINTF, EFLG
};

// Can-bus setup
struct canBus_vars {
    unsigned char this_pico_flash_id[8];
    unsigned char node_address;  // Node address (last byte of the flash ID)
    struct can_frame canMsgTx;
    struct can_frame canMsgRx;
    unsigned long counterTx;
    unsigned long counterRx;
    MCP2515::ERROR err;
    unsigned long time_to_write;
    unsigned long write_delay;
    const byte interruptPin;

    canBus_vars() : counterTx(0), counterRx(0), write_delay(1000), interruptPin(20) {}
};

enum my_type : uint8_t //needs to 8 bits
{
  ACK,
  ACKINTERNA,
  
  SET_REFERENCE,
  MEASURE_NO_LIGHTS, //4
  MEASURE_LIGHTS, //5
  NOTIFY_FUTURE_LIGHT,//6
  ENDGAINS,
  WAKE_UP,
  SET_DUTY_CYCLE,
  SET_OCCUPANCY,
  SET_ANTI_WINDDUP,
  SET_FEEDBACK,

  START_STREAM_LUX,
  START_STREAM_DUTY_CYCLE,
  STOP_STREAM,
  STAR_BUFFER_PRINT_LUX,
  STAR_BUFFER_PRINT_DUTY_CYCLE,

  GET_REFERENCE, 
  GET_DUTY_CYCLE,
  GET_ILUMINANCE,
  GET_OCCUPANCY,
  GET_ANTI_WINDDUP,
  GET_FEEDBACK,
  GET_EXTERNAL_ILUMINANCE,
  GET_ELAPSED_TIME,
  GET_INSTANTANEOUS_POWER,
  GET_AVERAGE_ENERGY,
  GET_AVERAGE_VISIBILITY,
  GET_AVERAGE_FLICKER,
  GET_LAST_MINUTE_BUFFER,
  
  GET_LOWER_BOUND_OCCUPIED,
  GET_LOWER_BOUND_UNOCCUPIED,
  GET_CURRENT_LOWER_BOUND,
  GET_CURRENT_ENERGY_COST,
  SET_LOWER_BOUND_OCCUPIED,
  SET_LOWER_BOUND_UNOCCUPIED,
  SET_CURRENT_ENERGY_COST,
  RESTART,
  SERIAL_GET_LOWER_BOUND_OCCUPIED,
  SERIAL_GET_LOWER_BOUND_UNOCCUPIED,
  SERIAL_GET_CURRENT_LOWER_BOUND,
  SERIAL_GET_CURRENT_ENERGY_COST,

  SERIAL_GET_REFERENCE,
  SERIAL_GET_DUTY_CYCLE,
  SERIAL_GET_ILUMINANCE,
  SERIAL_GET_OCCUPANCY,
  SERIAL_GET_ANTI_WINDDUP,
  SERIAL_GET_FEEDBACK,
  SERIAL_GET_EXTERNAL_ILUMINANCE,
  SERIAL_GET_ELAPSED_TIME,
  SERIAL_GET_INSTANTANEOUS_POWER,
  SERIAL_GET_AVERAGE_ENERGY,
  SERIAL_GET_AVERAGE_VISIBILITY,
  SERIAL_GET_AVERAGE_FLICKER,
  SERIAL_GET_LAST_MINUTE_BUFFER_LUX,
  SERIAL_GET_LAST_MINUTE_BUFFER_DUTY_CYCLE,

  SERIAL_STREAM_LUX,
  SERIAL_STREAM_DUTY_CYCLE,
  STOP_STREAM_LUX,
  STOP_STREAM_DUTY_CYCLE,

  FOUND_HUB,
  //consensus
  //ENDCONSENSUS,
  ACKCONSENSUS,
  RECEIVECONSENSUS0,
  RECEIVECONSENSUS1,
  RECEIVECONSENSUS2,
  RECEIVECONSENSUS3,
  BEGINCONSENSUS,
  ACKBEGINCONSENSUS,
  CHANGEITER,
};

struct info_msg {
  my_type type;
  size_t size;
  unsigned char sender, data[8];
  unsigned char can_id;
};