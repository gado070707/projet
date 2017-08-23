#include <Capteur.h>
#include <mesFonctions.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <String.h>

#define ATTENTE001  10
#define ATTENTE01 100
#define ATTENTE1 1000
#define ATTENTE2 2000
#define PERIODEINITIALISATION 300000
#define PINCAPTEURMOUVEMENT 2
#define PINBUZZER 0
#define TEMPSENTRELESBIPS 10
#define NOMBREDEBIPS 10
#define SECONDE millis()/1000

bool enLoop(true);
//bool compteurSecondes(false);
bool initialisationTermine(false);

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";

Capteur mouvement(PINCAPTEURMOUVEMENT, LOW, PERIODEINITIALISATION);
Capteur buzzer(PINBUZZER, LOW);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  mouvement.input();
  buzzer.output();
  Serial.begin(74880);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  setup_wifi();
  Serial.println("\n --------- Parametrage OK ----------- \n"); 
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println( WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------------

void loop() {
  
  if(enLoop){enLoop = loopOperationnel(&enLoop);}
  initialisationTermine=mouvement.periodeInitialisation();
  if(!initialisationTermine){return;}
  //if(millis() < PERIODEINITIALISATION){compteurSecondes = true;return;} 
  //if(Capteur::ms_declenchement && millis() <= millisFin && compteurSecondes == false){compteurSecondes=true;} 
  //Capteur::ms_declenchement = false;
  
  if(mouvement.getReadEtat() == HIGH){
    if (!client.connected()) {
      delay(ATTENTE001);
      reconnect();
    }
    client.loop();
    unsigned long now = millis();
    if (now - lastMsg > 2000) {
      lastMsg = now;
      ++value;
      snprintf (msg, 75, "===========> ALERTE No %ld <===========", value);
      //Serial.print("Publish message : ");
      Serial.println(msg);
      //client.publish("outTopic", msg);
      client.publish("Hall", msg);
    }
    
  Serial.println("\n\n ===== > Mouvement detecte ! < ===== \n\n");
  Serial.println();
  delay(ATTENTE001);
  buzzer.bipEnContinue(TEMPSENTRELESBIPS, NOMBREDEBIPS);
  Capteur::ms_declenchement = true ;
  //initialisationTermine = mouvement.periodeInitialisation();
  delay(ATTENTE001);
  }

  else{
    Serial.println(" ===== > Mouvement non detecte < ===== ");
    Serial.println();
  }
}
