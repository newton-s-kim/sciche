include "json";

var dict = Map();
dict["name"] = "John Doe";
dict["age"] = 30;
dict["city"] = "New York";
dict["is_student"] = false;
dict["hobbies"] = List("reading", "gaming", "coding");

var str = json.dump(dict);

print str; // expect: {"hobbies":["reading","gaming","coding"],"is_student":false,"age":30.0,"city":"New York","name":"John Doe"}
