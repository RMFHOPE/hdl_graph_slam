#include "ros/ros.h"
#include <hdl_graph_slam/SaveMap.h>
#include <string.h>
#include <signal.h>

ros::ServiceClient client;
hdl_graph_slam::SaveMap savemap;

void save_interrupt(int s) {
    // Save map
    client.call(savemap);

    // Restore default behaviour of SIGINT
    signal(SIGINT, SIG_DFL);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "map_saver");
    ros::NodeHandle n;

    // Form message
    float res = 0.05;
    std::string des = argv[1];

    // Prepare to call save service
    client = n.serviceClient<hdl_graph_slam::SaveMap>("/hdl_graph_slam/save_map");
    savemap.request.resolution = res;
    savemap.request.destination = des + ".pcd";

    // Trigger ctrl c interrupt to call service
    signal(SIGINT, save_interrupt);

    ros::spin();

    return 0;
}

