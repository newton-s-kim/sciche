include "sigpack";
include "math";

// Number of samples
var N = 400;

// Create a FIR filter and adaptive
var G = sigpack.FIR_filt();
var Ghat = sigpack.FIR_filt();

// Filter coeffs.
var b = vec();
b.add(-0.2, -0.1, 0.1, 0.3, 0.7);
var c = vec();
c.add(-0.4, 0.0, 0.2, 0.1, 0.4);
G.set_coeffs(b);
var M = b.size;

// Setup adaptive filter
//	Ghat.setup_rls(M, 0.8, 5);
//	Ghat.setup_nlms(M,0.6,0.001);
//	Ghat.setup_lms(M,0.05);
Ghat.setup_kalman(M, 10, 1, 5);
//	Ghat.setup_newt(M,0.99,0.01,15);

// Signal vectors
var x = vec(N, randn); // Input sig
var y = vec(N, zeros); // Model sig
var d = vec(N, zeros); // Output sig
var z = vec(N, randn); // Measurement noise
var e = vec(N, zeros); // Err sig

// Log matrix
var Wlog = mat(M, N);

// Apply G to input signal and add some measurement noise
for (var n = 0; n < N; n = n + 1) {
    if (n == 200)
        G.update_coeffs(c);
    d[n] = G.step(x[n]) + 0.0001 * z[n];
}

// Filter - sample loop
for (var n = 0; n < N; n = n + 1) {
    // Apply adaptiv filter
    y[n] = Ghat.step(x[n]);

    // Calc error
    e[n] = d[n] - y[n];

    // Update filter
    //		Ghat.rls_adapt(e(n));
    //		Ghat.nlms_adapt(e(n));
    //		Ghat.lms_adapt(e(n));
    Ghat.kalman_adapt(e[n]);
    //		Ghat.newt_adapt(e(n));
    // Save to log
    Wlog.col(n, Ghat.get_coeffs());
}

// Display result
println("Filter coeffs:    ", c.t()); // expect: Filter coeffs:      -0.4000        0   0.2000   0.1000   0.4000
// expect:
println("Estimated coeffs: ", Ghat.get_coeffs().t()); // expect: Estimated coeffs:   -4.0001e-01  -9.2298e-05   2.0005e-01   9.9962e-02   3.9991e-01
// expect:

var gp0 = sigpack.gplot();
var gp1 = sigpack.gplot();
gp0.window("Plot", 10, 10, 500, 500);
//    gp0.set_output("Wlog.png");
gp0.plot_add_mat(Wlog, "b");
gp0.plot_show();

gp1.window("Plot2", 600, 10, 500, 500);
//    gp1.set_output("LMS_err.png");
var J = 10 * log10(e % e);
gp1.plot_add(J, "|Error|^2");
gp1.plot_show();


