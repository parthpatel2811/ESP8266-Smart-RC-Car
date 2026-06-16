# ESP8266-Smart-RC-Car
ESP8266-based WiFi RC Car with dual control modes: Manual and Auto. Control the car through a web interface or RemoteXY mobile app. Features obstacle detection and avoidance using an HC-SR04 ultrasonic sensor, wireless communication, and motor control via an L298N driver, demonstrating IoT and embedded systems concepts.

# Features

✅ WiFi-based Web Control
✅ Mobile App Control using RemoteXY
✅ Manual Driving Mode
✅ Automatic Obstacle Avoidance Mode
✅ ESP8266 Access Point Control
✅ Ultrasonic Distance Measurement

# Components Used
| Component                 | Quantity |
| ------------------------- | -------- |
| ESP8266 NodeMCU           | 1        |
| L298N Motor Driver        | 1        |
| HC-SR04 Ultrasonic Sensor | 1        |
| DC Gear Motors            | 2        |
| Chassis                   | 1        |
| Battery Pack              | 1        |

# Pin Connections
| ESP8266 | L298N |
| ------- | ----- |
| D1      | IN1   |
| D2      | IN2   |
| D5      | IN3   |
| D6      | IN4   |

| ESP8266 | HC-SR04 |
| ------- | ------- |
| D7      | Trigger |
| D0      | Echo    |

# Circuit Diagram
