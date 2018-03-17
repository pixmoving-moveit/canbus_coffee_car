#include "ros/ros.h"
#include "canalystii_node.h"
#include "canalystii_node_msg/can.h"
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/TwistStamped.h>

VCI_BOARD_INFO pInfo;
int count=0;
int main(int argc, char **argv){
      ros::init(argc, argv, "canalystii_node");
      CANalystii_node can_node;
      ros::NodeHandle nh;
      if(!can_node.start_device())
      {
          ROS_WARN("device starts error");
          return -1;
      }
      ROS_INFO("listening to can bus");
      VCI_CAN_OBJ can_obj;
      printf(">>this is hello !\r\n");

      VCI_INIT_CONFIG config;
      config.AccCode=0x80000008;
      config.AccMask=0xFFFFFFFF;
      config.Filter=1;
      config.Timing0=0x00;
      config.Timing1=0x1C;
      config.Mode=0;
      unsigned int can_idx = 0;
      if(VCI_InitCAN(VCI_USBCAN2,0,0,&config)!=1)
      {
        printf(">>Init CAN1 error\n");
        VCI_CloseDevice(VCI_USBCAN2,0);
      }

      if(VCI_StartCAN(VCI_USBCAN2,0,0)!=1)
      {
        printf(">>Start CAN1 error\n");
        VCI_CloseDevice(VCI_USBCAN2,0);

      }

      if(VCI_InitCAN(VCI_USBCAN2,0,1,&config)!=1)
      {
        printf(">>Init can2 error\n");
        VCI_CloseDevice(VCI_USBCAN2,0);

      }
      if(VCI_StartCAN(VCI_USBCAN2,0,1)!=1)
      {
        printf(">>Start can2 error\n");
        VCI_CloseDevice(VCI_USBCAN2,0);

      }
  
      VCI_CAN_OBJ send[1];
      send[0].ID=0;
      send[0].SendType=0;
      send[0].RemoteFlag=0;
      send[0].ExternFlag=1;
      send[0].DataLen=8;

      int i=0;
      for(i = 0; i < send[0].DataLen; i++)
      {
        send[0].Data[i] = i;
      }

      ros::Rate r(10.0);
      while(ros::ok())
      {

        ros::spinOnce();
        unsigned int recv_len = 1;
        if(VCI_Transmit(VCI_USBCAN2, 0, 0, send, 1) == 1)
        {

          send[0].ID=387;
          send[0].TimeFlag=1;
          send[0].SendType=0;
          send[0].RemoteFlag=0;
          send[0].ExternFlag=0;
          send[0].DataLen=8;
          send[0].Data[0]=1;
          send[0].Data[1]=1;
          send[0].Data[2]=1;
          send[0].Data[3]=1;
          send[0].Data[4]=1;
          send[0].Data[5]=1;
          send[0].Data[6]=1;
          send[0].Data[7]=send[0].Data[0]+send[0].Data[1]+send[0].Data[2]+send[0].Data[3]+send[0].Data[4]+send[0].Data[5]+send[0].Data[6];//checksum

        }

        if(can_node.receive_can_frame(can_idx,can_obj,recv_len,20))
        {
          canalystii_node_msg::can msg = CANalystii_node::can_obj2msg(can_obj);
          can_node.can_msg_pub_.publish(msg);
        }
        r.sleep();

      }

    return 0;
}
