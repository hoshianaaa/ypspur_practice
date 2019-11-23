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
  YPSpur_set_angvel(1.0);
  YPSpur_set_accel(1.0);
  YPSpur_set_angaccel(2.0);

  Spur_set_pos_GL( 0, 0, 0);



  double x, y, t;
  while(ros::ok()){
  	Spur_stop_line_GL( 5, 1, 0);
  }
  return 0;
}


