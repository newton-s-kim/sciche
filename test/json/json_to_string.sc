include "scjson";

var dict = Map();
dict["name"] = "John Doe";
dict["age"] = 30;
dict["city"] = "New York";
dict["is_student"] = false;
dict["hobbies"] = List("reading", "gaming", "coding");

var json = json();

var str = json.dump(dict);

print str; //expect: "{\"name\": \"John Doe\", \"age\": 30, \"city\": \"New York\", \"is_student\": false, \"hobbies\": [\"reading\", \"gaming\", \"coding\"]}"
