# Back To The Future Clock
Reproduction of the DeLorean's time display from the famous movie Back to the Future

![Back to the Future Clock in Action 2020](https://github.com/yildi1337/BackToTheFutureClock/blob/master/pictures/running_2020.jpg)

This project is based on a 32-bit ARM Cortex-M3 microcontroller *STM32F103RET6*. All seven-segment displays are controlled by several *MAX6955* LED display drivers. Please note that, unfortunately, all r(6) and s(14) pins of the LED display drivers are swaped in the schematics and on the board which has been corrected utilizing some copper wires:

![Bottom side of the PCB](https://github.com/yildi1337/BackToTheFutureClock/blob/master/pictures/pcb_bottom_2.jpg)

For historical reasons, the bottom line of the display (last time departed) is static and shows the same date/time as in the movie.

The center line (present time) shows the current date/time based on a real-time clock *PCF8583* and date/time information aquired via a ferrite antenna from the longwave time signal and standard-frequency radio station DCF77.

The date/time in the top line (destination time) of the display is user programmable. Originally, I programmed it based on the infrared receiver with the remote of my television. The idea was to set an alarm (firmware/version_advanced). However, I never really used that feature. Thus, there is a light version of the software (firmware/version_light) without the infrared functionality and also without controlling the the alarm and bluetooth circuits.

If you have any questions, feel free to contact me!

# Update 2023
The Back to the Future watch has now been running for about 9 years without any major problems. However, I have moved in the meantime and the clock is now located in my workshop in the basement. Since the quality of the received DCF77 time signal is too bad here, I decided to adjust the software again.

In the third version of the firmware, the DCF77 functionality is now completely disabled. Instead, the displayed time is based exclusively on the integrated RTC. Since its accuracy is limited and the time would not be correct after some time any more, I put the built-in Bluetooth module into operation for the first time. It is now possible to send the current date/time to the clock via Bluetooth. For this purpose, I built a small Android app in which you only have to click a button to update the time again.

The following picture shows an exemplary measurement of the date/time data received via Bluetooth from the Android app at the output of the Bluetooth module (UART), where the oscilloscope is configured to additionally display the decoded serial data.

![Measurement result (UART at the output of the Bluetooth module)](https://github.com/yildi1337/BackToTheFutureClock/blob/master/pictures/measurement_date_time_via_bluetooth.png)

