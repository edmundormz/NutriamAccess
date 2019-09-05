#include <SPI.h>                          //Required to communicate with NFC reader
#include <MFRC522.h>                      //To manage NFC reader

#define SS_PIN 4                            //D2 (NFC reader required)
#define RST_PIN 5                           //D1 (NFC reader required)
#define relay1 15                           //D8
#define relay2 2                            //D4
#define led_red 0                           //D3
#define led_green 16                         //D0 BUILTINLED
#define relay1_timing 500                   //Time to leave the relays closed
#define relay2_timing 500

MFRC522 mfrc522(SS_PIN, RST_PIN);           //MFRC522 object

//MRFC522 Variables
String content;
String client_id;
byte letter;
String cards[5] = {"698f352d","1937332d","e973352d","397a3f2d","59c9392d"};
bool access = false;

void setup(void)
{ 
  //System initialization
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(led_red, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(led_green, OUTPUT);

  //Turn off outputs
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(led_green, HIGH);
  digitalWrite(led_red, HIGH);
}

void processRFID(){
  for(byte i = 0; i < mfrc522.uid.size; i++){
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  client_id = content;
  content = "";
  digitalWrite(led_green, LOW);
  delay(250);
  digitalWrite(led_green, HIGH);
  Serial.println(client_id);
  for(int i=0; i<5; i++){
    Serial.println(cards[i]);
    if(cards[i]==client_id){
      access = true;
      break;
      }
    else{
      access = false;
      }
    }
  access_control(access);
  return;
}

void access_control(bool access){
  if (access == true){
        digitalWrite(relay2, HIGH);
        digitalWrite(led_green, LOW);
        delay(relay1_timing);
        digitalWrite(relay2, LOW);
        digitalWrite(led_green, HIGH);
      }
      else{
        digitalWrite(relay1, HIGH);
        digitalWrite(led_red, LOW);
        delay(relay2_timing);
        digitalWrite(relay1, LOW);
        digitalWrite(led_red, HIGH);
      }
  return;
}

void loop(){
  //RFID Card detection
  if(!mfrc522.PICC_IsNewCardPresent()){    //Look for cards
    return;
  }
  if(!mfrc522.PICC_ReadCardSerial()){       //Select one of the cards
    return;
  }
  
  processRFID();
  }
