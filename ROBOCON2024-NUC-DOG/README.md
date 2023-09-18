# Chassis 底盘A板代码


## 功能

- 舵轮底盘运动：
  - 转向电机M2006(C610)、速度电机5065(ODrive)驱动
  - Action OPS 数据接收
  - 底盘速度方向闭环控制
- 遥控器接收：
  - 底盘运动
  - TODO: 取环、发射
- TODO: 与云台C板通信

## 硬件连接&资源使用

- CAN1 -> DJI电机电调
- CAN2 -> ODrive
- USART1 -> DBUS
- USART6 -> Action OPS
- UART? -> 云台C板
