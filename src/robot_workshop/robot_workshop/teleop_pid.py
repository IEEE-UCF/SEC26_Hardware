#!/usr/bin/env python3
"""
WASD teleop with PID velocity control for differential drive robots

Keys:
  W/S : forward / backward
  A/D : turn left / right
  Q/E : decrease / increase max speed scaling
  SPACE : stop (reset PIDs)
  X : exit
"""

import sys, select, termios, tty, time
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry


class PID:
    def __init__(self, kp=1.0, ki=0.0, kd=0.0, i_limit=1.0, out_limit=None):
        self.kp, self.ki, self.kd = kp, ki, kd
        self.i_limit = abs(i_limit)
        self.out_limit = out_limit
        self.reset()

    def reset(self):
        self.i = 0.0
        self.prev_e = None

    def update(self, e, dt):
        p = self.kp * e
        self.i += self.ki * e * dt
        self.i = max(min(self.i, self.i_limit), -self.i_limit)
        d = 0.0
        if self.prev_e is not None and dt > 1e-6:
            d = self.kd * (e - self.prev_e) / dt
        self.prev_e = e
        u = p + self.i + d
        if self.out_limit:
            u = max(min(u, self.out_limit), -self.out_limit)
        return u


class TeleopPID(Node):
    def __init__(self):
        super().__init__('teleop_pid')

        # Parameters
        self.rate_hz = 50.0
        self.max_lin = 0.8
        self.max_ang = 2.5
        self.step_lin = 0.2
        self.step_ang = 0.6

        # User-scaling
        self.scale = 1.0
        self.scale_step = 0.1
        self.scale_min, self.scale_max = 0.2, 1.5

        # PIDs
        self.pid_lin = PID(1.2, 0.3, 0.02, i_limit=0.6, out_limit=self.max_lin)
        self.pid_ang = PID(1.2, 0.25, 0.02, i_limit=1.0, out_limit=self.max_ang)

        # Targets + state
        self.vx_target = 0.0
        self.wz_target = 0.0
        self.vx_meas = 0.0
        self.wz_meas = 0.0

        # ROS 2 I/O shit
        self.cmd_pub = self.create_publisher(Twist, '/cmd_vel', 10)
        self.create_subscription(Odometry, '/odom', self.odom_cb, 10)

        # Control loop timer
        self.dt = 1.0 / self.rate_hz
        self.create_timer(self.dt, self.loop)

        self.settings = termios.tcgetattr(sys.stdin)
        self.get_logger().info("Teleop PID ready — use W/A/S/D, Q/E, SPACE, X")

    # Callbacks
    def odom_cb(self, msg):
        self.vx_meas = msg.twist.twist.linear.x
        self.wz_meas = msg.twist.twist.angular.z

    # Keyboard crap
    def get_key(self, timeout=0.0):
        tty.setraw(sys.stdin.fileno())
        rlist, _, _ = select.select([sys.stdin], [], [], timeout)
        key = sys.stdin.read(1) if rlist else ''
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, self.settings)
        return key

    def handle_key(self, k):
        if k in ['w', 'W']:
            self.vx_target += self.step_lin
        elif k in ['s', 'S']:
            self.vx_target -= self.step_lin
        elif k in ['a', 'A']:
            self.wz_target += self.step_ang
        elif k in ['d', 'D']:
            self.wz_target -= self.step_ang
        elif k in ['q', 'Q']:
            self.scale = max(self.scale_min, self.scale - self.scale_step)
            self.get_logger().info(f'Scale -> {self.scale:.1f}x')
        elif k in ['e', 'E']:
            self.scale = min(self.scale_max, self.scale + self.scale_step)
            self.get_logger().info(f'Scale -> {self.scale:.1f}x')
        elif k == ' ':
            self.vx_target = self.wz_target = 0.0
            self.pid_lin.reset(); self.pid_ang.reset()
            self.get_logger().info('STOP')
        elif k in ['x', 'X']:
            raise KeyboardInterrupt

        # Clamp
        self.vx_target = max(min(self.vx_target, self.max_lin*self.scale),
                             -self.max_lin*self.scale)
        self.wz_target = max(min(self.wz_target, self.max_ang*self.scale),
                             -self.max_ang*self.scale)

    # Control Loop
    def loop(self):
        try:
            k = self.get_key()
            if k:
                self.handle_key(k)
        except KeyboardInterrupt:
            self.stop()
            rclpy.shutdown()
            return

        e_vx = self.vx_target - self.vx_meas
        e_wz = self.wz_target - self.wz_meas
        vx_cmd = self.pid_lin.update(e_vx, self.dt)
        wz_cmd = self.pid_ang.update(e_wz, self.dt)

        twist = Twist()
        twist.linear.x = vx_cmd
        twist.angular.z = wz_cmd
        self.cmd_pub.publish(twist)

    def stop(self):
        twist = Twist()
        self.cmd_pub.publish(twist)
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, self.settings)
        self.get_logger().info("Stopped robot.")


def main():
    rclpy.init()
    node = TeleopPID()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.stop()
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
