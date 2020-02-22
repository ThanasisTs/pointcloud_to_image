#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>

ros::Publisher image_pub_;
sensor_msgs::Image image_;
int count=0;

void cloud_cb (const sensor_msgs::PointCloud2ConstPtr& cloud){
	count++;
	ROS_INFO("Received pointcloud %d", count);
	if ((cloud->width * cloud->height) == 0)
	 	return; //return if the cloud is not dense!
	try{
 		pcl::toROSMsg (*cloud, image_); //convert the cloud
	}
	catch (std::runtime_error e){
 		ROS_ERROR_STREAM("Error in converting cloud to image message: " << e.what());
	}
	image_pub_.publish (image_); //publish our cloud image
}


int main (int argc, char **argv){
	ros::init (argc, argv, "pointcloud_to_image");
	ros::NodeHandle nh;
	std::string input_topic, output_topic;
	nh.param("/pointcloud_to_image/input_topic", input_topic, std::string("None"));
	nh.param("/pointcloud_to_image/output_topic", output_topic, std::string("None"));
	
	ros::Subscriber sub_ = nh.subscribe (input_topic, 30, cloud_cb);
    image_pub_ = nh.advertise<sensor_msgs::Image> (output_topic, 30);

	ros::spin (); //where she stops nobody knows
	return 0;
}