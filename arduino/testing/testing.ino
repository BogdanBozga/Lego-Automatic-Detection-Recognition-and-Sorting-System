#include <LinkedList.h>

// Create a linked list to hold integers
LinkedList<String> list = LinkedList<String>();

void setup() {
  Serial.begin(9600);

  // Add elements to the linked list
  list.add("Ceva");
  list.add("Ceva2");
  list.add("Cev3");

  // Print the elements of the list
  for (int i = 0; i < list.size(); i++) {
    Serial.println(list.get(i));
  }
}

void loop() {
  // Empty loop for this example
}
