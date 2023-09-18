#ifndef ODRIVE_ODRIVE_H_
#define ODRIVE_ODRIVE_H_

#include "main.h"
#include "bsp_can.h"


//COMMAND ID
#define ODRIVE_HEARTBEAT_MESSAGE		0x001
#define SET_AXIS_NODE_ID				0x006
#define SET_AXIS_REQUESTED_STATE 		0x007
#define ENCODER_ESTIMATES				0x009
#define GET_ENCODER_COUNT				0x00A
#define SET_CONTROLLER_MODES			0x00B
#define SET_INPUT_POS					0x00C
#define SET_INPUT_VEL					0x00D
#define SET_INPUT_TORQUE				0x00E
#define SET_LIMITS						0x00F
#define GET_IQ							0x014
#define REBOOT_ODRIVE					0x016
#define GET_BUS_VOLTAGE_CURRENT			0x017
#define CLEAR_ERRORS					0x018
#define SET_POSITION_GAIN				0x01A
#define SET_VEL_GAINS					0x01B

//ODrive CAN ID
typedef enum
{   CAN_ODRIVE_M1_ID = 0x11,
    CAN_ODRIVE_M2_ID = 0x15,
    CAN_ODRIVE_M3_ID = 0x13,
    CAN_ODRIVE_M4_ID = 0x14,

} can_odr_id_e;

//Axis Parameters
typedef struct axis_t
{
    int AXIS_ID;
    float AXIS_Encoder_Pos;
    float AXIS_Encoder_Vel;
    int32_t AXIS_Encoder_CPR;
    int32_t AXIS_Encoder_Shadow;
    float AXIS_Bus_Voltage;
    float AXIS_Bus_Current;
    float AXIS_Iq_Setpoint;
    float AXIS_Iq_Measured;
    uint32_t AXIS_Error;
    uint8_t AXIS_Current_State;
    uint8_t Controller_Status;
} axis_t;

//Axis States
typedef enum {
    UNDEFINED = 0x0,
    IDLE = 0x1,
    STARTUP_SEQUENCE = 0x2,
    FULL_CALIBRATION_SEQUENCE = 0x3,
    MOTOR_CALIBRATION = 0x4,
    ENCODER_INDEX_SEARCH = 0x6,
    ENCODER_OFFSET_CALIBRATION = 0x7,
    CLOSED_LOOP_CONTROL = 0x8,
    LOCKIN_SPIN = 0x9,
    ENCODER_DIR_FIND = 0xA,
    HOMING = 0xB,
    ENCODER_HALL_POLARITY_CALIBRATION = 0xC,
    ENCODER_HALL_PHASE_CALIBRATION = 0xD
} Axis_State;

//Control Modes
typedef enum {
    VOLTAGE_CONTROL = 0x0,
    TORQUE_CONTROL = 0x1,
    VELOCITY_CONTROL = 0x2,
    POSITION_CONTROL = 0x3
} Control_Mode;

//Input Modes
typedef enum {
    INACTIVE = 0x0,
    PASSTHROUGH = 0x1,
    VEL_RAMP = 0x2,
    POS_FILTER = 0x3,
    MIX_CHANNELS = 0x4,
    TRAP_TRAJ = 0x5,
    TORQUE_RAMP = 0x6,
    MIRROR = 0x7,
    TUNING = 0x8
} Input_Mode;



/**
 * @param       AXIS: Pointer to Axis Instance
 * @return      none
 * @brief	    This function is used as a callback function whenever an CAN RX interrupt occurs.
 * 				It is used to receive data from the ODrive when an RTR frame is sent to it as well
 * 				as the heartbeat message and Encoder Count sent at a set frequency. The data is used
 * 		        to update the required variables in the Axis struct.
 */
void odrv_get_axis_status(axis_t *AXIS, int32_t CMD_ID);


/**
 * @param	Axis Defines Axis Instance
 * @param	state  Used to select Axis_State
 * @return  none
 * @brief	Sends a CAN Data Packet with the required Axis ID and Command ID to command the axis
 * 			to change state or perform certain routines. List of Axis States listed above.
 */
void odrv_Set_Axis_Requested_State(axis_t Axis, Axis_State state);


/**
 * @param	    Axis: Defines Axis Instance
 * @param		vel: Sets the desired velocity of the axis
 * @param		torqueff: sets the feed-forward torque of the torque controller
 * @return      none
 * @brief		Sends a CAN Data Packet with the required Axis ID and Command ID to set the desired
 * 				velocity of the axis and the the feed-forward torque of the torque controller
 */
void odrv_Set_Input_Vel(axis_t Axis, float vel, float torqueff);


/**
 * @param	AXIS_ID Axis ID of the given Axis
 * @param	COMMAND_ID Command ID of command to be executed
 * @param	id_type Used to select standard or extended ID(CAN_ID_Standard,CAN_ID_Extended)
 * @param	frame_type Select frame type: RTR or Data(CAN_Frame_Data, CAN_Frame_Remote)
 * @param	data_length Set data length of packet to be sent
 * @return  none
 * @brief	sed to set the CAN TX Struct Parameters such as data_length, frame_type, id_type, ID
 */
void Set_TX_Param(int AXIS_ID, int COMMAND_ID, int id_type, int frame_type, int data_length);



