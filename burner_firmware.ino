#define PROG_MOSI 3
#define PROG_MISO 4
#define PROG_SCK 5
#define PROG_RST 6


/**
 * Envia dados SPI. O MSB é enviado primeiro.
 */
uint8_t send_data(uint8_t data);


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

uint8_t send_data(uint8_t data)
{
  // inicia ack com 0
  uint8_t ack = 0;
  int i;
  for(i = 0; i<7; i++) {

    digitalWrite(PROG_SCK, LOW);

    if(data & (1 << (7 - i)) ) { 
      digitalWrite(PROG_MOSI, HIGH);
    }
    else {
      digitalWrite(PROG_MOSI, LOW);
    }

    // Pulso alto no SCK
    digitalWrite(PROG_SCK, HIGH);
    delayMicroseconds(3);

    // Lê o bit de ACK e aciona o bit respectivo
    // no ack. Note que recebemos primeiro o MSB.
    if(digitalRead(PROG_MISO)) {
      ack |= (1 << (7-i));
    }

    // parte baixa do pulso
    digitalWrite(PROG_SCK, LOW);
    delayMicroseconds(3);
  }
}

