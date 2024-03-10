#include <LoRa_E22.h>// e22 kutuphanesi
#include <Wire.h>// i2c kutuphanesi
#include <LPS25HBSensor.h> // barometre sensörü kutuphanesi
#include <LPS25HB_Driver.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#define DESTINATION_ADDL 16 // lora da fixed transmission yaptigimiz icin mesaji 
                            //gonderecegimiz alicinin adresi
float pressure;             // basinc verisini float olarak almak icin
float temperature;          // sicaklik verisini float olarak aldigimiz icin
String pressurestring;      // basinc verisini gondermek icin strigng yapacağiz
String temperaturestring;   // bu degiskenler float i stringe cevirmemize yarayacak
String serialgonderme;      // bu degisken de gonderecegimiz yazilar ve verilerin birlesimi



  
HardwareSerial Serial4(PB7, PB6); // stm32eki USART1 PİNLERİ
                    // RX   TX
LoRa_E22 e22ttl(&Serial4, PD7, PB4, PB7); 
             //  RXTX     AUX  M0   M1     eger konfigürasyon yapmak istersek kullanacagimiz pinler
void setup() {
  // put your setup code here, to run once:
  e22ttl.begin(); // lora ile haberlesmeyi baslatıyoruz
  pinMode(PC1, OUTPUT);  // baromere modülünün chip select(cs) pini
  e22ttl.sendFixedMessage(0, DESTINATION_ADDL, 23, "MADE BY NEMEGE16\n"); //ilk mesajı gonderdik
  delay(1000);
}

void loop() {
  TwoWire dev_i2c(PC9, PA8);                        // barometrenin i2c pinleri
  digitalWrite(PC1,HIGH);                           // veri alacagimiz icin chip selecti high yapiyoruz
  dev_i2c.begin();                                  // i2c haberlesmesini baslatiyoruz
  LPS25HBSensor PressTemp(&dev_i2c);                // kutuphaneye "bak kullancagin i2c arayuzu bu" diyoruz
  PressTemp.begin();                                // barometre haberleşmesi başladı
  PressTemp.Enable();                               // barometreyi açtık
  PressTemp.GetPressure(&pressure);                 // basınç verisini sensörden alıyoruz
  PressTemp.GetTemperature(&temperature);           // sıcaklık verisini sensörden alıyoruz
  digitalWrite(PC1,LOW);                            // sensörle işimiz bitti şimdi chip selecti kapatıyoruz
  pressurestring = String(pressure);                // float olan verileri birbirine eklemek için string yapıyoruz
  temperaturestring = String(temperature);          // float olan verileri birbirine eklemek için string yapıyoruz
  serialgonderme = "sicaklik: " + temperaturestring + " C basinc:" + pressurestring + " hPa\n";
                                                    // bütün degiskenleri birbirine ekliyoruz
  e22ttl.sendFixedMessage(0, DESTINATION_ADDL, 23, serialgonderme);
                                                   // sonunda lora modülümüze gonderiyoruz
  delay(75);                                       // denemelerim sonucu çıkabildiğim saniyede en çok veri gönderme hızı
}
