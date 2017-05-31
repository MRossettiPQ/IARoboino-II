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
const int MOTOR_1_A = 11;
const int MOTOR_1_B = 10;
const int MOTOR_2_A = 9;
const int MOTOR_2_B = 6;
const int MOTOR_1_Enable = 5;
const int MOTOR_2_Enable = 4;
//PINAGEM SENSORES
const int Trigger_1 = 13;
const int Echo_1 = 12;
const int Trigger_2 = 3;
const int Echo_2 = 2;

void  movTras(int vel);
void  movFrente(int vel);
void  movEsquerda(int vel);
void  movDireita(int vel);
void  curvaDireita(int vel);
void  curvaEsquerda(int vel);

float lerSensor_1();
float lerSensor_2();
void  programa_1();
void  programa_2();

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
	FuzzyInput* distancia = new FuzzyInput(1);												// Como parametro seu ID
																							// Criando os FuzzySet que compoem o FuzzyInput distancia
	FuzzySet* proximo = new FuzzySet(0, 20, 20, 40);										// Distancia pequena
	distancia->addFuzzySet(proximo);													// Adicionando o FuzzySet small em distance
	FuzzySet* seguro = new FuzzySet(30, 50, 50, 70);										// Distancia segura
	distancia->addFuzzySet(seguro);														// Adicionando o FuzzySet safe em distance
	FuzzySet* distante = new FuzzySet(60, 80, 80, 80);										// Distancia grande
	distancia->addFuzzySet(distante);													// Adicionando o FuzzySet big em distance
	fuzzy->addFuzzyInput(distancia);														// Adicionando o FuzzyInput no objeto Fuzzy

																							// Passo 3 - Criando o FuzzyOutput velocidade
	FuzzyOutput* velocidade = new FuzzyOutput(1);											// Como parametro seu ID
																							// Criando os FuzzySet que compoem o FuzzyOutput velocidade
	FuzzySet* baixaVelocidade = new FuzzySet(0, 10, 10, 20);								// Velocidade lenta
	velocidade->addFuzzySet(baixaVelocidade);											// Adicionando o FuzzySet slow em velocity
	FuzzySet* mediaVelocidade = new FuzzySet(10, 20, 30, 40);								// Velocidade normal
	velocidade->addFuzzySet(mediaVelocidade);											// Adicionando o FuzzySet average em velocity
	FuzzySet* altaVelocidade = new FuzzySet(30, 40, 40, 50);								// Velocidade alta
	velocidade->addFuzzySet(altaVelocidade);											// Adicionando o FuzzySet fast em velocity
	fuzzy->addFuzzyOutput(velocidade);														// Adicionando o FuzzyOutput no objeto Fuzzy

																							//Passo 4 - Montando as regras Fuzzy
																							// FuzzyRule "SE distancia = pequena ENTAO velocidade = lenta"
	FuzzyRuleAntecedent* ifDistanciaPequena = new FuzzyRuleAntecedent();					// Instanciando um Antecedente para a expresso
	ifDistanciaPequena->joinSingle(proximo);											// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVelocidadeBaixa = new FuzzyRuleConsequent();					// Instancinado um Consequente para a expressao
	thenVelocidadeBaixa->addOutput(baixaVelocidade);									// Adicionando o FuzzySet correspondente ao objeto Consequente
																						// Instanciando um objeto FuzzyRule
	FuzzyRule* regraFuzzy01 = new FuzzyRule(1, ifDistanciaPequena, thenVelocidadeBaixa);	// Passando o Antecedente e o Consequente da expressao
	fuzzy->addFuzzyRule(regraFuzzy01);														// Adicionando o FuzzyRule ao objeto Fuzzy

																							// FuzzyRule "SE distancia = segura ENTAO velocidade = normal"
	FuzzyRuleAntecedent* ifDistanciaSegura = new FuzzyRuleAntecedent();					// Instanciando um Antecedente para a expresso
	ifDistanciaSegura->joinSingle(seguro);												// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVelocidadeMedia = new FuzzyRuleConsequent();					// Instancinado um Consequente para a expressao
	thenVelocidadeMedia->addOutput(mediaVelocidade);									// Adicionando o FuzzySet correspondente ao objeto Consequente
																						// Instanciando um objeto FuzzyRule
	FuzzyRule* regraFuzzy02 = new FuzzyRule(2, ifDistanciaSegura, thenVelocidadeMedia); 	// Passando o Antecedente e o Consequente da expressao
	fuzzy->addFuzzyRule(regraFuzzy02);														// Adicionando o FuzzyRule ao objeto Fuzzy

																							// FuzzyRule "SE distancia = grande ENTAO velocidade = alta"
	FuzzyRuleAntecedent* ifDistanciaLonga = new FuzzyRuleAntecedent();					// Instanciando um Antecedente para a expresso
	ifDistanciaLonga->joinSingle(distante);												// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVelocidadeRapida = new FuzzyRuleConsequent();					// Instancinado um Consequente para a expressao
	thenVelocidadeRapida->addOutput(altaVelocidade);									// Adicionando o FuzzySet correspondente ao objeto Consequente
																						// Instanciando um objeto FuzzyRule
	FuzzyRule* regraFuzzy03 = new FuzzyRule(3, ifDistanciaLonga, thenVelocidadeRapida);		// Passando o Antecedente e o Consequente da expressao
	fuzzy->addFuzzyRule(regraFuzzy03);														// Adicionando o FuzzyRule ao objeto Fuzzy
}

//A função de loop é executada repetidamente até que a alimentação seja desligada ou reinicializada
void  loop()
{

}
//Movimentações
void  movTras(int vel)
{
	digitalWrite(MOTOR_1_B, LOW);
	digitalWrite(MOTOR_2_B, LOW);
	delay(50);
	analogWrite(MOTOR_1_B, vel);
	analogWrite(MOTOR_2_B, vel);
}
void  movFrente(int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, vel);
	analogWrite(MOTOR_2_A, vel);
}
void  movDireita(int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, 0);
	analogWrite(MOTOR_2_A, vel);
}
void  movEsquerda(int vel)
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
void  curvaDireita(int vel)
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, vel);
	analogWrite(MOTOR_2_A, MIN_VEL);
}
//Leitura dos Sensores
float lerSensor_1()
{
	float vlr_lido;
	digitalWrite(Trigger_1, HIGH);
	delayMicroseconds(50);
	digitalWrite(Trigger_1, LOW);
	vlr_lido = pulseIn(Echo_2, HIGH);												//Tempo do Sinal

	return (vlr_lido / 2 / 29);														//Em Cm.
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
//Implementação eFLL
void  programa_1()
{
	float distanciaAtual = getDistanceFromSonar();

	// Passo 5 - Informar o valor das entradas, passando seu ID e valor
	fuzzy->setInput(1, distanciaAtual);
	// Passo 6 - Executar a fuzzyficação
	fuzzy->fuzzify();
	// Passo 7 - Executar a desfuzzyficação para cada saída, passando seu ID
	float saida = fuzzy->defuzzify(1);

	setRobotSpeed(saida);
	delay(100);
}
//Implementação MLP
void  programa_2()
{

}