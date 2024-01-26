# GAME-WITH-IMU-AND-PIANO-PLATFORM
In this project, operations are performed on a menu on the LCD according to the ADC value received with the Joystick. The menu includes settings, game and piano options. The game is played with dice. When acceleration above a certain value is measured from the IMU sensor, the dice value is sent to the LEDs. Randomness of the dice is ensured using the Timer0 and Timer1 timers. Two PIC microcontrollers were also used in the project.
The PIC reading the IMU transmits acceleration data to the other PIC via UART. The game is played depending on the incoming data. The PIC on which the game is played sends information to the other PIC via UART whether the piano mode is entered or not. In short, IMU and piano operations are performed on one PIC, while other operations are performed on the other PIC.

![image](https://github.com/Emirhan-KURU/GAME-WITH-IMU-AND-PIANO-PLATFORM/assets/106912277/08df3507-493d-4f01-8bdc-44a67a7baf34)
