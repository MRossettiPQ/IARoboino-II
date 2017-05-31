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

//Passo 1 - Instanciando um objeto da biblioteca
Fuzzy* fuzzy = new Fuzzy();

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

//A função de configuração é executada uma vez quando você pressiona reset ou liga a placa
void setup()
{
	Serial.begin(9600);
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
	



	//UTILIZAÇÃO BIBLIOTECA eFLL
	//Passo 2 - Criando o FuzzyInput distancia
	FuzzyInput* distance = new FuzzyInput(1);										// Como parametro seu ID

																					//Criando os FuzzySet que compoem o FuzzyInput distancia
	FuzzySet* small = new FuzzySet(0, 20, 20, 40);									// Distancia pequena
	distance->addFuzzySet(small);													// Adicionando o FuzzySet small em distance
	FuzzySet* safe = new FuzzySet(30, 50, 50, 70);									// Distancia segura
	distance->addFuzzySet(safe);													// Adicionando o FuzzySet safe em distance
	FuzzySet* big = new FuzzySet(60, 80, 80, 80);									// Distancia grande
	distance->addFuzzySet(big);														//Adicionando o FuzzySet big em distance

	fuzzy->addFuzzyInput(distance);													// Adicionando o FuzzyInput no objeto Fuzzy

	// Passo 3 - Criando o FuzzyOutput velocidade
	FuzzyOutput* velocity = new FuzzyOutput(1);										// Como parametro seu ID

																					// Criando os FuzzySet que compoem o FuzzyOutput velocidade
	FuzzySet* slow = new FuzzySet(0, 10, 10, 20);									// Velocidade lenta
	velocity->addFuzzySet(slow);													// Adicionando o FuzzySet slow em velocity
	FuzzySet* average = new FuzzySet(10, 20, 30, 40);								// Velocidade normal
	velocity->addFuzzySet(average);													// Adicionando o FuzzySet average em velocity
	FuzzySet* fast = new FuzzySet(30, 40, 40, 50);									// Velocidade alta
	velocity->addFuzzySet(fast);													// Adicionando o FuzzySet fast em velocity

	fuzzy->addFuzzyOutput(velocity);												// Adicionando o FuzzyOutput no objeto Fuzzy

	//Passo 4 - Montando as regras Fuzzy
																					// FuzzyRule "SE distancia = pequena ENTAO velocidade = lenta"
	FuzzyRuleAntecedent* ifDistanceSmall = new FuzzyRuleAntecedent();				// Instanciando um Antecedente para a expresso
	ifDistanceSmall->joinSingle(small);												// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVelocitySlow = new FuzzyRuleConsequent();				// Instancinado um Consequente para a expressao
	thenVelocitySlow->addOutput(slow);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																					// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifDistanceSmall, thenVelocitySlow);	// Passando o Antecedente e o Consequente da expressao

	fuzzy->addFuzzyRule(fuzzyRule01);												// Adicionando o FuzzyRule ao objeto Fuzzy

																					// FuzzyRule "SE distancia = segura ENTAO velocidade = normal"
	FuzzyRuleAntecedent* ifDistanceSafe = new FuzzyRuleAntecedent();				// Instanciando um Antecedente para a expresso
	ifDistanceSafe->joinSingle(safe);												// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVelocityAverage = new FuzzyRuleConsequent();			// Instancinado um Consequente para a expressao
	thenVelocityAverage->addOutput(average);										// Adicionando o FuzzySet correspondente ao objeto Consequente
																					// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifDistanceSafe, thenVelocityAverage); // Passando o Antecedente e o Consequente da expressao

	fuzzy->addFuzzyRule(fuzzyRule02);												// Adicionando o FuzzyRule ao objeto Fuzzy

																					// FuzzyRule "SE distancia = grande ENTAO velocidade = alta"
	FuzzyRuleAntecedent* ifDistanceBig = new FuzzyRuleAntecedent();					// Instanciando um Antecedente para a expresso
	ifDistanceBig->joinSingle(big);													// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVelocityFast = new FuzzyRuleConsequent();				// Instancinado um Consequente para a expressao
	thenVelocityFast->addOutput(fast);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																					// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifDistanceBig, thenVelocityFast);		// Passando o Antecedente e o Consequente da expressao

	fuzzy->addFuzzyRule(fuzzyRule03);												// Adicionando o FuzzyRule ao objeto Fuzzy


}

//A função de loop é executada repetidamente até que a alimentação seja desligada ou reinicializada
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
//Leitura dos Sensores
float lerSensor_1  ()
{
	float vlr_lido;
	digitalWrite(Trigger_1, HIGH);
	delayMicroseconds(50);
	digitalWrite(Trigger_1, LOW);
	vlr_lido = pulseIn(Echo_2, HIGH);   //Tempo do Sinal

	return (vlr_lido / 2 / 29);       //Em Cm.
}
float lerSensor_2  ()
{
	float vlr_lido;
	digitalWrite(Trigger_2, HIGH);
	delayMicroseconds(50);
	digitalWrite(Trigger_2, LOW);
	vlr_lido = pulseIn(Echo_2, HIGH);   //Tempo do Sinal

	return (vlr_lido / 2 / 29);       //Em Cm.
}
//Implementação eFLL
void  programa_1   ()
{
	float dist = getDistanceFromSonar();

	// Passo 5 - Informar o valor das entradas, passando seu ID e valor
	fuzzy->setInput(1, dist);
	// Passo 6 - Executar a fuzzyficação
	fuzzy->fuzzify();
	// Passo 7 - Executar a desfuzzyficação para cada saída, passando seu ID
	float output = fuzzy->defuzzify(1);

	setRobotSpeed(output);

	delay(100);
}
//Implementação MLP
void  programa_2   ()
{

}