include "sigpack";
include "math";

var N = 25000;
var NFFT = 512;
var n = vec(N);

var gp0 = sigpack.gplot();
var gp1 = sigpack.gplot();
var gp2 = sigpack.gplot();
gp0.window("Spectrum", 10, 10, 500, 400);
gp1.window("Time", 550, 10, 900, 400);
gp2.window("Spectrogram", 10, 450, 1440, 500);

var t = sigpack.linspace(0, N - 1, N);
var f = 0.01 + 0.45 * t / N;
n.randn();
var x = sin((pi * f) % t) + 0.01 * n;

// Calc spectrum
var S = 10 * log10(sigpack.pwelch(x, NFFT, NFFT / 2));

// Calc spectrogram
var P = 10 * log10(sigpack.specgram(x, NFFT, NFFT / 4).abs);

x.resize(2000);
t.resize(2000);

gp0.send2gp("set xtics (\"-Fs/2\" 1,\"0\" 256,\"Fs/2\" 512)"); // Direct call to gnuplot pipe
gp0.ylabel("Power");
gp0.xlabel("Frequency");
gp0.plot_add(S, "TestSig");
gp0.plot_show();

gp1.ylabel("Amplitude");
gp1.xlabel("Time");
gp1.plot_add(t, x, "TestSig");
gp1.plot_show();

gp2.ylabel("Frequency");
gp2.xlabel("Time");
gp2.image(P);

