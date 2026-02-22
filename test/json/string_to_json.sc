include "json";

var str = "{\"name\": \"John Doe\", \"age\": 30, \"city\": \"New York\", \"is_student\": false, \"hobbies\": [\"reading\", \"gaming\", \"coding\"]}";

var dict = json.load(str);

print dict; // expect: {"hobbies":["reading","gaming","coding"],"is_student":false,"age":30,"city":"New York","name":"John Doe"}
