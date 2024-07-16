/*
 *   CiftKanalliDCmotorSurme örneği,
 *   Bu örnekte temel konfigürasyon ayarları yapılmaktadır.
 *   1.motoru %80 dutycyle hızı ile ileri yönde hareket etmektedir. 2.motoru %75 dutycyle hızı ile geri yönde hareket etmektedir.
 *
 *   Bu algılayıcı I2C haberleşme protokolü ile çalışmaktadır.
 *
 *   Bu örnek Deneyap Çift Kanallı Motor Sürücü için oluşturulmuştur
 *      ------>  https://docs.deneyapkart.org/tr/content/contentDetail/deneyap-module-deneyap-dual-channel-motor-driver-m  <------
 *      ------>  https://github.com/deneyapkart/deneyap-cift-kanalli-motor-surucu-arduino-library  <------ 
*/
#include <Deneyap_CiftKanalliMotorSurucu.h>         // Deneyap Çift Kanallı Motor Sürücü kütüphanesi eklenmesi
#include <Deneyap_DerinlikOlcer.h> 

#define buton D0

TofRangeFinder TofRangeFinder;
DualMotorDriver MotorSurucu;                        // DualMotorDriver için class tanımlanması

float mesafe;
int counter = 0;

void setup() {
  Serial.begin(115200); 
  TofRangeFinder.begin(0x29);
  pinMode(buton, INPUT);
  if (!MotorSurucu.begin(0x16)) {                   // begin(slaveAdress) fonksiyonu ile cihazların haberleşmesi başlatılması, varsayılan: begin(0x16,500), frekans:500Hz
      delay(3000);
      Serial.println("I2C bağlantısı başarısız");   // I2C bağlantısı başarısız olursa seri port ekranına yazdırılması
      while(1);
  } 
}

void loop() {                                        
  mesafe = TofRangeFinder.ReadDistance();
  Serial.println("Derinlik: " + String(mesafe));
  Serial.println("Counter: " + String(counter));
  
  if(digitalRead(buton) == 1 && counter == 0){
    while(mesafe > 16){
      MotorSurucu.MotorDrive(MOTOR1, 60, 0);
      mesafe = TofRangeFinder.ReadDistance();
      Serial.println("Derinlik: " + String(mesafe));
    }
    MotorSurucu.MotorDrive(MOTOR1, 0, 0);
    counter = 1;
  }
    
  else if(digitalRead(buton) == 1 && counter == 1){
    while(mesafe < 65){
      MotorSurucu.MotorDrive(MOTOR1, 60, 1);
      mesafe = TofRangeFinder.ReadDistance();
      Serial.println("Derinlik: " + String(mesafe));
    }
    MotorSurucu.MotorDrive(MOTOR1, 0, 1);
    counter = 0;
  }

  if(MotorSurucu.CheckMotorError()==1) Serial.println("HATA !!! Lütfen motorları kontrol edin");
}
