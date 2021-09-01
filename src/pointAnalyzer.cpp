#include "ros/ros.h"
#include "std_msgs/String.h"
#include <chrono>
#include <unistd.h>

#include "visualization_msgs/MarkerArray.h"
#include "visualization_msgs/Marker.h"

#include "vector"

visualization_msgs::MarkerArray markerArray_msg_saved_global;
//int minMark1;
//int minMark2;

bool done = false;

std::vector<int> shortestVector_global;


using namespace std;

float findPathLength(const std::vector<int>& v)
{
	float totalPathLength = 0.0;
	for(int i = 0; i < v.size(); i++)
	{
		float x1, y1, x2, y2;
		x1 = markerArray_msg_saved_global.markers.at(v.at(i%v.size())).pose.position.x;
		y1 = markerArray_msg_saved_global.markers.at(v.at(i%v.size())).pose.position.y;
		x2 = markerArray_msg_saved_global.markers.at(v.at((i+1)%v.size())).pose.position.x;
		y2 = markerArray_msg_saved_global.markers.at(v.at((i+1)%v.size())).pose.position.y;

		float dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
		totalPathLength += dist;
	}

return totalPathLength;
}

void markerArrayCallback(const visualization_msgs::MarkerArray::ConstPtr& markerArray_msg)
{
	auto start = chrono::steady_clock::now();

	vector<int> shortestVector;
	vector<int> vector1;

	visualization_msgs::MarkerArray markerArray_msg_saved;

	for(int i = 0; i < markerArray_msg->markers.size(); i++)
	{
		markerArray_msg_saved.markers.push_back(markerArray_msg->markers.at(i));
		vector1.push_back(markerArray_msg_saved.markers.at(i).id);
	}
	markerArray_msg_saved_global = markerArray_msg_saved;

	int numberOfPermutations = 0;
	float shortestPathLength = 1000.0;
    	do {
		float thisPathLength = findPathLength(vector1);
		numberOfPermutations++;
		if(thisPathLength < shortestPathLength)
		{
			shortestPathLength = thisPathLength;
			shortestVector = vector1;
		}
    	} while (std::next_permutation(vector1.begin(), vector1.end()));

	shortestVector.push_back(shortestVector.at(0));
	shortestVector_global = shortestVector;
	
	ROS_INFO("I found the shortest path at ");
	for(int i = 0; i < shortestVector.size(); i++)
	{
		ROS_INFO("ID %i", shortestVector.at(i));
	}
	ROS_INFO("With a length of %f", shortestPathLength);
	ROS_INFO("In total %i different path possibilities", numberOfPermutations);
	auto end = chrono::steady_clock::now();

	ROS_INFO("That took me %d milliseconds", chrono::duration_cast<chrono::milliseconds>(end - start).count());
	done = true;

	/*float minDist = 1000000;

	for(int j = 0; j < markerArray_msg_saved.markers.size() - 1; j++)
	{
		for(int k = j+1; k < markerArray_msg_saved.markers.size(); k++)
		{
			float x1, y1, x2, y2;
			x1 = markerArray_msg_saved.markers.at(j).pose.position.x;
			y1 = markerArray_msg_saved.markers.at(j).pose.position.y;
			x2 = markerArray_msg_saved.markers.at(k).pose.position.x;
			y2 = markerArray_msg_saved.markers.at(k).pose.position.y;

			float dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);

			if(dist < minDist)
			{
				minDist = dist;
				minMark1 = j;
				minMark2 = k;
			}
		}
	}
	ROS_INFO("minDIstance is %f between Marker %i and %i", minDist, minMark1, minMark2);*/
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "pointAnalyzer");

	ros::NodeHandle n;
  
	ros::Subscriber marker_sub = n.subscribe("markerArray", 1000, markerArrayCallback);
	//ros::Publisher marker_pub = n.advertise<visualization_msgs::MarkerArray>("markerArrayShortDist", 100);
  	ros::Publisher marker_pub2 = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);

	ros::Rate loop_rate(10);


	while (ros::ok())
	{

		if(!markerArray_msg_saved_global.markers.empty())
		{
  			visualization_msgs::MarkerArray markerArray_msg;
  			visualization_msgs::Marker marker;

			visualization_msgs::Marker line_strip;
			line_strip.header.frame_id = "/my_frame";
			line_strip.header.stamp = ros::Time::now();
			line_strip.ns = "points_and_lines";
			line_strip.action = visualization_msgs::Marker::ADD;
			line_strip.pose.orientation.w = 1.0;
		        line_strip.id = 1;
   			line_strip.type = visualization_msgs::Marker::LINE_STRIP;
			line_strip.scale.x = 0.5;
			line_strip.color.b = 1.0;
			line_strip.color.a = 1.0;
  			

			for(int i = 0; i < shortestVector_global.size(); i++)
			{
				geometry_msgs::Point p;
				marker = markerArray_msg_saved_global.markers.at(shortestVector_global.at(i));

				p.x = marker.pose.position.x;
				p.y = marker.pose.position.y;
	     			line_strip.points.push_back(p);

			}
			marker_pub2.publish(line_strip);

				/*geometry_msgs::Point p;

				marker = markerArray_msg_saved_global.markers.at(minMark1);
				p.x = marker.pose.position.x;
				p.y = marker.pose.position.y;
	     			line_strip.points.push_back(p);
				
				marker.id = 789;
				marker.color.r = 1.0;
	  			marker.color.g = 0.0;
	  			marker.color.b = 0.0;
	 			markerArray_msg.markers.push_back(marker);


	  			marker = markerArray_msg_saved_global.markers.at(minMark2);
				p.x = marker.pose.position.x;
				p.y = marker.pose.position.y;
	     			line_strip.points.push_back(p);

				marker.id = 790;
	  			marker.color.r = 1.0;
	  			marker.color.g = 0.0;
	  			marker.color.b = 0.0;
	 			markerArray_msg.markers.push_back(marker);
	    			
				marker_pub.publish(markerArray_msg);
	   			marker_pub2.publish(line_strip);*/
		}

		if(done == true)
		{
			int test;
			cin >> test;
			break;
		}
		ros::spinOnce();

    		loop_rate.sleep();
  	}

return 0;
}
