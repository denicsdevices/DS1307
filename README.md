# DS1307

### Arduino Library for I2C Real Time Clock

The Denics Devices DS1307 is an Arduino library for interfacing with the DS1307 real time clock chip. This library is an evolution of Bonezegei_DS1307 library by Bonezegei (Jofel Batutay). The library provides functions to read and write the clock time. The library is based on the datasheet of the DS1307 chip from Analog Devices (Analog Devices, 2015). The library is compatible with all Arduino architectures and can be installed from the Arduino library manager. The library also depends on the Wire library for I2C communication.

The library provides way to use different available Hardware I2C instances or pins on 32 bit ARM microcontrollers like RP2040, RP2350, ESP32 etc.

To initiate library using Wire1 (I2C1) instance on RP2040 initiated on SDA=Pin(6) and SCL=Pin(7)

DS1307 rtc(0x68,6,7,&Wire1);

<h4>Disclaimer</h4>
<p>The code and libraries provided by Denics Devices is intended for informational and educational purposes only. Denics Devices does not own or manufacture the hardware associated with the code. The code is provided "as is" without any warranty, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non-infringement. In no event shall Denics Devices be liable for any claim, damages, or other liability arising from the use of the code or the hardware. Users are solely responsible for ensuring that their use of the code complies with all applicable laws and regulations.</p>

### Other Resources

[Bonezegei_DS1307 library by Bonezegei](https://github.com/bonezegei/Bonezegei_DS1307)

#Denics Devices
