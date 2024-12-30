#ifndef __IMU_CAL_FLOAT__
#define __IMU_CAL_FLOAT__

#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include "ap_fixed.h"
#include "hls_math.h"

// Fixed-point data type definitions
typedef ap_fixed<16, 9> rawin_t;               // Raw input data type

// Constants
const ap_ufixed<16, 2> PI = 3.14159265358;     // Value of π

// Stream definitions for raw input data
typedef hls::axis<float, 1, 1, 1> data_t;      // AXI stream data type
typedef hls::stream<data_t> mystream;          // AXI stream for raw input

// Stream definitions for output data
typedef ap_fixed<32, 10> result_t;             // Result data type
typedef hls::axis<float, 1, 1, 1> axis_result_t; // AXI stream data type for output
typedef hls::stream<axis_result_t> mystream_out; // AXI stream for output data

// Top function prototype
void imu_cal_float(
    // Acceleration vector streams
    mystream &stream_ax, mystream &stream_ay,
    // Magnitude vector streams
    mystream &stream_mx, mystream &stream_my,
    // Calculated environment data streams
    mystream_out &stream_pitchout, mystream_out &stream_rollout, mystream_out &stream_yawout
);

// Helper function to read from the input stream
void readStream(mystream &stream, data_t &out);

// Computation function to process input data and generate results
void doComputing(
    data_t &axtemp, data_t &aytemp,
    data_t &mxtemp, data_t &mytemp,
    result_t &pitchtemp, result_t &rolltemp, result_t &yawtemp
);

// Helper function to write to the output stream
void writeStream(result_t &out_out, mystream_out &stream_out);

#endif // __IMU_CAL_FLOAT__
