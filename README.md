# hikingbuddy

The Hiking Buddy is a small, portable way for hikers to monitor their exertion and protect themselves from exhaustion. 

I. Pitch
This product will employ an accelerometer and gyroscope combination device which will analyze the user’s stride and pace. 
With this MPU-6050, the Hiking Buddy can determine the level of effort a person is using while hiking. 
The accelerometer measures peaks in two dimensions (x, y) to determine a person’s gate.
*To further accuracy in measuring levels of effort, two dimensions (y, z) can be analyzed determine whether a person is speeding up 
normally for a descending climb or slowing down normally for an ascending climb.


II. Sensing Plan
With the information from the two sensors, the Hiking Buddy will determine whether the user is
stopped, moving slowly, moving normally, or moving fast, relative to the terrain. If the user sustains a
slow pace, the Hiking Buddy will blink red after 60 seconds to remind the user to take a stopping break.
The light will remain on for 30 seconds, and then switch off. If the user sustains a fast pace, the Hiking
Buddy will blink red to remind the user to take a break after 30 seconds. *Once the user passes 1 mile,
the Hiking Buddy will blink green. Figure 1 shows the circuit diagram which will be used to connect the
accelerometer to the Arduino.

III. Sensor Analysis
Sensitivity is improved by using an accelerometer, rather than a pedometer which only measures step
count. An accelerometer is measuring multiple axes to determine if a “bounce” correlates to a step
while analyzing step count, speed, and distance.
*In its proposed application, the accelerometer will require enough resolution to determine at least a
+/-10% grade. Speed will be characterized by stopped (0 mph), slow (0-2 mph), medium (2-4 mph), high
(5-6 mph), fast (>6 mph). Most noise from this data can be resolved with a moving average filter.

IV. Actuation Plan
The Hiking Buddy will employ a DC motor. This motor has been chosen for its simple
implementation and its ability to generate variable motion. In its proposed application, the motor
will power a fan which will have 3 separate output levels: off, low, high.

V. Software Plan
Data from the MPU will be filtered using a moving-average filter. This is chosen to reduce random
noise used to combat low-pass finite impulse responses. The moving average data filter can be used
to resolve the noise seen in the experiment.
The MPU’s gyroscope can sample at 6kHz, and the accelerometer can sample at a maximum of 1kHz.
A 10-minute mile is 6 miles per hour. This is the minimum speed the system needs to be able to
recognize. 6mph gives approximately 190 steps per minute. The product will be used to sample at
least 3.2 Hz, which correlates to 312.5ms.
Fan behavior is determined by user speed. Speed will be characterized by stopped (0 mph), slow (0-
2 mph), medium (2-4 mph), high (5-6 mph), fast (>6 mph). Speed is based on an average steps per
mile for the user’s height. *(Eventually, it can determine speed based on acceleration and distance.)
If the user switches from fast to slow or medium, the Hiking Buddy will provide low fan power for 60
seconds. If the user sustains a fast pace, stops from any speed, the Hiking Buddy will provide high
fan power until the user starts walking again.



* indicates long-term goals
