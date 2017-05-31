/*
 Name:		algArduino.ino
 Created:	5/24/2017 11:59:12 PM
 Author:	Matheus Rossetti & Rian Turibio
*/
#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>

#define MAX_VEL 255 //Velocidade Maxima
#define MIN_VEL 10  //Velocidade Minima

//PINAGEM MOTORES
const int MOTOR_1_A      = 11;
const int MOTOR_1_B      = 10;
const int MOTOR_2_A      = 9;
const int MOTOR_2_B      = 6;
const int MOTOR_1_Enable = 5;
const int MOTOR_2_Enable = 4;
//PINAGEM SENSORES
const int Trigger_1      = 13;
const int Echo_1         = 12;
const int Trigger_2		 = 3;
const int Echo_2         = 2;

void  movTras      (int vel);
void  movFrente    (int vel);
void  movEsquerda  (int vel);
void  movDireita   (int vel);
void  curvaDireita (int vel);
void  curvaEsquerda(int vel);

float lerSensor_1  ();
float lerSensor_2  ();
void  programa_1   ();
void  programa_2   ();

// a função de configuração é executada uma vez quando você pressiona reset ou liga a placa
void setup()
{
	//Modos dos Pinos dos Motores
	pinMode(MOTOR_1_A, OUTPUT);
	pinMode(MOTOR_1_B, OUTPUT);
	pinMode(MOTOR_2_A, OUTPUT);
	pinMode(MOTOR_2_B, OUTPUT);
	pinMode(MOTOR_1_Enable, OUTPUT);
	pinMode(MOTOR_2_Enable, OUTPUT);
	//Modos dos Pinos dos Sensores
	pinMode(Trigger_1, OUTPUT);
	pinMode(Echo_1, INPUT);
	pinMode(Trigger_2, OUTPUT);
	pinMode(Echo_2, INPUT);
	//Ligar Motores
	digitalWrite(MOTOR_1_Enable, HIGH);
	digitalWrite(MOTOR_2_Enable, HIGH);
}

// a função de loop é executada repetidamente até que a alimentação seja desligada ou reinicializada
void  loop()
{

}
//Movimentações
void  movTras      (int vel)
{
	digitalWrite(MOTOR_1_B, LOW);
	digitalWrite(MOTOR_2_B, LOW);
	delay(50);
	analogWrite(MOTOR_1_B, vel);
	analogWrite(MOTOR_2_B, vel);
}
void  movFrente    (int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, vel);
	analogWrite(MOTOR_2_A, vel);
}
void  movDireita   (int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, 0);
	analogWrite(MOTOR_2_A, vel);
}
void  movEsquerda  (int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, vel);
	analogWrite(MOTOR_2_A, 0);
}
//Suavização de Movimentos Curvos
void  curvaEsquerda(int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, MIN_VEL);
	analogWrite(MOTOR_2_A, vel);
}
void  curvaDireita (int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, vel);
	analogWrite(MOTOR_2_A, MIN_VEL);
}

float lerSensor_1  ()
{
	float vlr_lido;
	digitalWrite(Trigger_1, HIGH);
	delayMicroseconds(50);
	digitalWrite(Trigger_1, LOW);
	vlr_lido = pulseIn(Echo_2, HIGH);   //Tempo do Sinal

	return (vlr_lido / 2 / 29);       //Em Cm.
}
float lerSensor_2()
{
	float vlr_lido;
	digitalWrite(Trigger_2, HIGH);
	delayMicroseconds(50);
	digitalWrite(Trigger_2, LOW);
	vlr_lido = pulseIn(Echo_2, HIGH);   //Tempo do Sinal

	return (vlr_lido / 2 / 29);       //Em Cm.
}

void  programa_1   ()
{

}
void  programa_2   ()
{

}