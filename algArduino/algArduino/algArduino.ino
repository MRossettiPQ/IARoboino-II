/*
 Name:		algArduino.ino
 Created:	5/24/2017 11:59:12 PM
 Author:	Matheus
*/

#define MAX_VEL 255 //Velocidade Maxima
#define MIN_VEL 10  //Velocidade Minima

const int MOTOR_1_A = 11;
const int MOTOR_1_B = 10;
const int MOTOR_2_A = 6;
const int MOTOR_2_B = 5;
const int MOTOR_1_Enable = 4;
const int MOTOR_2_Enable = 3;
const int Trigger = 13;
const int Echo = 12;

void ir_para_tras(int vel);
void ir_para_frente(int vel);
void ir_para_esquerda(int vel);
void ir_para_direita(int vel);
void curva_direita(int vel);
void curva_esquerda(int vel);

float ler_sensor();
void programa_1();
void programa_2();

// the setup function runs once when you press reset or power the board
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

// the loop function runs over and over again until power down or reset
void loop()
{

}
void ir_para_tras(int vel)
{
	digitalWrite(MOTOR_1_B, LOW);
	digitalWrite(MOTOR_2_B, LOW);
	delay(50);
	analogWrite(MOTOR_1_B, vel);
	analogWrite(MOTOR_2_B, vel);
}
void ir_para_frente(int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, vel);
	analogWrite(MOTOR_2_A, vel);
}
void ir_para_direita(int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, 0);
	analogWrite(MOTOR_2_A, vel);
}
void ir_para_esquerda(int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, vel);
	analogWrite(MOTOR_2_A, 0);
}
void curva_esquerda(int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, MIN_VEL);
	analogWrite(MOTOR_2_A, vel);
}
void curva_direita(int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, vel);
	analogWrite(MOTOR_2_A, MIN_VEL);
}
float ler_sensor()
{
	float vlr_lido;
	digitalWrite(Trigger, HIGH);
	delayMicroseconds(50);
	digitalWrite(Trigger, LOW);
	vlr_lido = pulseIn(Echo, HIGH);   //Tempo do SInal

	return (vlr_lido / 2 / 29);       //Em Cm.
}
void programa_1()
{
	ir_para_frente(MAX_VEL);
	delay(1000);
	ir_para_tras(MAX_VEL);
	delay(1000);
	ir_para_esquerda(MAX_VEL);
	delay(500);
	ir_para_direita(MAX_VEL);
	delay(500);
}
void programa_2()
{

}