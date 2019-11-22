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

void do_stuff(int* publish_rate)
{
  ros::Rate loop_rate(*publish_rate);
  while (ros::ok())
  {
    std::cout << "test" << std::endl;
    loop_rate.sleep();
  }
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "test2");
  ros::NodeHandle nh;

  int rate_b = 1;
  boost::thread thread_b(do_stuff, &rate_b);  
/*
  Spur_init();

  YPSpur_set_vel(0.5);
  YPSpur_set_angvel(1.0);
  YPSpur_set_accel(1.0);
  YPSpur_set_angaccel(2.0);

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
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(0.0, 0.0, 0.0) );
  tf::Quaternion q;
  q.setRPY(0, 0, 1.4);
  transform.setRotation(q);

  tf::TransformListener listener;
  ros::Rate rate(1);

  double x, y, t;
  while(ros::ok()){

    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "odom", "base_link"));

    tf::StampedTransform s_transform;
    try{
      listener.lookupTransform("/map", "/base_link",  
                               ros::Time(0), s_transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    } 
    
    std::cout << "trans x:" << s_transform.getOrigin().x() << std::endl;
    std::cout << "trans y:" << s_transform.getOrigin().y() << std::endl;

    double roll, pitch, yaw;
    tf::Matrix3x3(s_transform.getRotation()).getRPY(roll, pitch, yaw);
    std::cout << "t:" << yaw << std::endl << std::endl << std::endl;


    rate.sleep();
    
/*
    Spur_get_pos_GL(&x, &y, &t);
    std::cout << "GL:" << x << "," << y << "," << t << std::endl;
    Spur_get_pos_LC(&x, &y, &t);
    std::cout << "LC:" << x << "," << y << "," << t << std::endl;
    Spur_get_pos_BS(&x, &y, &t);
    std::cout << "BS:" << x << "," << y << "," << t << std::endl;
    Spur_get_pos_BL(&x, &y, &t);
    std::cout << "BL:" << x << "," << y << "," << t << std::endl;
    std::cout << std::endl;
    usleep(1000000);
*/
  }
  return 0;
}


