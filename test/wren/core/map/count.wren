var map = Map();
print map.size(); // expect: 0
map["one"] = "value";
print map.size(); // expect: 1
map["two"] = "value";
print map.size(); // expect: 2
map["three"] = "value";
print map.size(); // expect: 3
print map; // expect: {"one":"value","three":"value","two":"value"}
// Adding existing key does not increase count.
map["two"] = "new value";
print map.size(); // expect: 3
print map; // expect: {"one":"value","three":"value","two":"new value"}
