// PHAT
#include <Arduino.h>
#include <SPI.h> // include libraries
#include <LoRa.h>

int OP = 20;
int counterout = 0;
const int csPin = 10;   // LoRa radio chip select
const int resetPin = 5; // LoRa radio reset
const int irqPin = 2;   // change for your board; must be a hardware interrupt pin

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}

void LoRa_txMode()
{
  LoRa.idle();            // set standby mode
  LoRa.disableInvertIQ(); // normal mode
}

void LoRa_sendMessage(String message)
{
  LoRa_txMode();        // set tx mode
  LoRa.beginPacket();   // start packet
  LoRa.print(message);  // add payload
  LoRa.endPacket(true); // finish packet and send it
}

void onTxDone()
{
  Serial.println("TxDone");
  //  LoRa_rxMode();
}

void cauhinh()
{
  while (Serial.available())

  {
    String data = "";
    data += (char)Serial.read();

    // Cấu hình 1: Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on.
    // Default medium range; timmer 1byte ( 3s )
    if (data == "1")
    {
      Serial.println("Cấu hình 1 (Default medium range): ");
      Serial.print("BW: 125E3; CR: 4/5; SF: 7; ");
      Serial.print("OP: ");
      Serial.println(OP);

      LoRa.setSignalBandwidth(125E3);
      LoRa.setCodingRate4(5);
      LoRa.setSpreadingFactor(7);
      LoRa.enableCrc();
      counterout = 0;
    }

    // Cấu hình 2: Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on.
    // Fast + short range; timmer 1byte ( 1s )
    if (data == "2")
    {
      Serial.println("Cấu hình 2 (Fast + short range): ");
      Serial.print("BW: 500E3; CR: 4/5; SF: 7; ");
      Serial.print("OP: ");
      Serial.println(OP);

      LoRa.setSignalBandwidth(500E3);
      LoRa.setCodingRate4(5);
      LoRa.setSpreadingFactor(7);
      LoRa.enableCrc();
      counterout = 0;
    }

    // Cấuhình 3: Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on.
    // Slow + long range; timmer 1byte ( 33s )

    if (data == "3")
    {
      Serial.println("Cấu hình 3 (Slow + long range): ");
      Serial.print("BW: 31.25E3; CR: 4/8; SF: 9; ");
      Serial.print("OP: ");
      Serial.println(OP);

      LoRa.setSignalBandwidth(31.25E3);
      LoRa.setCodingRate4(8);
      LoRa.setSpreadingFactor(9);
      LoRa.enableCrc();
      counterout = 0;
    }

    // Cấu hình 4: Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on.
    // Slow + long range; timmer 1byte ( 1p06s )

    if (data == "4")
    {
      Serial.println("Cấu hình 4 (Slow + long range): ");
      Serial.print("BW: 125E3; CR: 4/8; SF: 12; ");
      Serial.print("OP: ");
      Serial.println(OP);

      LoRa.setSignalBandwidth(125E3);
      LoRa.setCodingRate4(8);
      LoRa.setSpreadingFactor(12);
      LoRa.enableCrc();
      counterout = 0;
    }
    // Output Power
    if (data == "o" || data == "O")
    {
      OP++;
      if (OP > 20)
      {
        OP = 14;
      }
      // LoRa.setTxPower(OP);
      LoRa.setTxPower(OP, PA_OUTPUT_PA_BOOST_PIN);
      Serial.print("OP: ");
      Serial.println(OP);
    }
  }
}

void setup()
{
  Serial.begin(9600); // initialize serial
  while (!Serial)
    ;

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(433E6))
  {
    Serial.println("LoRa init failed. Check your connections.");
    while (true)
      ; // if failed, do nothing
  }
  Serial.println("LoRa Sender init succeeded.");
}
void loop()
{
  if (counterout < 100) // Packet Delivery Ratio
  {
    if (runEvery(3000))
    { // repeat every 30000 millis
      String message = "A";
      LoRa_sendMessage(message); // send a message
      Serial.print("Send Message!: ");
      Serial.print(message);
      counterout++;
      Serial.print(" -TONG GOI TIN GUI: ");
      Serial.println(counterout);
    }
  }
  cauhinh();
}
