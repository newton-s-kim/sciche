include "math";
print sin(0);       // expect: 0
print sin(pi / 2);  // expect: 1

// these should of course be 0, but it's not that precise
print abs(sin(pi)) < 1.0e-15;        // expect: true
print abs(sin(2 * pi)) < 1.0e-15;  // expect: true
