# Booster K1 crash minimal example

## Install Booster SDK

Clone the following repository: https://github.com/BoosterRobotics/booster_robotics_sdk/

And run `install.sh` as root

## Compile

Run the following:

```
mkdir build
cd build
cmake ..
make
```

And then run:

```
./main
```

The robot should go to 0 position with a soft kp (10).

## Crash

The crash happens after a while (usually a few minutes). The booster-daemon crashes and the robot

<video src="crash.mp4" width="180" />