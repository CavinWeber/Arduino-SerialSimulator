#include <Arduino.h>

#define messageSize 12

int counter = 0;
volatile int counter_interrupt = 0;

byte outputByte = 0x00;

byte mask = 0b00000001;


char output[messageSize] = "UUUUUUUUUUUU";  //'U' is 0b01010101


void setup()
{
  //  Serial.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
//  flipper = false;
  
  TCCR0A = (1 << WGM01);                  //CTC mode
  TCCR0B &= (3 << CS00);                     //div8
  OCR0A = F_CPU / 8 * 0.0000100 - 1;  // 50us compare value
  TIMSK0 |= (1 << OCIE0A);                  //if you want interrupt
}

//Testing

void loop()
{
  //PORTB |= 1;  // Toggle trigger test pin on
  //PORTB &= 0b11111110;  // Toggle trigger test pin off
  if (counter_interrupt != counter) // If the ISR was called since our last check
  {
    counter = counter_interrupt; // Reset flag that tells if the ISR has been called since last check
    int byteWalker = counter / 8;
    if (mask == 0)
    {
      mask = 0b00000001;
    }

    if (output[byteWalker] & mask)  // if bitwise AND resolves to true
    {
//      PORTB |= 1; //Trigger Pin On
      outputByte |= 0b00010000; // send 1
//      PORTB &= 0b11111110; //Trigger Pin Off
    }
    else  //if bitwise AND resolves to false
    {
      outputByte &= 0b11101111; // send 0
    }
    mask <<= 1;
  }
}


ISR(TIMER0_COMPA_vect) {
  PORTB = outputByte;
  if (counter < 96)
  {
    counter_interrupt++;
  }
  else
  {
    counter_interrupt = 0;
//    PORTB = 0x00;
  }

}




/*

  bool flipper;

  void setup(){
  pinMode(13, OUTPUT);
  flipper = false;
  TCCR0A = (1 << WGM01);                  //CTC mode
  TCCR0B = (2 << CS00);                     //div8
  OCR0A = F_CPU/8 * 0.0000150 - 1;    // 50us compare value
  TIMSK0 |= (1<<OCIE0A);                    //if you want interrupt
  }



  void loop() {
  // put your main code here, to run repeatedly:

  }

  ISR(TIMER0_COMPA_vect){
    if (flipper){
      PORTB = 0x0;
    }
    else {
      PORTB = 0xFF;
    }
    flipper = !flipper;
  }

*/
