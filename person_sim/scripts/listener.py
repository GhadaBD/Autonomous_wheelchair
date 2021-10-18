#!/usr/bin/env python  
import roslib
roslib.load_manifest('newchair5')
import rospy
import math
import tf
import geometry_msgs.msg

if __name__ == '__main__':
    rospy.init_node('tf_listener')

    listener = tf.TransformListener()

    follower= 'newchair5'
    followed = 'person_standing'
    chair_vel = rospy.Publisher('/newchair5_diff_drive_controller/cmd_vel', geometry_msgs.msg.Twist,queue_size=5)

    rate = rospy.Rate(10.0)
    while not rospy.is_shutdown():
        try:
            (trans,rot) = listener.lookupTransform('/base_footprint', '/person_standing', rospy.Time(0))
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            continue

        angular = 0.3 * math.atan2(trans[1], trans[0])
        linear = 0.3 * math.sqrt(trans[0] ** 2 + trans[1] ** 2)
        cmd = geometry_msgs.msg.Twist()
        if (trans[0] > 1.5 or trans[1] > 1.5):
        	cmd.linear.x = linear
        	cmd.angular.z = angular
	else:
		cmd.linear.x = 0
        	cmd.angular.z = 0

        chair_vel.publish(cmd)
        rate.sleep()
