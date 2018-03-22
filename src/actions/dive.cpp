#include <actionlib/server/simple_action_server.h>
#include <geometry_msgs/Pose.h>
#include <robosub/DiveAction.h>
#include <ros/ros.h>
#include <cmath>
#include <string>

constexpr double kTolerance = 1.0;

// See
// http://wiki.ros.org/actionlib_tutorials/Tutorials/SimpleActionServer(GoalCallbackMethod)

typedef actionlib::SimpleActionServer<robosub::DiveAction> Server;

class DiveAction {
 public:
  DiveAction(std::string name)
      : depth_(0.0), server_(nh_, name, false), action_name_(name) {
    // Register callback for when a new goal is received
    server_.registerGoalCallback(boost::bind(&DiveAction::goalCallback, this));

    // Register callback for when the current goal is cancelled
    server_.registerPreemptCallback(
        boost::bind(&DiveAction::preemptCallback, this));

    depth_sub_ = nh_.subscribe("/pose", 1, &DiveAction::poseCallback, this);

    server_.start();
  }

  ~DiveAction() {}

  void goalCallback() { 
    depth_ = server_.acceptNewGoal()->depth;
    ROS_INFO("%s: Received new depth goal %f", action_name_.c_str(), depth_);
  }

  void preemptCallback() { 
    ROS_INFO("%s: Preempted", action_name_.c_str());
    server_.setPreempted();
  }

  void poseCallback(const geometry_msgs::Pose::ConstPtr& msg) {
    double depth = msg->position.z;
    robosub::DiveFeedback feedback;
    feedback.depth = depth;
    server_.publishFeedback(feedback);

    if (::fabs(depth_ - depth) < kTolerance) {
      ROS_INFO("%s: Succeeded", action_name_.c_str(),);
      robosub::DiveResult result;
      result.depth = depth;
      server_.setSucceeded(result);
    } else {
      // Do PID control here
    }
  }

 private:
  double depth_;
  ros::NodeHandle nh_;
  Server server_;
  std::string action_name_;
  ros::Subscriber depth_sub_;
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "dive_server");

  DiveAction(ros::this_node::getName());

  ros::spin();
  return 0;
}