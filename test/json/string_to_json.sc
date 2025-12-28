include "scjson";

var ijson = json();

var str = "{\"name\": \"John Doe\", \"age\": 30, \"city\": \"New York\", \"is_student\": false, \"hobbies\": [\"reading\", \"gaming\", \"coding\"]}";

var dict = ijson.load(str);

print dict; // expect: {"age":30, "city":"New York", "hobbies":["reading", "gaming", "coding"], "is_student":false, "name":"John Doe"}
