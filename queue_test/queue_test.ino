#include "Queue.h"

Queue<byte> queue = Queue<byte>(8);


void setup() {
  Serial.begin(9600);
  queue.push(13);
  queue.push(2);
  queue.push(93);
  queue.push(4);
  queue.push(25);
  queue.push(6);
  queue.push(44);
  queue.push(8);
}

void loop() {
  
  
  int count = queue.count();
  if (count > 0) {
    Serial.print("Found ");
    Serial.print(count);
    Serial.print(" items.. '");
    Serial.print(queue.pop());
    Serial.print("' is the oldest. We are ");
    Serial.print(queue.front());
    Serial.print(" in front and ");
    Serial.print(queue.back());
    Serial.print(" in back. Next is.. '");
    Serial.print(queue.peek());
    Serial.println("'.");
  } else {
  }
  delay(20000);
}
