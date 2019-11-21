#include <ros/ros.h>

extern "C"{
  #include <ypspur.h>
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "test1");
  ros::NodeHandle nh;
  
  Spur_init();

  YPSpur_set_vel(0.5);
  YPSpur_set_angvel(1.5);
  YPSpur_set_accel(1.0);
  YPSpur_set_angaccel(2.0);

  YPSpur_set_pos( 0, 0, 0, 0);

  while(ros::ok()){

    YPSpur_line( 0, 0, 0, 0);
    while( !YPSpur_near_pos(0, 2, 0, 0.1))usleep(10000);
    YPSpur_spin(0,M_PI/2);
    while( !YPSpur_near_ang(0, M_PI/2, 0.1))usleep(10000);
    YPSpur_line( 0, 2, 0, M_PI/2);
    while( !YPSpur_near_pos(0, 2, 2, 0.1))usleep(10000);
    YPSpur_spin(0,M_PI);
    while( !YPSpur_near_ang(0, M_PI, 0.1))usleep(10000);
    YPSpur_line( 0, 0, 2, M_PI);
    while( !YPSpur_near_pos(0, 0, 2, 0.1))usleep(10000);
    YPSpur_spin(0,M_PI*3/2);
    while( !YPSpur_near_ang(0, M_PI*3/2, 0.1))usleep(10000);
    YPSpur_line( 0, 0, 0, M_PI*3/2);
    while( !YPSpur_near_pos(0, 0, 0, 0.1))usleep(10000);

    YPSpur_freeze();
  }
  return 0;
}
