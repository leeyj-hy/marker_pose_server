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
            for(int i=0; i<msg.transforms.size() ; i++)
            {
                this -> trn_x = msg.transforms[i].transform.translation.x;
                this -> trn_y = msg.transforms[i].transform.translation.y;
                this -> trn_z = msg.transforms[i].transform.translation.z;
                this -> q_x = msg.transforms[i].transform.rotation.x;
                this -> q_y = msg.transforms[i].transform.rotation.y;
                this -> q_z = msg.transforms[i].transform.rotation.z;
                this -> q_w = msg.transforms[i].transform.rotation.w;
                ROS_INFO("pose received");
            }
        }

        bool mrkrPoseSrv(robot_msgs::mrkrPos::Request &req,
                         robot_msgs::mrkrPos::Response &res)
        {
            Q2E();

            res.trn_x = trn_x;
            res.trn_y = trn_y;
            res.trn_z = trn_z;
            res.rot_x = _R;
            res.rot_y = _P;
            res.rot_z = _Y;
            res.is_pos_return = true;

            ROS_INFO("Pose Service returned!");
            ROS_INFO("R : %f", _R);
            ROS_INFO("P : %f", _P);
            ROS_INFO("Y : %f", _Y);
            return true;
            
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