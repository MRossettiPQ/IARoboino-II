/*
 Name:		algArduino.ino
 Created:	5/24/2017 11:59:12 PM
 Author:	Matheus
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

const int MOTOR_1_A      = 11;
const int MOTOR_1_B      = 10;
const int MOTOR_2_A      = 6;
const int MOTOR_2_B      = 5;
const int MOTOR_1_Enable = 4;
const int MOTOR_2_Enable = 3;
const int Trigger        = 13;
const int Echo           = 12;

void  movTras      (int vel);
void  movFrente    (int vel);
void  movEsquerda  (int vel);
void  movDireita   (int vel);
void  curvaDireita (int vel);
void  curvaEsquerda(int vel);

float lerSensor    ();
void  programa_1   ();
void  programa_2   ();

// a fun��o de configura��o � executada uma vez quando voc� pressiona reset ou liga a placa
void setup()
{
	//Pinos dos Motores
	pinMode(MOTOR_1_A, OUTPUT);
	pinMode(MOTOR_1_B, OUTPUT);
	pinMode(MOTOR_2_A, OUTPUT);
	pinMode(MOTOR_2_B, OUTPUT);
	pinMode(MOTOR_1_Enable, OUTPUT);
	pinMode(MOTOR_2_Enable, OUTPUT);
	pinMode(Trigger, OUTPUT);
	pinMode(Echo, INPUT);
	//Ligar Motores
	digitalWrite(MOTOR_1_Enable, HIGH);
	digitalWrite(MOTOR_2_Enable, HIGH);
}

// a fun��o de loop � executada repetidamente at� que a alimenta��o seja desligada ou reinicializada
void  loop()
{

}
//Movimenta��es
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
//Suaviza��o de Movimentos Curvos
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

float lerSensor    ()
{
	float vlr_lido;
	digitalWrite(Trigger, HIGH);
	delayMicroseconds(50);
	digitalWrite(Trigger, LOW);
	vlr_lido = pulseIn(Echo, HIGH);   //Tempo do Sinal

	return (vlr_lido / 2 / 29);       //Em Cm.
}

void  programa_1   ()
{

}
void  programa_2   ()
{

}