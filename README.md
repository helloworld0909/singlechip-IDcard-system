# ID card system based on Arduino singlechip terminal

Architecture:<br>
![](image/ID card system.png)

Flow chart:<br>
1. Payment
![](image/Payment.png)
2. School Bus check-in
![](image/Check-in.png)

Terminal:<br>
* RFID card reader built on Arduino singlechip platform.
* remote control based on IRremote module
* LCD display
* Buzzer

Backend:<br>
* MySQL Database which can store personal information, balance information, etc.
* Serial Communication between terminal and database.

Notice:<br>
* settings.py should be modified to connect to MySQL database.
