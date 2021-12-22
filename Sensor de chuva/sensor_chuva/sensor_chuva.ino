// Programa : Teste sensor de chuva
// Autor : Arduino e Cia

#include <ESP8266WiFi.h>

// Dados de Acesso à rede wifi
char* ssid = "TIROLIPA"; //coloque aqui o nome da rede principal que se deseja conectar
char* pass = "27077618"; //coloque aqui a senha da rede principal que se deseja conectar
int count = 0;

// Dados do disposivo
String ID_DISP = "1";

// Url
String URL = "GET https://marcoafonso.com.br/site/microclima/insvalorsensor.php?id=" + ID_DISP + "&valor=";

// Pinos utilizados
int pino_d = 4; //Pino ligado ao D0 do sensor
int pino_a = A0; //Pino ligado ao A0 do sensor

// Portas ligadas aos leds
int pino_led_verde = 12;
int pino_led_amarelo = 13;
int pino_led_vermelho = 14;

/* Definições - deep sleep */
#define FATOR_US_PARA_S   1000000  /* Fator de conversão de micro-segundos para segundos */
#define TEMPO_DEEP_SLEEP  30       /* Tempo para o ESP32 ficar em deep sleep (segundos) */

void setup()
{
  // Define os pinos do sensor como entrada
  pinMode(pino_d, INPUT);
  pinMode(pino_a, INPUT);

  // Define os pinos dos leds como saida
  pinMode(pino_led_verde, OUTPUT);
  pinMode(pino_led_amarelo, OUTPUT);
  pinMode(pino_led_vermelho, OUTPUT);
  Serial.begin(9600);

  int valor = lerSensor();
  
  ligarLeds(valor);
  enviarDados(valor);

  ESP.deepSleep(TEMPO_DEEP_SLEEP * FATOR_US_PARA_S);
}

int lerSensor() 
{
  int val_d = 0; //Armazena o valor lido do pino digital
  int val_a = 0; //Armazena o valor lido do pino analogico
  
  //Le e arnazena o valor do pino digital
  val_d = digitalRead(pino_d);
  //Le e armazena o valor do pino analogico
  val_a = analogRead(pino_a);
  //Envia as informacoes para o serial monitor
  Serial.print("Valor digital : ");
  Serial.print(val_d);
  Serial.print(" - Valor analogico : ");
  Serial.println(val_a);

  return val_a;
}

void ligarLeds(int valor) {
    // Acende o led de acordo com a intensidade 
  if (valor >=900)
  {
    // Acende led verde - intensidade baixa
    digitalWrite(pino_led_verde, HIGH);
    digitalWrite(pino_led_amarelo, LOW);
    digitalWrite(pino_led_vermelho, LOW);
  }
  if (valor >=400 && valor <900)
  {
    // Acende led amarelo - intensidade moderada
    digitalWrite(pino_led_verde, LOW);
    digitalWrite(pino_led_amarelo, HIGH);
    digitalWrite(pino_led_vermelho, LOW);
  }
  if (valor >= 0 && valor <400)
  {
    // Acende led vermelho - intensidade alta
    digitalWrite(pino_led_verde, LOW);
    digitalWrite(pino_led_amarelo, LOW);
    digitalWrite(pino_led_vermelho, HIGH);
  }

}

void enviarDados(int valor) {
  WiFiClientSecure cliente; //Cria um cliente seguro (para ter acesso ao HTTPS)
  cliente.setInsecure();
  
  conectarWifi();
  
  Serial.println(URL + valor);

  if (cliente.connect("marcoafonso.com.br", 443) == 1) {
    cliente.println(URL + valor);
    cliente.println("Host: marcoafonso.com.br"); 
    cliente.println("");
    cliente.stop();
    Serial.println("Dados enviados.");
  }
  else {
    Serial.println("Erro ao conectar no servidor.");
  }
  
}

void conectarWifi()
{
  //Quando conectado contador volta a zero
  count = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
 
  //Enquanto Wi-Fi não estiver conectado e o limite do tempo de conexão não exceder
  Serial.print("Conectando");
  while (WiFi.status()!= WL_CONNECTED && count<= 130)
  {
    delay(100);
    Serial.print(".");
    count = count + 1;
  }
  
  if ((WiFi.status() == WL_CONNECTED))
  {
    Serial.println("");
    Serial.print("Conectado em: ");
    Serial.println(ssid);
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
  }
}

void loop()
{
  
}
