// Prikazy:
// gloz - otevre nebo zavre levou garaz
// gpoz - otevre nebo zavre pravou garaz
// broz - otevre nebo zavre branu
// pepa - otevre pravou garaz a pokud je zapnute potvrzovani, odesle info na +420777132363
// ping - pro zjisteni, zda-li modul zije, modul posle zpet SMS
// potvrz - zapne potvrzovani prikazu a ulozi do EEPROM - pri kazdem prikazu na otevreni
//          nebo zavreni odesle SMS s informaci o vykonanem prikazu
// nepotvrz - vypne potvrzovani prikazu a ulozi do EEPROM
// v pripade ze je zapnute potvrzovani a prijde SMS s neznamym prikazem, modul oznami SMSkou
// neznamy prikaz


#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <SIM900.h>
#include <sms.h>

SMSGSM sms;

boolean started = false;
boolean potvrzeni = false;
char position;  
char smsNumber[20];
char smsText[160];
int smsLength;
int relay1 = 6;
int relay2 = 5;
int relay3 = 4;

void setup() 
{
  Serial.begin(9600);

  Serial.println();
  Serial.println("****************************");
  Serial.println("* Brana © 2015 David Toman *");
  Serial.println("****************************");
  Serial.println();

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  // zjisti v EEPROM jestli je zapnute potvrzovani
  if (EEPROM.read(0) == 1) {
    potvrzeni = true;
    Serial.println("Potvrzovani zapnuto \(nacteno z EEPROM\)");
  }

  if (gsm.begin(9600)) {
    Serial.println("\nstatus=READY");
    started=true;  
  }
  else Serial.println("\nstatus=IDLE");
  
  if (started) {
    // smaze vsechny ulozene SMS
    if(gsm.SendATCmdWaitResp("AT+CMGD=1,1", 1000, 50, "OK", 2)) {
      Serial.println("Mazu vsechny ulozene SMS");
    }
  }
};

void loop() 
{
  if(started){
    // nacte pozici SMS, pokud je nejaka neprectena
    position = sms.IsSMSPresent(SMS_UNREAD);
    if (position) {
      // read new SMS
      sms.GetSMS(position, smsNumber, smsText, 160);

      String smsMessage = smsText;
      smsMessage.trim();
      smsMessage.toLowerCase();
      
      Serial.println(smsMessage);
      
      // Zpracovani prichoziho prikazu
      if (smsMessage == "gloz") {
        re1Zap();
        if (potvrzeni) sms.SendSMS(smsNumber, "Leva garaz Otevrit / Zavrit");
      } else if (smsMessage == "gpoz") {
        re2Zap();
        if (potvrzeni) sms.SendSMS(smsNumber, "Prava garaz Otevrit / Zavrit");
      }
      else if (smsMessage == "pepa") {
        re2Zap();
        if (potvrzeni) {
          sms.SendSMS(smsNumber, "Chuj Pepa, kterej jde zase pozde, vetra v garazi 1");
          sms.SendSMS("+420777132363", "Chuj Pepa, kterej jde zase pozde, vetra v garazi 1");
        }
      }
      else if (smsMessage == "broz") {
        re3Zap();
        if (potvrzeni) sms.SendSMS(smsNumber, "Brana 2 Otevrit / Zavrit");
      }
      else if (smsMessage == "potvrz") {
        potvrzeni = true;
        EEPROM.write(0,1);
        sms.SendSMS(smsNumber, "Potvrzovani prikazu:\n zapnuto");
      }
      else if (smsMessage == "nepotvrz") {
        potvrzeni = false;
        EEPROM.write(0,0);
        sms.SendSMS(smsNumber, "Potvrzovani prikazu:\n Vypnuto");
      }
      else if (smsMessage == "ping") {
        sms.SendSMS(smsNumber, "ICMP Echo Reply - Pong, ziju!");
      }
      else if (smsMessage.indexOf("konfig") >=0 ) {
        konfigurace();
      }
      else {
        Serial.println("Neznamy prikaz");
        Serial.println(smsMessage);
        if (potvrzeni) sms.SendSMS(smsNumber, "Neznamy prikaz");
      }
      sms.DeleteSMS(position);
    }
    delay(1000);
  }
};

// Vykonani prichoziho prikazu
void re1Zap() {
  digitalWrite(relay1, HIGH);
  delay(2000);
  digitalWrite(relay1, LOW);
}

void re2Zap() {
  digitalWrite(relay2, HIGH);
  delay(2000);
  digitalWrite(relay2, LOW);
}

void re3Zap() {
  digitalWrite(relay3, HIGH);
  delay(2000);
  digitalWrite(relay3, LOW);
}

void konfigurace() {
}
