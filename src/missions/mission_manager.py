#!/usr/bin/env python2
'''
Executable for mission manager code
'''
import json
import subprocess
import os
import signal
import time
import sys

import rospy
import rospkg

from missions import MissionManager

MISSION_PARAM = '~missions'

if __name__ == '__main__':
    rospack = rospkg.RosPack()


    base = rospack.get_path('robosub')
    try:
        # Check if roscore is already running
        print(rospy.get_master().getPid())
        run_core = False
    except Exception as e:
        print(e)
        run_core = True

    if run_core:
        print("Starting new roscore")
        # Run roscore in a process group to be able to kill it and its children properly
        # See https://stackoverflow.com/questions/4789837/how-to-terminate-a-python-subprocess-launched-with-shell-true/4791612
        roscore = subprocess.Popen('roscore', shell=True, preexec_fn=os.setsid)
        time.sleep(2)

    # Needed for parameter namespaces
    rospy.init_node('mission_manager')
    if rospy.has_param(MISSION_PARAM):
        missions = json.loads(rospy.get_param(MISSION_PARAM))
    else:
        print("%s must be specified" % MISSION_PARAM)
        sys.exit(-1)

    try:
        manager = MissionManager()


        print("Running missions")
        manager.run(missions)
        print("Done running missions")
    except Exception as e:
        print(e)
    except KeyboardInterrupt:
        pass

    if run_core:
        # See https://stackoverflow.com/questions/4789837/how-to-terminate-a-python-subprocess-launched-with-shell-true/4791612
        os.killpg(os.getpgid(roscore.pid), signal.SIGINT)
