
#ifndef _CAN_BUS_PROCESS_H_
#define _CAN_BUS_PROCESS_H_

#include "stdint.h"
#include "stdbool.h"
#include "hal.h"
#include "string.h"
#include "adis16265.h"

#define BARREL_CAN        &CAND1

#define GIMBAL_MOTOR_NUM  2U
#define CHASSIS_MOTOR_NUM 4U
#define EXTRA_MOTOR_NUM   4U

/* CAN Bus 1 or 2 */
#define CAN_CHASSIS_FR_FEEDBACK_MSG_ID              0x201
#define CAN_CHASSIS_FL_FEEDBACK_MSG_ID              0x202
#define CAN_CHASSIS_BL_FEEDBACK_MSG_ID              0x203
#define CAN_CHASSIS_BR_FEEDBACK_MSG_ID              0x204
#define CAN_GIMBAL_YAW_FEEDBACK_MSG_ID              0x205
#define CAN_GIMBAL_PITCH_FEEDBACK_MSG_ID            0x206

#define CAN_GIMBAL_SEND_DBUS_ID                     0x001
#define CAN_CHASSIS_SEND_BARREL_ID                  0x002

#define CAN_POWER_MODULE_RECEIVER_ID                0x050
#define CAN_POWER_MODULE_SEND_MODE_ID               0x055      

#define CAN_SHOOTER_INFO_ID                         0x005


#define CAN_ENCODER_RANGE           8192            // 0x2000
#define CAN_ENCODER_RADIAN_RATIO    7.669904e-4f    // 2*M_PI / 0x2000

#define CAN_CHASSIS_DEBUG_FR 0x211
#define CAN_CHASSIS_DEBUG_FL 0x212
#define CAN_CHASSIS_DEBUG_BL 0x213
#define CAN_CHASSIS_DEBUG_BR 0x214

typedef enum
{
  GIMBAL_YAW = 0,
  GIMBAL_PITCH
}gimbal_num_t;

typedef enum
{
//  FRONT_LEFT = 1,
//  FRONT_RIGHT = 3,
//  BACK_LEFT = 0,
//  BACK_RIGHT = 2
    FRONT_RIGHT = 0,
    FRONT_LEFT = 1,
    BACK_LEFT = 2,
    BACK_RIGHT = 3
}chassis_num_t;

typedef struct {
    uint16_t raw_angle;
    int16_t  raw_current;
    int16_t  current_setpoint;

    uint16_t last_raw_angle;
    uint16_t offset_raw_angle;
    int32_t round_count;
    int32_t total_ecd;
    float radian_angle; // Continuous

    bool updated;
} GimbalEncoder_canStruct;

typedef struct {
    uint16_t raw_angle;
    int16_t  raw_speed;
    int16_t act_current;
    uint8_t temperature;

    uint16_t last_raw_angle;
    uint16_t offset_raw_angle;
    uint32_t msg_count;
    int32_t round_count;
    int32_t total_ecd;
    float radian_angle; // Continuous

    bool updated;
} ChassisEncoder_canStruct;

typedef struct{
    uint16_t channel0;
    uint16_t channel1;
    uint8_t  s1;
    uint8_t  s2;
    uint16_t key_code;
    bool updated;
} Gimbal_Send_Dbus_canStruct;

typedef struct{
  uint16_t heatLimit;
  uint16_t currentHeatValue;
  uint16_t remainHealth;

} BarrelStatus_canStruct;

typedef struct{
  int16_t _speed;
  int16_t speed_curve;
} MotorDebug_canStruct;

typedef struct 
{
  uint16_t shoot_speed;
  uint16_t rps;
  bool updated;
} ShooterInfo_canStruct;

typedef struct 
{
  uint8_t Vin;
  uint8_t pathType; // (1 = judge power, 2 = cap power)
  int16_t capEnergy;
  uint16_t powerJudge;
  uint16_t powerChassis;
  bool updated;
}__attribute__((packed)) PowerModule_canStruct;

volatile GimbalEncoder_canStruct* can_getGimbalMotor(void);
volatile ChassisEncoder_canStruct* can_getChassisMotor(void);
volatile ChassisEncoder_canStruct* can_getExtraMotor(void);
volatile Gimbal_Send_Dbus_canStruct* can_get_sent_dbus(void);
volatile BarrelStatus_canStruct* can_get_sent_barrelStatus(void);
volatile ShooterInfo_canStruct* can_get_gimbal_send_shooter_info(void);

void can_processInit(void);
void can_motorSetCurrent(CANDriver *const CANx,
  const uint16_t EID,
  const int16_t cm1_iq,
  const int16_t cm2_iq,
  const int16_t cm3_iq,
  const int16_t cm4_iq);


#endif
