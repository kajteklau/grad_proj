# grad_proj
A Behavioral Emulation of an Inertial Reference Unit

# Overview
This repo contains some of the code I've used for my graduation project "Behavioral Implementation of Air Data Inertial Reference Unit on PYNQ Z2 FPGA Platform". If you want the full project folder as is and the report, please contact me via email.

The goal of the project is to calculate the pitch, role, and yaw of the machine at a target frequency of 25 times per second, inline with the frequency of which an AOA (angle of attack) sensor would send AOA data to the flight computers on the Airbus A330. To keep it short, the project contains several components: FPGA block design (or Programmable Logic, 'PL'), Grove drivers, and Jupyter notebook front. The FPGA block design contains a custom designed function map created using Xilinx Vitis HLS, the C++ code used for this is included in the repo (more on "Background"). The Grove drivers that allow sensors to communicate with the Processing System (PS) is slightly modified to increase the read frequency of the sensors (more on "Design"). With the modifications to the drivers and the accelerated calculations brought by the custom designed function map. The system achieved a high of 200 calculations per second, and 30 calculations per second in its full implementation.

Upon reviewing this project, I noticed a lot that can be improved. Namely the calculations should be made hard timing deterministic. However, I've decided to leave all the code and components as how it was submitted and graded.

# Background
An ADIRU is a core component of a modern airliner's flight control system. It receives input from environmental sensors such as pitot tubes, alpha probes, static ports, accelerometers, gyroscopes, GPS, etc. to calculate all values pretaining to the aircraft's relation to its environment, such as its attitute (pitch), role, yaw, heading, etc. 
