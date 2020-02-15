# BackToTheFutureClock
Reproduction of the DeLorean's time display from the famous movie Back to the Future

![Back to the Future Clock in Action 2020](https://github.com/yildi1337/BackToTheFutureClock/blob/master/pictures/running_2020.jpg)

This project is based on a 32-bit ARM Cortex-M3 microcontroller *STM32F103RET6*. All seven-segment displays are controlled by several *MAX6955* LED display drivers. Please note that, unfortunately, all r(6) and s(14) pins of the LED display drivers are swaped in the schematics and on the board which has been corrected utilizing some copper wires:

![Bottom side of the PCB](https://github.com/yildi1337/BackToTheFutureClock/blob/master/pictures/pcb_bottom_2.jpg)

For historical reasons, the bottom line of the display (last time departed) is static and shows the same date/time as in the movie.

The center line (present time) shows the current date/time based on a real-time clock *PCF8583* and date/time information aquired via a ferrite antenna from the longwave time signal and standard-frequency radio station DCF77.

The date/time in the top line (destination time) of the display is user programmable. Originally, I programmed it based on the infrared receiver with the remote of my television. The idea was to set an alarm (firmware/version_advanced). However, I never really used that feature. Thus, there is a light version of the software (firmware/version_light) without the infrared functionality and also without controlling the the alarm and bluetooth circuits.

If you have any questions, feel free to contact me!