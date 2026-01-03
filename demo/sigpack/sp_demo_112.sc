include "sigpack";
include "math";
// Number of samples
var Nsamp = 120;
var N = 6; // Nr of states
var M = 2; // Nr of measurements
var L = 1; // Nr of inputs

// Instatiate a Kalman Filter
var kalman = sigpack.KF(N, M, L);

// Initialisation and setup of system
var P0 = 100;
var Q0 = 0.00003;
var R0 = 25;

// Meas interval
var dT = 0.1;

var x = mat();
x.set(0, 10, 1, 50, -0.08, -9);
kalman.set_state_vec(0.9 * x.t());
// [X,Y,Vx,Vy,Ax,Ay] use position, velocity and acceleration as states
var A = mat();
A.set(1, 0, dT, 0, dT * dT / 2, 0, nil,
               0, 1, 0, dT, 0, dT * dT / 2, nil,
               0, 0, 1, 0, dT, 0, nil,
               0, 0, 0, 1, 0, dT, nil,
               0, 0, 0, 0, 1, 0, nil,
               0, 0, 0, 0, 0, 1);
kalman.set_trans_mat(A);

var H = mat();
H.set(1, 0, 0, 0, 0, 0, nil,
      0, 1, 0, 0, 0, 0);
kalman.set_meas_mat(H);

var P = P0 * mat(N, N, eye);
kalman.set_err_cov(P);
var Q = mat(N, N, zeros);
Q[N - 2, N - 2] = 1;
Q[N - 1, N - 1] = 1;
Q = Q0 * Q;
kalman.set_proc_noise(Q);

var R = R0 * mat(M, M, eye);
kalman.set_meas_noise(R);

// Create simulation data
var z = mat(M, Nsamp, zeros);
var z0 = mat(M, Nsamp, zeros);
var xx = x.t();
for (var n = 1; n < Nsamp; n = n + 1) {
    xx = A * xx + 0.1 * Q * mat(N, 1, randn);
    z0.col(n, H * xx); //
}

z.row(0, z0.row(0) + 0.001 * R0 * mat(1, Nsamp, randn));
z.row(1, z0.row(1) + 0.8 * R0 * mat(1, Nsamp, randn));

var x_log = mat(N, Nsamp);
var e_log = mat(M, Nsamp);
var P_log = cube(N, N, Nsamp);
var xs_log = mat(M, Nsamp);
var Ps_log = cube(N, N, Nsamp);

// Kalman filter loop
for (var n = 0; n < Nsamp; n = n + 1) {
    kalman.predict();
    kalman.update(z.col(n));

    x_log.col(n, kalman.get_state_vec());
    e_log.col(n, kalman.get_err());
    P_log.slice(n, kalman.get_err_cov());
}

// RTS smoother
kalman.rts_smooth(x_log, P_log, xs_log, Ps_log);

// Display result
var gp0 = sigpack.gplot();
gp0.window("Plot", 10, 10, 700, 500);
gp0.plot_add(z0.row(0), z0.row(1), "True Y", "lines dashtype 2 linecolor \"black\"");
gp0.plot_add(z.row(0), z.row(1), "Meas Y", "points");
gp0.plot_add(x_log.row(0), x_log.row(1), "Kalman");
gp0.plot_add(xs_log.row(0), xs_log.row(1), "RTS smooth");
gp0.plot_show();


