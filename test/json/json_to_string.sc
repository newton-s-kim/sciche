include "scjson";

var dict = Map();
dict["name"] = "John Doe";
dict["age"] = 30;
dict["city"] = "New York";
dict["is_student"] = false;
dict["hobbies"] = List("reading", "gaming", "coding");

var str = json.dump(dict);

print str; // expect: {"age":30.0,"city":"New York","hobbies":["reading","gaming","coding"],"is_student":false,"name":"John Doe"}
