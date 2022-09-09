#include <vector>
#include <string>


#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "/home/werlley/catkin_ws/devel/include/ros_publisher_subscriber/RoboDistancia.h"

using ros_publisher_subscriber::RoboDistancia;
using std::string;
using std::vector;

class Landmark{
public:
    Landmark(string nome, double x, double y): nome(nome), x(x), y(y){}
    string nome;
    double x;
    double y;
};

class PublishAndSubscribe{
public:
    PublishAndSubscribe(const ros::Publisher& landmark_pub): landmarks_(), landmark_pub_(landmark_pub){
        InicializarLandmark();
    }
    void OdomCallback(const nav_msgs::Odometry::ConstPtr& msg){
        double x = msg->pose.pose.position.x;
        double y = msg->pose.pose.position.y;
        RoboDistancia rd = FindClosest(x,y);
                 
        landmark_pub_.publish(rd);
        if(rd.distancia <= 0.5){
            ROS_INFO("Eu estou proximo a %s", rd.nome.c_str());
        }
    }


private:
    vector<Landmark> landmarks_;
    ros::Publisher landmark_pub_;

    RoboDistancia FindClosest(double x, double y){
        RoboDistancia resultado;
        resultado.distancia = -1;

        for(size_t i = 0; i < landmarks_.size(); ++i){
            Landmark landmark = landmarks_[i];

            double xd = landmark.x = x;
            double yd = landmark.y = y;
            double distance = sqrt(xd*xd + yd*yd);

            if(resultado.distancia < 0 || distance < resultado.distancia){
                resultado.nome = landmark.nome;
                resultado.distancia = distance;
            }
        }
        return resultado;
    }

    void InicializarLandmark(){
        landmarks_.push_back(Landmark("Cubo",0.31,-0.99));
        landmarks_.push_back(Landmark("Container",0.11,-2.42));
        landmarks_.push_back(Landmark("Cilindro",-1.14,-2.88));
        landmarks_.push_back(Landmark("Barreira",-2.59,-0.83));
        landmarks_.push_back(Landmark("Estante",-0.09,0.53));
    }
};





int main(int argc, char** argv){
    ros::init(argc, argv, "location_monitor");
    ros::NodeHandle nh;
    ros::Publisher landmark_pub = nh.advertise<RoboDistancia>("closest_landmark",10);
    PublishAndSubscribe monitor(landmark_pub);
    ros::Subscriber sub = nh.subscribe("odom", 10, &PublishAndSubscribe::OdomCallback, &monitor);
    ros::spin();

    return 0;
}

