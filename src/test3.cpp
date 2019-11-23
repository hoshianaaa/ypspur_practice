#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <boost/thread/thread.hpp>

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

void tf_broad(int* publish_rate)
{
  ros::Rate loop_rate(*publish_rate);
  static tf::TransformBroadcaster br;

	double x, y, yaw;
  while (ros::ok())
  {

    // broad cast
    Spur_get_pos_BS(&x, &y, &yaw);
    //std::cout << "tf broad thread" << std::endl;
    //std::cout << "x:" << x << " y:" << y << " yaw:" << yaw << std::endl << std::endl;
    tf::Transform transform;
    transform.setOrigin( tf::Vector3(x, -y, 0.0) );
    tf::Quaternion q;
    q.setRPY(0, 0, -yaw);
    transform.setRotation(q);
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "odom", "base_link"));

    loop_rate.sleep();
  }
}

void command(int* publish_rate)
{
  ros::Rate loop_rate(*publish_rate);
	while (ros::ok())
		{
			std::cout << "command thread" << std::endl;
      loop_rate.sleep();
		}
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "test3");
  ros::NodeHandle nh;

  int tf_broad_rate = 10;
  int command_rate = 1;
  boost::thread tf_broad_thread(tf_broad, &tf_broad_rate);  
  boost::thread command_thread(command, &command_rate);  

  Spur_init();

  YPSpur_set_vel(0.5);
  YPSpur_set_angvel(1.0);
  YPSpur_set_accel(1.0);
  YPSpur_set_angaccel(2.0);

  Spur_spin_GL(-M_PI/2);
  //Spur_line_GL(0.0, 0.0, 0.0);
/*
  Spur_set_pos_GL( 0, 0, 0);
  YPSpur_unfreeze();
  move2point(3.0,0.0); 
  Spur_set_pos_GL( 3.0, 0.3, 0.2);
  move2point(6.0,0.0); 
  move2point(6.0,-5.0); 
  //Spur_line_GL( 0, 0, 0);
  //while( !Spur_near_pos_GL( 0.5, 0, 0.1))usleep(10000);
  YPSpur_freeze();
*/

  ros::Rate rate(5);

  double x, y, z;
  double roll, pitch, yaw;

  tf::TransformListener listener;

  while(ros::ok()){
    tf::StampedTransform transform;
    try{
			//listener.waitForTransform("/map", "/base_link", ros::Time(0), ros::Duration(10.0) );
      listener.lookupTransform("/map", "/base_link", ros::Time(0), transform);

			double x = transform.getOrigin().x();
			double y = transform.getOrigin().y();
    	tf::Matrix3x3(transform.getRotation()).getRPY(roll, pitch, yaw);

      Spur_set_pos_GL(x, y, yaw);

    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    } 

    Spur_get_pos_GL(&x, &y, &yaw);
    std::cout << "GL:" << x << "," << y << "," << yaw << std::endl;
    Spur_get_pos_LC(&x, &y, &yaw);
    std::cout << "LC:" << x << "," << y << "," << yaw << std::endl;
    Spur_get_pos_BS(&x, &y, &yaw);
    std::cout << "BS:" << x << "," << y << "," << yaw << std::endl;
    Spur_get_pos_BL(&x, &y, &yaw);
    std::cout << "BL:" << x << "," << y << "," << yaw << std::endl;
    std::cout << std::endl;

    rate.sleep();
  }
  return 0;
}


