#include "ros/ros.h"
#include "std_msgs/String.h"
#include "visualization_msgs/MarkerArray.h"
#include "visualization_msgs/Marker.h"

#include <stdio.h>
#include <stdlib.h>
#include<time.h>

#include <sstream>

int minRegion = -50;
int maxRegion = 50;
int howManyMarkers = 11;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "pointGenerator");

	ros::NodeHandle n;

	ros::Publisher marker_pub = n.advertise<visualization_msgs::MarkerArray>("markerArray", 1000);

	ros::Rate loop_rate(10);

  
	visualization_msgs::MarkerArray markerArray_msg;

	visualization_msgs::Marker marker;
  	marker.header.frame_id = "/my_frame";
	marker.header.stamp = ros::Time();
	marker.ns = "my_namespace";
	marker.type = visualization_msgs::Marker::SPHERE;
	marker.action = visualization_msgs::Marker::ADD;
	marker.pose.position.z = 0;
	marker.pose.orientation.x = 0.0;
	marker.pose.orientation.y = 0.0;
	marker.pose.orientation.z = 0.0;
	marker.pose.orientation.w = 1.0;
	marker.scale.x = 1.0;
	marker.scale.y = 1.0;
	marker.scale.z = 1.0;
	marker.color.a = 1.0; 
	marker.color.r = 0.0;
	marker.color.g = 1.0;
	marker.color.b = 0.0;


	srand(time(0));
	for(int i = 0; i < howManyMarkers; i++)
	{
 		marker.id = i;
  
		marker.pose.position.x = rand() % (maxRegion - minRegion) + minRegion;
		marker.pose.position.y = rand() % (maxRegion - minRegion) + minRegion;
  

		markerArray_msg.markers.push_back(marker);
	}

	while (ros::ok())
	{
    		marker_pub.publish(markerArray_msg);

    		ros::spinOnce();

    		loop_rate.sleep();
	}

return 0;
}
