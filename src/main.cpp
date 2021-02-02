#include <Arduino.h>
#include "Ultrasonic.h"
#include "dlacolor.h"

//Constantes
#define INTERVALO 900000 //(15min) //intervalo de ligação da bomba em millisegundos
#define TEMPOENCHER 4000

//Sensores e atuadores
#define bombaAquario 13
#define sensorCapacitivo 8
#define triggerDistancia 4
#define echoDistancia 5
#define red 9
#define green 10
#define blue 6
#define buzzer 3
#define ledCopo 12

//Funções
void ativaAgua(void);    //Ativa a rotina de ligar o motor
bool copoPresente(void); //verifica se o copo está no lugar
void setaCorDoLED(void); //seleciona a cor do led de acordo com tempo de espera
void tocaBuzzer(void);   //som do buzzer

//Variáveis
int tempoAtual, tempoInicial;
Ultrasonic sensorDistancia(triggerDistancia, echoDistancia);
pinRGB luz(red, green, blue); //Leds para mostrar o que está acontecendo. Serão dois LEDs ligados em paralelo

void setup()
{
  pinMode(bombaAquario, OUTPUT);
  pinMode(sensorCapacitivo, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledCopo, OUTPUT);
  tempoInicial = millis();
  noTone(buzzer);     //pra evitar que o buzzer fique com lixo
  Serial.begin(9600); //debug
}

void loop()
{
  tempoAtual = millis();

  setaCorDoLED();
  if (copoPresente() && (digitalRead(sensorCapacitivo) || (tempoAtual - tempoInicial >= INTERVALO)))
  {
    delay(300);
    ativaAgua();
    tempoInicial = tempoAtual; //atualização do millis
  }
  //delay(300);
}

void ativaAgua(void) //Rotina de ativar a água
{
  digitalWrite(bombaAquario, HIGH);
  luz.setCode(1);
  tocaBuzzer();
  delay(TEMPOENCHER);
  digitalWrite(bombaAquario, LOW);
  luz.setCode(6);
}

bool copoPresente(void) //Verifica se o código está no lugar
{
  int distancia = sensorDistancia.convert(sensorDistancia.timing(), Ultrasonic ::CM); //distância em centímetros
  //Serial.println(distancia);//debug
  if (distancia < 8) //Alterar aqui o valor da distância
  {
    digitalWrite(ledCopo, HIGH);
    return true;
  }
  digitalWrite(ledCopo, LOW);
  return false;
}

void setaCorDoLED(void) //Função para mudar a cor do LED de acordo com o tempo
{
  int tempoVermelho = INTERVALO / 2;
  if (tempoAtual - tempoInicial >= tempoVermelho)
  {
    luz.setCode(4);
  }
  //luz.setClean();
}

void tocaBuzzer(void)
{
  tone(buzzer, 1700);
  delay(700);
  noTone(buzzer);
}
