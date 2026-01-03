include "sigpack";

// Filter coeffs.

var b = vec();
var N = 15;

// Create a FIR filter
var fir_filt = sigpack.FIR_filt();
var b = sigpack.fir1(7, 0.35);
fir_filt.set_coeffs(b);

var X = vec(N);
X[0] = 1;

var Y = vec(N);

for(var n = 0; n < N; n = n + 1) {
    Y[n] = fir_filt.step(X[n]);
}

print "Filter coeffs: "; // expect: Filter coeffs: 
print b.t(); // expect:   -0.0051   0.0134   0.1474   0.3444   0.3444   0.1474   0.0134  -0.0051
// expect: 

print "Impulse response:"; // expect: Impulse response:
print Y.t(); // expect:   -0.0051   0.0134   0.1474   0.3444   0.3444   0.1474   0.0134  -0.0051        0        0        0        0        0        0        0
// expect: 

var iir_filt = sigpack.IIR_filt();
var b = vec();
var a = vec();
b.add(0.25, 0.5, 0.25);
a.add(1.0, -0.9);

iir_filt.set_coeffs(b, a);
Y = iir_filt.filter(X);

print "IIR theor. impulse response: "; // expect: IIR theor. impulse response: 
print "   0.2500   0.7250   0.9025   0.8123   0.7310   0.6579   0.5921   0.5329   0.4796   0.4317   0.3885 ..."; // expect:    0.2500   0.7250   0.9025   0.8123   0.7310   0.6579   0.5921   0.5329   0.4796   0.4317   0.3885 ...
print "IIR output: "; // expect: IIR output: 
print Y.t(); // expect:    0.2500   0.7250   0.9025   0.8123   0.7310   0.6579   0.5921   0.5329   0.4796   0.4317   0.3885   0.3496   0.3147   0.2832   0.2549
// expect:
var del = sigpack.Delay(3);
print "Delay three samples:"; // expect: Delay three samples:
print del.delay(Y).t(); // expect:         0        0        0   0.2500   0.7250   0.9025   0.8123   0.7310   0.6579   0.5921   0.5329   0.4796   0.4317   0.3885   0.3496
// expect:
