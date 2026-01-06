var words = "";
var a = List("One", "Two", "Three");
fun b(word) {
  //println(words,"+=",word);
  words = words + word;
}
a.each(b);
print words; // expect: OneTwoThree
