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
var x = vec(N);
x.randn(); // Input sig
var y = vec(N);
y.zeros(); // Model sig
var z = vec(N);
z.randn(); // Measurement noise
var e = vec(N);
e.zeros(); // Err sig

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

