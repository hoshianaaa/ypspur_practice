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
			std::cout << Spur_near_ang_GL(M_PI/2, M_PI/4) << std::endl;
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

  YPSpur_set_vel(1);
  YPSpur_set_angvel(1.0);
  YPSpur_set_accel(2.0);
  YPSpur_set_angaccel(2.0);

  Spur_set_pos_GL( 0, 0, 0);

  double x, y, t;
  while(ros::ok()){
		Spur_vel(0.0, 0.4);
		usleep(2000000);
		//Spur_vel(-0.3, 0);
		usleep(2000000);
  }
  return 0;
}


