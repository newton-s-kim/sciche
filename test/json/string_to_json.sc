include "json";

var str = "{\"name\": \"John Doe\", \"age\": 30, \"city\": \"New York\", \"is_student\": false, \"hobbies\": [\"reading\", \"gaming\", \"coding\"]}";

var dict = json.load(str);

print dict; // expect: {"city":"New York","age":30,"is_student":false,"name":"John Doe","hobbies":["reading","gaming","coding"]}
