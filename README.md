# MIE444
Design of Autonomous Vehicle
1.0 Project Requirements
Our primary task is to design an autonomous rover that performs localization with minimum resolution of larger 1’ x 1’ square, delivery of load, and obstacle avoidances. The rover performs the following activities in successions within 5 minutes:
Locates itself in the given map through localization. 
Identify loading zone, drive to loading zone
Locate and pick up block, a 0.5lbs beige block smaller than 2’’x2’’x2’’, from the loading zone
Deliver block to location B,  (1’x 1’ square surrounded by 3 walls) , given prior to starting the run

The physical limitation of the rover includes dimensions less than 12’’x 12’’x 12’’ and weighs less than 5lbs. The rover parts are limited to non-premade kits, not incorporating touch sensors for obstacle avoidance (walls and blocks). The pick up mechanism must not include the use of adhesive or velcro. All groups are given 2 trials to complete all given tasks.
 
2.0 Rover Design

2.1 Mechanical Design
The frame of the robot is in the shape of a cube. Even though the maximum allowable size of the rover is 12” x 12”, our robot will be 8” x 8” wide. This will allow it to rotate in place without hitting walls — i.e., the maximum diagonal dimension should be less than 12”, so each side should not be more than 12"/2 = 8.4”. The robot will be 8.2” tall, calculated by height of the ultrasonic sensors, arduino, battery, wheel and frame. Figure 1a, 1b, and 1c are CAD drawings of our robot.
