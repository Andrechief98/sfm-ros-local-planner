#ifndef TURTLE_PLANNER_H_
#define TURTLE_PLANNER_H_

// abstract class from which our plugin inherits
#include <nav_core/base_local_planner.h>

#include <ros/ros.h>
#include <tf2_ros/buffer.h>
#include <tf2/utils.h>
#include <angles/angles.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <base_local_planner/odometry_helper_ros.h>
#include <nav_msgs/Odometry.h>
#include <vector>

using namespace std;

static const double     _PI= 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348;
static const double _TWO_PI= 6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696;

nav_msgs::Odometry robot_pose_;

namespace turtle_planner{

class TurtlePlanner : public nav_core::BaseLocalPlanner{
public:
    TurtlePlanner();
    TurtlePlanner(std::string name, tf2_ros::Buffer* tf, costmap_2d::Costmap2DROS* costmap_ros);

    ~TurtlePlanner();

    void initialize(std::string name, tf2_ros::Buffer* tf, costmap_2d::Costmap2DROS* costmap_ros);

    bool setPlan(const std::vector<geometry_msgs::PoseStamped>& orig_global_plan);

    bool computeVelocityCommands(geometry_msgs::Twist& cmd_vel);

    bool isGoalReached();

private:
    ros::Publisher pub;
    ros::Subscriber sub;

    ros::Subscriber sub_odom;
    ros::Subscriber sub_goal;
    ros::Publisher pub_cmd;
    ros::NodeHandle nh;
    costmap_2d::Costmap2DROS* costmap_ros_;
    tf2_ros::Buffer* tf_;
    geometry_msgs::Twist cmd_vel_;
    bool initialized_;

    //GOAL:
    std::vector<geometry_msgs::PoseStamped> global_plan_;
    geometry_msgs::PoseStamped goal_pose_;

    std::vector<double> goal_coordinates;
    double goal_orientation;

    //ROBOT ODOMETRY AND VELOCITY:
    std::vector<double> curr_robot_coordinates;
    double curr_robot_orientation;
    std::vector<double> curr_robot_lin_vel;

    std::vector<double> new_robot_lin_vel;
    double new_robot_ang_vel_z;

    //ROBOT SOCIAL FORCES
    std::vector<double> e;
    std::vector<double> F_att;
    std::vector<double> F_rep;
    std::vector<double> F_tot;

    double distance_tolerance=0.1;
    double angle_tolerance=0.17;
    bool goal_reached=false;
    
    double max_lin_acc_x=2;
    double max_lin_vel=0.5; //da ricavare dal file config dell'interbotix
    double max_angular_vel_z=1; //da ricavare dal file config dell'interbotix
    double desired_vel = 0.5; //valore da ricavare direttamente dal file dell'interbotix
    //double max_acc=2; //valore massimo di accelerazione (da sostituire con quelli ricavabili dal file di configurazione interbotix)
    double delta_t=0.7;
    double alfa=0.35;   //valore da inserire nel file di configurazione interbotix (se fattibile)
    double K_p=0.75; //costante proporzionale per il calcolo della velocità angolare (proporzionale all'errore);
    double K_r=0.9;
    };
};

#endif