# grad_proj
A Behavioral Emulation of an Inertial Reference Unit

# Overview
This repo contains some of the code I've used for my graduation project "Behavioral Implementation of Air Data Inertial Reference Unit on PYNQ Z2 FPGA Platform". If you want the full project folder as is and the report, please contact me via email.

The goal of the project is to calculate the pitch, role, and yaw of the machine at a target frequency of 25 times per second, inline with the frequency of which an AOA (angle of attack) sensor would send AOA data to the flight computers on the Airbus A330. To keep it short, the project contains several components: FPGA block design (or Programmable Logic, 'PL'), Grove drivers, and Jupyter notebook front. The FPGA block design contains a custom designed function map created using Xilinx Vitis HLS, the C++ code used for this is included in the repo (more on "Background"). The Grove drivers that allow sensors to communicate with the Processing System (PS) is slightly modified to increase the read frequency of the sensors. With the modifications to the drivers and the accelerated calculations brought by the custom designed function map. The system achieved a high of 200 calculations per second, and 30 calculations per second in its full implementation.

Upon reviewing this project, I noticed a lot that can be improved. Namely the calculations should be made hard timing deterministic. However, I've decided to leave all the code and components as how it was submitted and graded.

# Aviation Technical Background
An ADIRU is a core component of a modern airliner's flight control system. It receives input from environmental sensors such as pitot tubes, alpha probes, static ports, accelerometers, gyroscopes, GPS, etc. to calculate all values pretaining to the aircraft's relation to its environment, such as its attitute (pitch), roll, heading, AOA, etc. Generally speaking, each ADIRU contains two units: an air data unit and an inertial reference unit. The former calculates results in relation to air, while the latter handles inertial sensing and self-contained navigation. Some flight critical data can't be calculated directly from readings of environmental sensors, such as the mach number (the ratio between the plane's airspeed and the speed of sound the air is moving in) requires not only the reading from the pitot tube for the dynamic air pressure value and the static ports for the static air pressure value to calculate the airspeed, but also further calculations to determine the speed of sound. Hence, an ADIRU has to be both hard timing deterministic in its processes, it also has to be fast.

# Project Tools

# Project Components
This repo contains both the C++ file that Vitis HLS used to generate a custom function map, and the FPGA block design modified from the Pynq Z2 default block design that included the custom function map. Unfortunately the project was more complicated than I anticipated, I ran out of time before I could've implemented the air data portion of the system. Hence, the C++ code used to generate the function map only calculated pitch, roll, and heading. Other components of this project, such as the modified drivers for the Grove IMU module and the Jupyter notebook that allows user to interact with the FPGA fabric (PL) from the PS, were not included. 

The programming style is different from regular high level C++ programming for several reasons: 
1) the function map generated from the C++ code has to fit onto the relatively small FPGA fabric, this is determined by the available LUTs;
2) Vitis HLS synthesis cannot synthesize certain high level programming funtionalities into RTL logic, such as dynamic memory allocation (in extension: recursion programming), variable-length data structures, complex flow control, etc., and;
3) Certain Vitis HLS libraries are more resrictive on the data types of values that can be passed into functions.
