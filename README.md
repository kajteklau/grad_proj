# grad_proj
A Behavioral Emulation of an Inertial Reference Unit

# Overview
This repo contains some of the code I've used for my graduation project "Behavioral Implementation of Air Data Inertial Reference Unit on PYNQ Z2 FPGA Platform". If you want the full project folder as is and the report, please contact me via email.

The goal of the project is to calculate the pitch, role, and yaw of the machine at a target frequency of 25 times per second, inline with the frequency of which an AOA (angle of attack) sensor would send AOA data to the flight computers on the Airbus A330. The project contains several components: FPGA block design (or Programmable Logic, 'PL'), Grove drivers, and Jupyter notebook front. The FPGA block design contains a custom designed function map created using Xilinx Vitis HLS, the C++ code used for this is included in the repo (more on "Background"). The Grove drivers that allow sensors to communicate with the Processing System (PS) is slightly modified to increase the read frequency of the sensors. With the modifications to the drivers and the accelerated calculations brought by the custom designed function map. The system achieved a high of 200 calculations per second, and 30 calculations per second in its full implementation.

Upon reviewing this project, I noticed a lot that can be improved. Namely the calculations should be made hard timing deterministic. However, I've decided to leave all the code and components as how it was submitted and graded.

# Aviation Technical Background
An ADIRU is a core component of a modern airliner's flight control system. It receives input from environmental sensors such as pitot tubes, alpha probes, static ports, accelerometers, gyroscopes, GPS, etc. to calculate all values pretaining to the aircraft's relation to its environment, such as its attitute (pitch), roll, heading, AOA, etc. Generally speaking, each ADIRU contains two units: an air data unit and an inertial reference unit. The former calculates results in relation to air, while the latter handles inertial sensing and self-contained navigation. Some flight critical data can't be calculated directly from readings of environmental sensors, such as the mach number (the ratio between the plane's airspeed and the speed of sound the air is moving in) requires not only the reading from the pitot tube for the dynamic air pressure value and the static ports for the static air pressure value to calculate the airspeed, but also further calculations to determine the speed of sound. Hence, an ADIRU has to be both hard timing deterministic in its processes, it also has to be fast.

# Project Tools
Xilinx Vitis HLS was used to synthesize the C++ code into a function map. Xilinx Vivado was used to implement the custom function map into a block design and generate the bitstream to program the PYNQ Z2 FPGA fabric.

# Project Components
This repo contains both "imu_calc_float.cpp", the C++ file that Vitis HLS used to generate a custom function map, and the FPGA block design modified from the Pynq Z2 default block design that included the custom function map. Other components of this project, such as the testbench, the modified drivers for the Grove IMU module and the Jupyter notebook that allows user to interact with the FPGA fabric (PL) from the PS, were not included. Unfortunately the project was more complicated than I anticipated, I ran out of time before I could've implemented the air data portion of the system. Hence, "imu_calc_float.cpp" only calculated pitch, roll, and heading. 

"imu_calc_float.cpp" was written with several design constraints in mind: 
1) the function map generated from the C++ code has to fit onto the relatively small FPGA fabric, this is determined by the available LUTs;
2) Vitis HLS synthesis cannot synthesize certain high level programming funtionalities into RTL logic, such as dynamic memory allocation (in extension: recursion programming), variable-length data structures, complex flow control, etc., and;
3) Certain Vitis HLS libraries are more resrictive on the data types of values that can be passed into functions.
Hence, it necessitates certain programming decisions such as working around HLS_math.h data type limitations by breaking compound formulas apart with XXtemp, XXtemp1, XXtemp2 suffixes, and reducing variable data sizes by using fixed point data tpe as defined in HLS_math.h instead of floating point as defined in C++.

The FPGA block design was modified from the block design of the PYNQ Z2 image. Unused peripherals and functions are removed from the design to free up LUT space for the function map. Since both the original block design and the function map use AXI4-interconnect interface, the function map can be connected directly to the PS via Xilinx's direct memory access (DMA) IP.

