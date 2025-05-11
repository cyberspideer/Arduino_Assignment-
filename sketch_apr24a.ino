#include <WiFiS3.h>
// #include <WiFiS3Udp.h>

char ssid[] = "Zain Iphone";
char pass[] = "123456789";

int status = WL_IDLE_STATUS;
WiFiUDP Udp;

const unsigned int localPort = 12345;
char packetBuffer[255];

const int redLED   = 2;
const int greenLED = 3;
const int buzzer   = 4;
const int LDR_THRESHOLD = 600;

void setup() {
  Serial.begin(9600);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  while (status != WL_CONNECTED) {
    Serial.print("WiFi connecting…");
    status = WiFi.begin(ssid, pass);
    delay(2000);
  }
  Serial.println("WiFi connected");
  Serial.print("RX IP = "); Serial.println(WiFi.localIP());

  Udp.begin(localPort);
  Serial.print("Listening on UDP port "); Serial.println(localPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (!packetSize) return;

  int len = Udp.read(packetBuffer, 254);
  packetBuffer[len] = '\0';
  Serial.print("RAW RX → "); Serial.println(packetBuffer);

  float distance; int ldr, btn;
  if (sscanf(packetBuffer, "DIST:%f,LDR:%d,BTN:%d", &distance, &ldr, &btn) != 3) {
    Serial.println("Parse error");
    return;
  }

  // distanse 
  if (distance < 10.0) {
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
  }

  // light and dark LED
  if (ldr < LDR_THRESHOLD) {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
  } else {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
  }

  // button press
  if (btn == 1) {
    tone(buzzer, 1000, 200);
  }
}