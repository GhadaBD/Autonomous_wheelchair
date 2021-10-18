#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "std_msgs/String.h"
#include <tf/transform_listener.h>
#include <iostream>
#include <fstream>

int main( int argc, char* argv[] )
{
  ros::init(argc, argv, "basic_shapes");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  
   assert(argc == 2);

  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;
  visualization_msgs::Marker marker;
  marker.ns = "basic_shapes";
  marker.id = atoi(argv[1]);
  //while (ros::ok())
  //{
    
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    

    // Set the marker type.  
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;

  
    tf::StampedTransform transform;
    tf::TransformListener listener;
    // Open the database file
    std::ofstream myfile;
    myfile.open("/home/koussay/catkin_ws/src/newchair5/text",std::ofstream::app);
  
    try {
          listener.waitForTransform("map", "base_link", ros::Time(0), ros::Duration(52.0));
          listener.lookupTransform("/map","/base_link",ros::Time(0), transform);
          if (marker.id ==0) {
          	myfile << "Kitchen: " << transform.getOrigin().x() << "," << transform.getOrigin().y() << "\n";
			      }
          else if (marker.id==1) {
                myfile << "Bathroom: " << transform.getOrigin().x() << "," << transform.getOrigin().y() << "\n";
				  }
          else {
                myfile << "Living room: " << transform.getOrigin().x() << "," << transform.getOrigin().y() << "\n";
               }
        }
    catch (tf::TransformException ex){
          ROS_ERROR("Nope ! %s", ex.what());
        }

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = transform.getOrigin().x();
    marker.pose.position.y = transform.getOrigin().y();
    marker.pose.position.z = 0.5;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 1.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();
    



    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    marker_pub.publish(marker);
    r.sleep();

    myfile.close();
  //}
   
}
