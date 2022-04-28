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

        
        double trn_x, trn_y, trn_z;
        double _R, _P, _Y;
        
        
        
        double q_x;
        double q_y;
        double q_z;
        double q_w;
        

        // typedef struct
        // {
        //     double x;
        //     double y;
        //     double z;
        // }rot;



    public:
        marker_pose_srv()
        {
            sub1 = n.subscribe("/fiducial_transforms", 2, &marker_pose_srv::mrkrPoseCallback, this);
            srv1 = n.advertiseService("/marker_pose_srv", &marker_pose_srv::mrkrPoseSrv, this);
        }

        void mrkrPoseCallback(const fiducial_msgs::FiducialTransformArray &msg)
        {

        }

        bool mrkrPoseSrv(robot_msgs::mrkrPos::Request &req,
                         robot_msgs::mrkrPos::Response &res)
        {
            Q2E();

            res.trn_x = trn_x;
            res.trn_y = trn_y;
            res.trn_z = trn_z;
            res.rot_x = _Y;
            res.rot_y = _P;
            res.rot_z = _R;

            
        }

        void poseAvg()
        {

        }

        void Q2E()
        {
            double sinr_cosp = 2 * (q_w * q_x +q_y * q_z);
            double cosr_cosp = 1-2*(q_x*q_x+q_y*q_y);
            _R = atan2(sinr_cosp, cosr_cosp);

            double sinp = 2* (q_w * q_y - q_z * q_x);
            if(abs(sinp)>=1)
                _P=copysign(M_PI / 2, sinp);
            else
                _P = asin(sinp);

            double siny_cosp = 2*(q_w * q_z + q_x * q_y);
            double cosy_cosp = 1 - 2 * (q_y*q_y + q_z*q_z);
            _Y = atan2(siny_cosp, cosy_cosp);

        }

};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "marker_pose_srvr");
    marker_pose_srv MPS_obj;

    ros::spin();
    return 0;
}