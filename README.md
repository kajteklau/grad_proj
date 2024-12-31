# grad_proj
**Behavioral Emulation of an Inertial Reference Unit**

## Overview
This repository contains code from my graduation project, *"Behavioral Implementation of Air Data Inertial Reference Unit on PYNQ Z2 FPGA Platform"*. For the full project folder and report, contact me via email.

The goal was to calculate pitch, roll, and yaw at 25 Hz, matching the data frequency of an Airbus A330's Angle of Attack (AOA) sensor. Key components include FPGA block design (Programmable Logic or "PL"), modified Grove drivers, and a Jupyter notebook front end. The FPGA block design integrates a custom function map, synthesized from the included `imu_calc_float.cpp` using Xilinx Vitis HLS. Modified Grove drivers enhance sensor read frequencies. The system achieved up to 200 calculations per second (30 Hz in full implementation).

This submission reflects the original project as graded, with potential improvements left for future work (e.g., ensuring hard timing determinism).

## Background
An Air Data Inertial Reference Unit (ADIRU) is a crucial flight control component in modern aircraft, combining air data and inertial sensing for navigation. ADIRUs process sensor inputs (e.g., pitot tubes, gyroscopes) to calculate critical values like pitch, roll, and heading. These systems require fast, deterministic calculations to ensure reliable operation.

## Tools
- **Xilinx Vitis HLS**: Synthesized C++ code into a function map.
- **Xilinx Vivado**: Integrated the function map into a custom block design and generated the bitstream for the PYNQ Z2 FPGA.
- **PYNQ Z2 Board**: The board that hosts the system with the FPGA fabric.
- **Grove IMU 10**: The module that includes an accelerometer, gyroscope, and temperature sensor.

## Project Components
This repo includes:
- `imu_calc_float.cpp`: The C++ file used to generate the custom function map.
- A modified FPGA block design based on the PYNQ Z2 default design.

Other components (e.g., testbench, modified Grove IMU drivers, and Jupyter notebook) are excluded. The air data calculation portion was not implemented due to time constraints; the current system focuses on pitch, roll, and heading calculations.

### Design Constraints
1. **FPGA Resource Limits**: The function map had to fit the small PYNQ Z2 FPGA fabric (LUTs).
2. **Vitis HLS Restrictions**: Dynamic memory allocation, recursion, and complex control flows were avoided. Fixed-point data types were preferred over floating-point for compatibility.
3. **Programming Adjustments**: Workarounds (e.g., breaking formulas into smaller components) were necessary to comply with HLS constraints.

The FPGA block design was streamlined by removing unused peripherals to free LUT space. The custom function map connects to the Processing System (PS) via Xilinx's AXI4 DMA interface, ensuring efficient communication.
