#define PROG_MOSI 3
#define PROG_MISO 4
#define PROG_SCK 5
#define PROG_RST 6

#define DUMMY_DATA 0xAA


/**
 * Envia dados SPI. O MSB é enviado primeiro.
 * 
 * param data Dado a ser enviado para o uC.
 * 
 * return Retorna o byte de acknowledge do uC.
 */
uint8_t send_data(uint8_t data);

/**
 * Habilita programação.
 */
void prog_enable();

/**
 * Apaga o programa no uC.
 */
void chip_rease();


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

void prog_enable()
{
  send_data(0xAC);
  send_data(0x53);
  send_data(DUMMY_DATA);
  send_data(DUMMY_DATA);
}

void chip_erase()
{
  send_data(0xAC);
  send_data(0x80);
  send_data(DUMMY_DATA);
  send_data(DUMMY_DATA);
}

