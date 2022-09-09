#include "PublisherAndSubscriber.h"

PublisherAndSubscriber::PublisherAndSubscriber(){ 
//PublisherAndSubscriber::PublisherAndSubscriber(const ros::Publisher& landmark_pub): landmarks_(), landmark_pub_(landmark_pub){
    InicializarLandmark();

    odometry = node_.subscribe("/odom", 10, &PublisherAndSubscriber::OdomCallback, this);

    Robot_pub = node_.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 10);
    landmark_pub = node_.advertise<RoboDistancia>("/closest_landmark", 10);

}
void PublisherAndSubscriber::OdomCallback(const nav_msgs::Odometry::ConstPtr& msg){
    double x = msg->pose.pose.position.x;
    double y = msg->pose.pose.position.y;
    RoboDistancia rd = FindClosest(x,y);
                
    landmark_pub.publish(rd);

    if(rd.distancia <= 0.5){
        ROS_INFO("Eu estou proximo: %s", rd.nome.c_str());
    }
}

RoboDistancia PublisherAndSubscriber::FindClosest(double x, double y){
    RoboDistancia resultado;
    resultado.distancia = -1;

    for(size_t i = 0; i < landmarks_.size(); ++i){
        Landmark landmark = landmarks_[i];

        double xd = landmark.x - x;
        double yd = landmark.y - y;
        double distance = sqrt(xd*xd + yd*yd);

        if(resultado.distancia < 0 || distance < resultado.distancia){
            resultado.nome = landmark.nome;
            resultado.distancia = distance;
        }
    }
    return resultado;
}

void PublisherAndSubscriber::InicializarLandmark(){
    landmarks_.push_back(Landmark("Cubo",0.31,-0.99));
    landmarks_.push_back(Landmark("Container",0.11,-2.42));
    landmarks_.push_back(Landmark("Cilindro",-1.14,-2.88));
    landmarks_.push_back(Landmark("Barreira",-2.59,-0.83));
    landmarks_.push_back(Landmark("Estante",-0.09,0.53));
}