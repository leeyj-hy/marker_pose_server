#include "ros/ros.h"
#include "fiducial_msgs/Fiducial.h"
#include "fiducial_msgs/FiducialTransformArray.h"
#include "geometry_msgs/Twist.h"
#include "robot_msgs/mrkrPos.h"

using namespace ros;
using namespace std;


class marker_pose_srv
{
    private:

        ros::NodeHandle n;
        ros::Subscriber sub1;
        ros::ServiceServer srv1;

    public:
        marker_pose_srv()
        {
            sub1 = n.subscribe("/fiducial_transforms", 2, &marker_pose_srv::mrkrPoseCallback, this);
            srv1 = n.advertiseService("/marker_pose_srv", &::mrkrPoseSrv);
        }

        void mrkrPoseCallback(const fiducial_msgs::FiducialTransformArray &msg)
        {

        }

        bool mrkrPoseSrv(robot_msgs::)
        {

        }

        void poseAvg()
        {

        }

};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "marker_pose_srvr");
    marker_pose_srv MPS_obj;

    ros::spin();
    return 0;
}