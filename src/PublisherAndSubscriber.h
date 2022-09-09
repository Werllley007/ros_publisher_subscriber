#include <vector>
#include <string>


#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "/home/werlley/catkin_ws/devel/include/ros_publisher_subscriber/RoboDistancia.h"
#include "Landmark.h"

#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"

using ros_publisher_subscriber::RoboDistancia;
using std::string;
using std::vector;

class PublisherAndSubscriber{
public:
    PublisherAndSubscriber();
    PublisherAndSubscriber(const ros::Publisher& landmark_pub);
    void OdomCallback(const nav_msgs::Odometry::ConstPtr& msg);
    RoboDistancia FindClosest(double x, double y);
    void InicializarLandmark();

private:
    vector<Landmark> landmarks_;   
    //ros::Publisher landmark_pub_;
    ros::Subscriber odometry;
    ros::Publisher Robot_pub;
    ros::Publisher landmark_pub;
    ros::NodeHandle node_;
};