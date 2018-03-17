# CANBUS ROS Package
It contains the `CAN` ROS message that adapts to the one used in `CANalyst-ii`, and wrapped ROS node for further using. The ROS reads CAN frame from CAN bus and broadcast `canalystii_node_msg/can` msg with specific topic.

CANalyst-2 driver package under ROS can read CANbus device information and send CANbus information.

When using Autoware to control the unmanned vehicle, open the launch directly.

roslaunch canalystii_node coffee_car_control.launch
