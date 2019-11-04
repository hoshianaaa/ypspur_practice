#include <ros/ros.h>

extern "C"{
  #include <ypspur.h>
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "test1");
  ros::NodeHandle nh;
  
  Spur_init();
  YP_set_wheel_vel(13.0, 13.0);
  YP_set_wheel_accel(13.0, 13.0);
 
  double x, y, th;
  while(ros::ok()){
    //YP_wheel_vel(1.0, 1.0); 
    YPSpur_get_pos(0, &x, &y, &th);
    std::cout << "pos GL: (" << x << ", " << y << ", " << th << ")" << std::endl;
    YPSpur_get_pos(1, &x, &y, &th);
    std::cout << "pos GL: (" << x << ", " << y << ", " << th << ")" << std::endl;
  }
  return 0;
}
