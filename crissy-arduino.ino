#define PROG_MOSI 3
#define PROG_MISO 4
#define PROG_SCK 5
#define PROG_RST 6

#define DUMMY_DATA 0xAA

uint8_t high_addr = 0x00;
uint8_t low_addr = 0x00;

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

/**
 * Escreve na memoria de programa no modo byte.
 * 
 * param addr Endereço da memoria de programa de 2 bytes.
 * param data Data para ser gravado na memoria de programa.
 * param data Dado a ser gravado na memoria.
 */
void write_progmem_byte(uint16_t addr, uint8_t data);

/**
 * Escreve na memoria de programa no modo byte.
 * 
 * param high_addr Endereço alto da memoria de programa.
 * param low_addr Endereço baixo da memoria de programa.
 * param data Dado a ser gravado na memoria.
 */
void write_progmem_byte(uint8_t high_addr, uint8_t low_addr, uint8_t data);

void setup() {

  // Debug
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  pinMode(PROG_MOSI, OUTPUT);
  pinMode(PROG_MISO, INPUT);
  pinMode(PROG_SCK, OUTPUT);
  pinMode(PROG_RST, OUTPUT);

  // Gravação
  
  // 1: RST -> High
  digitalWrite(PROG_RST, LOW);
  //delay(1);

  // 2: Programming Enable
  //prog_enable();

  // 3: Chip Erase
 // chip_erase();

  // 4: Gravar
  //write_progmem_byte(0x0000, 0xC2);
  //write_progmem_byte(0x0001, 0xA0);
}

void loop() {
  if(Serial.available() > 0) {
    char c = Serial.read();

    switch(c)
    {
      case 's':
        Serial.println("Vou habilitar gravacao");
        digitalWrite(PROG_RST, HIGH);
        delay(1);
        prog_enable();
        break;
        
      case 'e':
        Serial.println("Vou apagar o chip");
        chip_erase();
        break;

      case 'f':
        Serial.println("Vou finalizar a gravacao");
        digitalWrite(PROG_RST, LOW);
        delay(1);
        break;

      case 'a':
        Serial.println("Recebendo byte baixo");
        while(!Serial.available());
        low_addr = Serial.read();
        break;

      case 'A':
        Serial.println("Recebendo byte alto");
        while(!Serial.available());
        high_addr = Serial.read();
        break;

      case 'w':
        Serial.println("Vou gravar um byte");
        while(!Serial.available());
        write_progmem_byte(high_addr, low_addr, Serial.read());
        break;

      case '1':
        Serial.println("Vou gravar o codigo de um led");
        write_progmem_byte(0x0000, 0xC2);
        write_progmem_byte(0x0001, 0xA0);
        break;

      case '2':
        Serial.println("Vou gravar o codigo de dois leds");
        write_progmem_byte(0x0000, 0xC2);
        write_progmem_byte(0x0001, 0xA0);
        write_progmem_byte(0x0002, 0xC2);
        write_progmem_byte(0x0003, 0xA1);
        break;
        
    }
  }
}

uint8_t send_data(uint8_t data)
{
  // inicia ack com 0
  uint8_t ack = 0;
  int i;
  
  for(i=0; i<8; i++) {

    digitalWrite(PROG_SCK, LOW);

    if(data & (1 << (7 - i)) ) { 
      digitalWrite(PROG_MOSI, HIGH);
    }
    else {
      digitalWrite(PROG_MOSI, LOW);
    }

    // Pulso alto no SCK
    digitalWrite(PROG_SCK, HIGH);
    delayMicroseconds(20);

    // Lê o bit de ACK e aciona o bit respectivo
    // no ack. Note que recebemos primeiro o MSB.
    if(digitalRead(PROG_MISO)) {
      ack |= (1 << (7-i));
    }

    // parte baixa do pulso
    digitalWrite(PROG_SCK, LOW);
    delayMicroseconds(20);
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

void write_progmem_byte(uint8_t high_addr, uint8_t low_addr, uint8_t data)
{
  send_data(0x40);
  send_data(high_addr);
  send_data(low_addr);
  send_data(data);
}

void write_progmem_byte(uint16_t addr, uint8_t data)
{
  uint8_t high_addr = ( (addr & 0xF0) >> 8 );
  uint8_t low_addr = ( addr & 0x0F );

  write_progmem_byte(high_addr, low_addr, data);
}




