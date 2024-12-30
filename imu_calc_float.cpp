#include "imu_calc_float.h"
#include "hls_stream.h"

using namespace hls;

void imu_cal_float(
    // Acceleration vector streams
    mystream &stream_ax, mystream &stream_ay,
    // Magnitude vector streams
    mystream &stream_mx, mystream &stream_my,
    // Calculated environment data streams
    mystream_out &stream_pitchout, mystream_out &stream_rollout, mystream_out &stream_yawout
) {
    // Control protocol: ap_ctrl_none is required for task-level parallelism
    #pragma HLS INTERFACE ap_ctrl_none port=return

    // Acceleration vector streams
    #pragma HLS INTERFACE axis port=stream_ax
    #pragma HLS INTERFACE axis port=stream_ay

    // Magnitude vector streams
    #pragma HLS INTERFACE axis port=stream_mx
    #pragma HLS INTERFACE axis port=stream_my

    // Output result streams
    #pragma HLS INTERFACE axis port=stream_pitchout
    #pragma HLS INTERFACE axis port=stream_rollout
    #pragma HLS INTERFACE axis port=stream_yawout

    #pragma HLS DATAFLOW
    #pragma HLS INLINE

    // Temporary variables for input data
    data_t axtemp, aytemp, mxtemp, mytemp;

    // Temporary variables for calculated data
    result_t pitchtemp, rolltemp, yawtemp;

    // Read input stream data
    readStream(stream_ax, axtemp);
    readStream(stream_ay, aytemp);
    readStream(stream_mx, mxtemp);
    readStream(stream_my, mytemp);

    // Perform calculations
    doComputing(
        axtemp, aytemp,
        mxtemp, mytemp,
        pitchtemp, rolltemp, yawtemp
    );

    // Write computed data to output streams
    writeStream(pitchtemp, stream_pitchout);
    writeStream(rolltemp, stream_rollout);
    writeStream(yawtemp, stream_yawout);
}

void readStream(mystream &stream, data_t &out) {
    #pragma HLS PIPELINE II=1
    out = stream.read();
}

void writeStream(result_t &out_out, mystream_out &stream_out) {
    #pragma HLS PIPELINE II=1
    stream_out.write(out_out);
}

void doComputing(
    data_t &axtemp, data_t &aytemp,
    data_t &mxtemp, data_t &mytemp,
    result_t &pitchtemp, result_t &rolltemp, result_t &yawtemp
) {
    #pragma HLS PIPELINE II=5

    // Temporary variables for calculations
    calpitch ax, ay, paxtemp, ptemp, rtemp1, rtemp2;
    rawin_t mx, my;
    float p, r, h;

    // Assign input data to temporary variables
    ax = calpitch(axtemp.data);
    ay = calpitch(aytemp.data);
    mx = rawin_t(mxtemp.data);
    my = rawin_t(mytemp.data);

    // Pitch calculation
    paxtemp = 1 - ax * ax;
    paxtemp = hls::sqrt(paxtemp);
    paxtemp = -ax / paxtemp;
    ptemp = hls::atan(paxtemp);
    p = static_cast<float>(halfangle * hls::atanpi(paxtemp));
    if (p < -90) {
        p += 96;
    }

    // Roll calculation
    rtemp1 = ay / hls::cos(ptemp);
    if (rtemp1 > 1 || rtemp1 < -1) {
        r = 0;
    } else {
        rtemp2 = 1 - rtemp1 * rtemp1;
        rtemp2 = hls::sqrt(rtemp2);
        rtemp2 = rtemp1 / rtemp2;
        r = static_cast<float>(halfangle * hls::atanpi(rtemp2));
        if (r < -90) {
            r += 96;
        }
    }

    // Yaw calculation
    h = static_cast<float>(halfangle * hls::atan2(my, mx) / pie);
    if (h < 0) {
        h += 360;
    }

    // Assign results to output variables
    pitchtemp.data = p;
    rolltemp.data = r;
    yawtemp.data = h;

    // Pass metadata from input to output
    pitchtemp.last = mytemp.last;
    pitchtemp.keep = mytemp.keep;
    pitchtemp.dest = mytemp.dest;
    pitchtemp.id = mytemp.id;
    pitchtemp.strb = mytemp.strb;
    pitchtemp.user = mytemp.user;

    rolltemp.last = mytemp.last;
    rolltemp.keep = mytemp.keep;
    rolltemp.dest = mytemp.dest;
    rolltemp.id = mytemp.id;
    rolltemp.strb = mytemp.strb;
    rolltemp.user = mytemp.user;

    yawtemp.last = mytemp.last;
    yawtemp.keep = mytemp.keep;
    yawtemp.dest = mytemp.dest;
    yawtemp.id = mytemp.id;
    yawtemp.strb = mytemp.strb;
    yawtemp.user = mytemp.user;
}