/**
 * @brief		Sends a CAN Data Packet with the required Axis ID and Command ID to clear all
 * 				the errors of this device including all contained submodules.
 * @param   	Axis: Defines Axis Instance
 * @return      none
 */
void odrv_Clear_Errors(axis_t Axis);


/**
 * @brief		  Sends a CAN Data Packet with the required Axis ID and Command ID to reboot
 * 				  the controller without saving the current configuraiton.
 * 				  Message can be sent to either address on a given ODrive board.
 * @param	      Axis: Defines Axis Instance
 * @return        none
 */
void odrv_Reboot_ODrive(axis_t AXIS_ID);


/**
 * @brief		  Sends a CAN Data Packet with the required Axis ID and Command ID to
 * 				  set the Control Mode and Input Mode of given axis
 * @param         Axis: Defines Axis Instance
 * @param		  ControlMode: Sets the control mode of specified axis
 * @param		  InputMode: sets the input mode of specified axis
 * @return        none
 */
void odrv_Set_Controller_Modes(axis_t Axis, Control_Mode ControlMode, Input_Mode InputMode);


/**
 * @brief		  Sends a CAN Data Packet with the required Axis ID and Command ID to set
 * 				  the desired position of the axis as well as the feed-forward velocity
 * 				  and feed-forward torque
 * @param	      Axis: Defines Axis Instance
 * @param		  Input_Pos: Set the desired position of the axis
 * @param		  Vel_FF: sets the feed-forward velocity of the velocity controller
 * @param		  Torque_FF: sets the feed-forward torque of the torque controller
 * @return        none
 */
void odrv_Set_Input_Pos(axis_t Axis, float Input_Pos, int Vel_FF, int Torque_FF);


/**
 * @brief		  Sends a CAN RTR Frame with required ID to the ODrive to request Encoder Shadow Count
 * 				  and Encoder Count in CPR.
 * 				  Note: Function only sends the RTR frame. Data will be received and variables will be updated
 * 				  via the CallBack function when an reception interrupt is triggered.
 * @param    	  Axis: Defines Axis Instance
 * @return        none
 */
void odrv_Get_Encoder_Count(axis_t Axis);


/**
 * @brief		  Sends a CAN Data Packet with the required Axis ID and Command ID to set
 * 				  the desired torque of the axis.
 * @param	      Axis: Defines Axis Instance
 * @param		  torque: Sets the desired output torque of the axis
 * @return        none
 */
void odrv_Set_Input_Torque(axis_t Axis, float torque);


/**
 * @brief		  Sends a CAN RTR Frame with required ID to the ODrive to request Bus Voltage and Bus Current.
 * 				  Note: Function only sends the RTR frame. Data will be received and variables will be updated
 * 				  via the CallBack function when an reception interrupt is triggered.
 * @param	      Axis: Defines Axis Instance
 * @return        none
 */
void odrv_Get_Bus_Voltage_Current(axis_t Axis);


/**
 * @brief		  Sends a CAN RTR Frame with required ID to the ODrive to request Iq Setpoint and Iq Measured.
 * 				  Note: Function only sends the RTR frame. Data will be received and variables will be updated
 * 				  via the CallBack function when an reception interrupt is triggered.
 * @param	      Axis: Defines Axis Instance
 * @return        none
 */
void odrv_Get_IQ(axis_t Axis);


/**
 * @brief		  Sends a CAN Data Packet with the required Axis ID and Command ID to set
 * 				  the desired position gain
 * @param      	  Axis: Defines Axis Instance
 * @param		  pos_gain: Sets the desired position gain
 * @return        none
 */
void odrv_Set_Position_Gain(axis_t Axis, float pos_gain);


/**
 * @brief		Sends a CAN Data Packet with the required Axis ID and Command ID to set
 * 				the desired velocity gain and velocity integrator gain
 * @param	    Axis:   Defines Axis Instance
 * @param		Vel_Gain: Sets the desired velocity gain
 * @param		Vel_Int_Gain: Sets the desired velocity integrator gain
 * @return      none

 */
void odrv_Set_Vel_Gains(axis_t Axis, float Vel_Gain, float Vel_Int_Gain);


/**
 * @brief		  Sends a CAN Data Packet with the required Axis ID and Command ID to update
 * 				  the given axis' Node Id
 * @param	      Axis: Defines Axis Instance
 * @param	      node_id: Updated Node Id
 * @return        none
 */
void odrv_Set_Axis_Node_ID(axis_t Axis, uint32_t node_id);


/**
 * @brief		Sends a CAN Data Packet with the required Axis ID and Command ID to update
 * 				the given axis' Node Id
 * @param	    Axis: Defines Axis Instance
 * @param	    vel_lim: Sets the velocity limit
 * @param		curr_lim: Sets the current limit
 * @return      none

 */
void odrv_Set_Limits(axis_t Axis, float vel_lim, float curr_lim);



/**
  * @brief          返回底盘电机 2006电机数据指针
  * @param[in]      i: 电机编号,范围[0,3]
  * @retval         电机数据指针
  */
const axis_t *get_chassis_vel_motor_measure_point(uint8_t i);


#endif /* CAN_ODRIVE_H_ */
