include "sigpack";
include "math";
// Number of samples
var N = 200;

// Create a FIR filter and adaptive
var G = FIR_filt();
var Ghat = FIR_filt();

// Filter coeffs.
var b = vec();
b.add(-0.2, -0.1, 0.1, 0.3, 0.7);
G.set_coeffs(b);
var M = b.size();

// Setup adaptive filter
//	Ghat.setup_rls(M, 0.95, 50);
//	Ghat.setup_nlms(M,0.5,0.001);
Ghat.setup_lms(M, 0.05);

// Signal vectors
var x = vec(N, randn); // Input sig
var y = vec(N, zeros); // Model sig
var z = vec(N, randn); // Measurement noise
var e = vec(N, zeros); // Err sig

// Log matrix
var Wlog = mat(M, N);

// Apply G to input signal and add some measurement noise
var d = G.filter(x) + 0.0001 * z;

// Filter - sample loop
for (var n = 0; n < N; n = n + 1) {
    // Apply adaptiv filter
    y[n] = Ghat.step(x[n]);

    // Calc error
    e[n] = d[n] - y[n];

    // Update filter
    //		Ghat.rls_adapt(e(n));
    //		Ghat.nlms_adapt(e(n));
    Ghat.lms_adapt(e[n]);

    // Save to log
    Wlog.col(n, Ghat.get_coeffs());
}

// Display result
println("Filter coeffs:    ", b.t()); // expect: Filter coeffs:      -0.2000  -0.1000   0.1000   0.3000   0.7000
// expect:
println("Estimated coeffs: ", Ghat.get_coeffs().t()); // expect: Estimated coeffs:   -0.2000  -0.1000   0.0999   0.3000   0.7000
// expect:
var gp0 = gplot();
var gp1 = gplot();
gp0.window("Plot", 10, 10, 500, 500);
//    gp0.set_output("Wlog.png");
gp0.plot_add_mat(Wlog, "b");
gp0.plot_show();

gp1.window("Plot2", 600, 10, 500, 500);
//    gp1.set_output("LMS_err.png");
var J = 10 * log10(e % e);
gp1.plot_add(J, "|Error|^2");
gp1.plot_show();

