#include <booster/idl/b1/LowCmd.h>
#include <booster/idl/b1/LowState.h>
#include <booster/robot/b1/b1_loco_client.hpp>
#include <booster/robot/channel/channel_publisher.hpp>
#include <booster/robot/channel/channel_subscriber.hpp>
#include <iostream>

int nb_motors = 22;

int main(int argc, char **argv) {
  // Initializing client & challen publisher
  booster::robot::b1::B1LocoClient client;
  booster::robot::ChannelPublisher<booster_interface::msg::LowCmd>
      channel_publisher(booster::robot::b1::kTopicJointCtrl);
  std::vector<booster_interface::msg::MotorCmd> motor_cmds;

  booster::robot::ChannelFactory::Instance()->Init(0);

  // Creating motor cmds
  for (size_t i = 0; i < nb_motors; i++) {
    booster_interface::msg::MotorCmd motor_cmd;
    motor_cmds.push_back(motor_cmd);
  }

  // Initializing channels
  std::cout << "Initialization..." << std::endl;
  channel_publisher.InitChannel();
  client.Init();

  std::this_thread::sleep_for(std::chrono::seconds(2));

  int32_t res;

  // Entering PREPARE mode
  do {
    std::cout << "Entering PREPARE mode..." << std::endl;
    int32_t res = client.ChangeMode(booster::robot::RobotMode::kPrepare);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  } while (res != 0);

  // Waiting for the robot to be PREP
  std::this_thread::sleep_for(std::chrono::seconds(6));

  // Entering CUSTOM mode
  do {
    std::cout << "Entering CUSTOM mode..." << std::endl;
    res = client.ChangeMode(booster::robot::RobotMode::kCustom);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  } while (res != 0);

  // Sending commands at ~50Hz
  std::cout << "Sending commands..." << std::endl;
  while (true) {
    booster_interface::msg::LowCmd low_cmd;
    low_cmd.cmd_type(booster_interface::msg::CmdType::SERIAL);
    low_cmd.motor_cmd(motor_cmds);
    for (int i = 0; i < nb_motors; i++) {
      low_cmd.motor_cmd()[i].q(0.0);
      low_cmd.motor_cmd()[i].dq(0.0);
      low_cmd.motor_cmd()[i].tau(0.0);
      low_cmd.motor_cmd()[i].kp(10.0);
      low_cmd.motor_cmd()[i].kd(1.0);
      low_cmd.motor_cmd()[i].weight(1.0);
    }
    channel_publisher.Write(&low_cmd);

    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
}