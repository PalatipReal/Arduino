#include <pthread.h>
 
void *printThreadId(void *threadid) {
  while(1) {Serial.println((int)threadid); delay(500);}
}
 
void setup() {
 
   Serial.begin(9600);
 
   pthread_t threads[4];
   int returnValue;
 
   for( int i = 0; i< 4; i++ ) {
 
      returnValue = pthread_create(&threads[i], NULL, printThreadId, (void *)i);
 
      if (returnValue) {
         Serial.println("An error has occurred");
      }
   }
 
}
 
void loop() {}