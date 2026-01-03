include "sigpack";
include "math";
include "io";

var wav = sndfile.open("demo/sigpack/thx-sound-test.wav", sndfile.READ);
var x = wav.read();
wav.close();
var x_left = x.row(0).t();
var FFT_SIZE = 1024;
var FFT_OVERLAP = 128;
var P = 10 * log10(sigpack.specgram(x_left, FFT_SIZE, FFT_OVERLAP).abs);
var Q = P.rows(FFT_SIZE / 2, FFT_SIZE - 1);
var gp0 = sigpack.gplot();
gp0.window("Deep note spectrogram", 100, 100, 1200, 4000);
gp0.send2gp("unset tics");
gp0.send2gp("unset colorbox");
gp0.image(Q);
