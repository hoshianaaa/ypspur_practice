#include <ros/ros.h>

extern "C"{
  #include <ypspur.h>
}

int move2point(double x, double y){

  double rx, ry, rt;
  Spur_get_pos_GL(&rx,&ry,&rt);
  std::cout << "x:" << rx << " y:" << ry << std::endl;
  
  double ang = std::atan2(y - ry, x - rx);  	
  std::cout << "ang:" << ang << std::endl;

  YPSpur_unfreeze();
  Spur_spin_GL(ang);
  while(!Spur_near_ang_GL(ang, 0.1))usleep(10000);
  Spur_line_GL( x, y, ang);
  while( !Spur_near_pos_GL( x, y, 0.1))usleep(10000);
  YPSpur_freeze();

  return 1;
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "spur_circle");
  ros::NodeHandle nh;
  
  Spur_init();

  YPSpur_set_vel(0.5);
  YPSpur_set_angvel(1.0);
  YPSpur_set_accel(1.0);
  YPSpur_set_angaccel(2.0);

  Spur_set_pos_GL( 0, 0, 0);

  while(ros::ok()){
	Spur_circle_GL( 0, 0, 1.0);
  }
  return 0;
}


