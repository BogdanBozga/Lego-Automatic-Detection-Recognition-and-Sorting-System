#include <LinkedList.h>

// Create a linked list to hold integers
LinkedList<int> pieces_distance = LinkedList<int>();

void setup() {
  Serial.begin(115200);
  delay(1000);
  // Add elements to the linked list
  pieces_distance.add(10);
  pieces_distance.add(20);
  pieces_distance.add(30);

  // Print the elements of the list
  for (int i = 0; i < pieces_distance.size(); i++) {
    Serial.println(pieces_distance.get(i));
  }
}

void loop() {
  for(int i=0;i<pieces_distance.size();i++){
        int distance = pieces_distance.get(i) - 10;
        // piece_distance.remove()
        pieces_distance.set(i,pieces_distance.get(i) - 10); // modify the piece
        Serial.print(pieces_distance.get(i));
        Serial.print("_");
  }
  Serial.println("__");
  delay(3000);
}
