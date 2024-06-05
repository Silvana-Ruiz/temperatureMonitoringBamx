#define TINY_GSM_MODEM_SIM900
#include <TinyGsmClient.h>


// Credenciales para GPRS
char apn[] = "internet.itelcel.com";
// user y pass se dejan vacías
char user[] = ""; 
char pass[] = "";


// Define los pines para SoftwareSerial
#include <SoftwareSerial.h>
const int RX = 5;  // Conectar al TX del SIM808
const int TX = 4;  // Conectar al RX del SIM808

SoftwareSerial SerialAT(RX, TX);

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);


BlynkTimer timer;

#include <DHT.h>
#define DHT22PIN 2  //pin D4
#define DHT22TYPE DHT22

DHT dht22(DHT22PIN, DHT22TYPE);

float h22;
float t22;

String temp = "Temperatura del camion 4 esta fuera de rango - La temperatura es: ";
String temp2 = "Reporte de temperatura actual del camion 4 - La Temperatura es: ";

void setup() {
  Serial.begin(9600);
  delay(100);
  SerialAT.begin(115200);
  delay(5000);
  dht22.begin();
  delay(3000);
  
  Serial.println("Reiniciando módem...");
  modem.restart();

  Serial.println("Conectando a GPRS...");
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println("Fallo en la conexión GPRS");
    while (true);
  }
  Serial.println("Conexión GPRS establecida");

  Serial.println(" ");
  Serial.println("Dirección IP obtenida: ");
  sendATCommand("AT+CIFSR");

  timer.setInterval(300000L, dht_sensors);
  timer.setInterval(3600000L, dht_sensors2);
}

void loop() {
  timer.run();
}

//----------------------------------------------------------------------------------------------//

// Función que captura la temperatura, si se encuentra en el rango válido se envía el SMS usando
// el método sendSMS de la librería TinyGSM.
void dht_sensors() {
    h22 = dht22.readHumidity();
    t22 = dht22.readTemperature();
    Serial.println(" ");
    Serial.print("Humedad actual: ");
    Serial.print(h22);
    Serial.println(" ");
    Serial.println(" ");
    Serial.print("Temperatura actual: ");
    Serial.print(t22);
    Serial.println(" ");

    if (t22 > 7 || t22 < 3){
      temp.concat(t22);
      Serial.println(" ");
      Serial.println(" ");
      Serial.println(modem.sendSMS("+526623449296",temp)?"El mensaje envíado a Carlos Iribe fue enviado con éxito":"El Mensaje enviado a Carlos Iribe no fue entregado");
      temp = "Temperatura del camion 4 esta fuera de rango - La temperatura es: ";
      Serial.println(" ");
      Serial.println(" ");
    }

    if (isnan(h22) || isnan(t22)) {
      Serial.println("No fue posible leer los datos del sensor DHT22");
    }
}


void sendATCommand(String command) {
  SerialAT.println(command); // Enviar el comando al SIM808
  delay(1000); // Esperar una respuesta
  while (SerialAT.available()) {
    char c = SerialAT.read(); // Leer la respuesta del SIM808
    Serial.write(c); // Mostrar la respuesta en el monitor serial
  }
  Serial.println();
}



void dht_sensors2() {
    h22 = dht22.readHumidity();
    t22 = dht22.readTemperature();
    Serial.println("Humedad: ");
    Serial.print(h22);
    Serial.println(" ");
    Serial.println(" ");
    Serial.println("Temperatura: ");
    Serial.print(t22);
    Serial.println(" ");

    temp2.concat(t22);
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(modem.sendSMS("+526623449296",temp)?"El mensaje envíado a Carlos Iribe fue enviado con éxito":"El Mensaje enviado a Carlos Iribe no fue entregado");
    Serial.println(" ");
    Serial.println(" ");
    
    temp2 = "Reporte de temperatura actual del camion 4 - La Temperatura es: ";
  
    if (isnan(h22) || isnan(t22)) {
      Serial.println("No fue posible leer los datos del sensor DHT22");
    }
}
