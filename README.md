Grove - 6-Axis Digital Accelerometer&Gyroscope ±40g (ADIS16470)
=================================  

![Grove - 6-Axis Digital Accelerometer&Gyroscope ±40g (ADIS16470)
](https://github.com/Jenkinlu001/Seeed_Picture/blob/master/ADIS16470.jpg)  


Introduction of sensor
----------------------------  
>1.  The ADIS16470 is a miniature MEMS inertial measurement unit (IMU) that includes a triaxial gyroscope and a triaxial accelerometer. The ADIS16470 provides a simple, cost effective method for integrating accurate, multiaxis inertial sensing into industrial systems, especially when compared with the complexity and investment associated with discrete designs.
>2.  Triaxial, digital gyroscope, ±2000°/sec dynamic range,8°/hr in run bias stability ,0.008°/sec/√Hz rms rate noise density ,Triaxial, digital accelerometer dynamic range: ±40 g ,13 µg in run bias stability .

***
Usage:
==========  
Download all the source files and open BurstRead_demo/BurstRead_demo.ino in arduino IDE. Compile and download and run it on a arduino board.

****
Notice:
=========
>1. This BurstRead_demo/BurstRead_demo.ino is just a simple display demo for general use case.
>2. Here is the raw data of the sensor, and the application needs to process the data by itself.
>3. In the middle, STM32 chip is used as the adapter. If necessary, the basic parameters of the sensor should be adjusted, files Stm32F030_Firmware/Stm32F030_Firmware.ino should be rewritten, and the firmware should be re-written by SWD.



***
This software is written by Jenkin for seeed studio<br>
Email:zhengxing.lu@seeed.cc
and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt for more information.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed Studio is an open hardware facilitation company based in Shenzhen, China. <br>
Benefiting from local manufacture power and convenient global logistic system, <br>
we integrate resources to serve new era of innovation. Seeed also works with <br>
global distributors and partners to push open hardware movement.<br>
