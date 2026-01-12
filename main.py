# Load ROS HUMBLE
# source /opt/ros/humble/setup.bash
# source ~/booster_robotics_sdk_ros2/booster_ros2_interface/install/setup.bash

import time
import rclpy
from rclpy.qos import QoSProfile, ReliabilityPolicy, HistoryPolicy

from booster_interface.msg import LowState, LowCmd, MotorCmd
from booster_robotics_sdk_python import (  # type: ignore
    B1LocoClient,
    RobotMode,
)

print("Initializing rclpy")
rclpy.init()

num_joints = 22

print("Creating publisher node")
publish_node = rclpy.create_node("booster_deploy_low_cmd_pub")
publisher = publish_node.create_publisher(
    LowCmd,
    "joint_ctrl",
    QoSProfile(
        depth=1,
        reliability=ReliabilityPolicy.RELIABLE,
        history=HistoryPolicy.KEEP_LAST
    )
)

print("Creating motor cmd buffer")
low_cmd = LowCmd()  # type: ignore
low_cmd.cmd_type = LowCmd.CMD_TYPE_SERIAL   # type: ignore

motor_cmd_buf = [
    MotorCmd() for _ in range(num_joints)
]  # type: ignore
for i in range(num_joints):
    motor_cmd_buf[i].q = 0.0
    motor_cmd_buf[i].dq = 0.0
    motor_cmd_buf[i].tau = 0.0
    motor_cmd_buf[i].kp = 10.0
    motor_cmd_buf[i].kd = 1.0
    motor_cmd_buf[i].weight = 1.0
low_cmd.motor_cmd.extend(motor_cmd_buf)
motor_cmd = low_cmd.motor_cmd

print("Creating B1LocoClient")
client = B1LocoClient()
client.Init()

print("Entering custom mode")
client.ChangeMode(RobotMode.kCustom)

print("Entering main custom loop")
while True:
    publisher.publish(low_cmd)
    time.sleep(0.002)
