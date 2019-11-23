#include <ros/ros.h>
#include <boost/thread/thread.hpp>

extern "C"{
  #include <ypspur.h>
}

void near_pos(int* publish_rate)
{
  ros::Rate loop_rate(*publish_rate);

  while (ros::ok())
  {
      loop_rate.sleep();
  }
}


int main(int argc, char **argv)
{

  ros::init(argc, argv, "test1");
  ros::NodeHandle nh;
  
  int thread_rate = 10;
  boost::thread thread(near_pos, &thread_rate);  

  Spur_init();

	YP_set_wheel_vel(1.0, 1.0);
	YP_set_wheel_accel(6.0, 6.0);

	YP_wheel_vel(1.0, 1.0);	
  double x, y, t;
  while(ros::ok()){
		
  }
  return 0;
}


