/*
Name:		algArduino.ino
Created:	5/24/2017 11:59:12 PM
Author:	Matheus Rossetti & Rian Turibio
*/	
//Includes
#include <EEPROM.h>
//FUZZY
#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>
//MLP
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Definições de Funções e Variaveis Globais
//DEFINIÇÃO FUZZY
#define MAX_VEL 255																				//Velocidade Maxima
#define MIN_VEL 10																				//Velocidade Minima
#define ENTRADAS        2
#define SAIDAS          1
#define NR_AMOSTRAS     4
#define NR_NEURON_O     6
#define EPOCAS          1000000
#define TX_APRENDIZADO  0.7

double cj_treinamento[NR_AMOSTRAS][ENTRADAS + SAIDAS] = {{ 0, 0, 1 },							// FRENTE
														 { 0, 1, 2 },							// ESQUERDA			
														 { 1, 0, 3 },							// DIREITA
														 { 1, 1, 4 }};							// TRAS

//Passo 1 - Instanciando um objeto da biblioteca
Fuzzy* fuzzy = new Fuzzy();

//PINAGEM MOTORES
const int MOTOR_1_A = 11;																	//Polo do motor 1
const int MOTOR_1_B = 10;																	//Polo do motor 1
const int MOTOR_2_A = 9;																	//Polo do motor 2
const int MOTOR_2_B = 6;																	//Polo do motor 2
const int MOTOR_1_Enable = 5;																//Ativação do motor 1
const int MOTOR_2_Enable = 4;																//Ativação do motor 2
//PINAGEM SENSORES			
const int Trigger_1 = 13;																	//Sinal sensor 1
const int Echo_1 = 12;																		//Sinal sensor 1
const int Trigger_2 = 3;																	//Sinal sensor 2
const int Echo_2 = 2;																		//Sinal sensor 2
//PINAGEM SELETOR
const int selectPin_1 = 7;																	//Opção 1 seletor 
const int selectPin_2 = 8;																	//Opção 2 seletor 
//VARIAVEIS GLOBAIS MLP
double	w_e_o[ENTRADAS + 1][NR_NEURON_O];
double	w_o_s[NR_NEURON_O + 1][SAIDAS];
double	saida_o[NR_NEURON_O];
double	saida_s[SAIDAS];
double	delta_saida[SAIDAS];
double	gradiente_oculta[NR_NEURON_O];
double	delta_oculta[NR_NEURON_O];
//VARIAVEIS PINO SELETOR
int selectState_1 = LOW, selectState_2 = LOW;

//Funções Basicas

//Movimentações
void   movTras						()
{
	digitalWrite(MOTOR_1_B, LOW);
	digitalWrite(MOTOR_2_B, LOW);
	delay(50);
	analogWrite(MOTOR_1_B, MAX_VEL);
	analogWrite(MOTOR_2_B, MAX_VEL);
}
void   movFrente					()
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, MAX_VEL);
	analogWrite(MOTOR_2_A, MAX_VEL);
}
void   movDireita					()
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, 0);
	analogWrite(MOTOR_2_A, MAX_VEL);
}
void   movEsquerda					()
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, MAX_VEL);
	analogWrite(MOTOR_2_A, 0);
}
//Suavização de Movimentos Curvos
void   curvaEsquerda				()
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, MIN_VEL);
	analogWrite(MOTOR_2_A, MAX_VEL);
}
void   curvaDireita					()
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, MAX_VEL);
	analogWrite(MOTOR_2_A, MIN_VEL);
}
//Leitura dos Sensores
float  lerSensor_1					()
{
	float vlr_lido;																			//Cria Variavel para o valor lido pelo sensor
	digitalWrite(Trigger_1, HIGH);															
	delayMicroseconds(50);																	
	digitalWrite(Trigger_1, LOW);															
	vlr_lido = pulseIn(Echo_1, HIGH);														//Tempo do Sinal

	return (vlr_lido / 2 / 29.1);															//Em Cm.
}
float  lerSensor_2					()
{
	float vlr_lido;																			//Cria Variavel para o valor lido pelo sensor
	digitalWrite(Trigger_2, HIGH);															
	delayMicroseconds(50);																	
	digitalWrite(Trigger_2, LOW);															
	vlr_lido = pulseIn(Echo_2, HIGH);														//Tempo do Sinal

	return (vlr_lido / 2 / 29.1);															//Em Cm.
}

//Implementação eFLL
void   programa_1					()
{
	int Dist1, Dist2;

	Dist1 = lerSensor_1();
	Dist2 = lerSensor_2();

	//Fuzzyfica e Defuzzyfica Sensor 1  
	fuzzy->setInput(1, Dist1);
	fuzzy->fuzzify ();
	float output1 = fuzzy->defuzzify(1);

	//Fuzzyfica e Defuzzyfica Sensor 2 
	fuzzy->setInput(2, Dist2);
	fuzzy->fuzzify ();
	float output2 = fuzzy->defuzzify(2);


	Serial.print("Saida Defuzzy 1: ");
	Serial.print(output1);
	Serial.print("Saida Defuzzy 2: ");
	Serial.println(output2);
	
	if (output1 == output2)
	{
		movFrente();
	}
	else if(output1 > output2)
	{
		movTras();
		delay(1000);
        movDireita();
	}
	else if(output1 < output2)
	{
        movTras();
        delay(1000);
        movEsquerda();
	}
	else if(output2 > output1)
	{
       movTras();
       delay(1000);
       movEsquerda();
	}
	else if(output2 < output1)
	{
        movTras();
        delay(1000);
        movDireita();
	}

	// wait 100 milli seconds before looping again
	delay(100);
}
//Implementação MLP
void   programa_2					()
{
	//Entradas da Rna são binarias
	double	entradas[ENTRADAS];

	entradas[0] = lerSensor_1();
	entradas[1] = lerSensor_2();

	inicializa_sinapses();
	treinar_RNA();
	calcular_saidas(entradas);


	if ((saida_s[0] == 1) && (saida_s[1] == 0) && (saida_s[2] == 1) && (saida_s[3] == 0))		//Frente
	{
		movFrente();
	}
	else if ((saida_s[0] == 0) && (saida_s[1] == 1) && (saida_s[2] == 0) && (saida_s[3] == 1))	//Tras
	{
		movTras();
	}
	else if ((saida_s[0] == 0) && (saida_s[1] == 0) && (saida_s[2] == 1) && (saida_s[3] == 0))	//Esquerda
	{
		movEsquerda();
	}
	else if ((saida_s[0] == 1) && (saida_s[1] == 0) && (saida_s[2] == 0) && (saida_s[3] == 0))	//Direita
	{
		movDireita();
	}
}
//MLP

void   inicializa_sinapses			()
{
	int i, j;

	for (i = 0; i < ENTRADAS + 1; i++)
	{
		for (j = 0; j < NR_NEURON_O; j++)
		{
			w_e_o[i][j] = gera_nr_aleatorios();
		}
	}
	for (i = 0; i < NR_NEURON_O + 1; i++)
	{
		for (j = 0; j < SAIDAS; j++)
		{
			w_o_s[i][j] = gera_nr_aleatorios();
		}
	}
}
int    gera_nr_aleatorios			()
{
	int numeros[2] = { -1, 1 };

	return (numeros[rand() % 2]);
}
void   mostrar_sinapses				()
{
	int i, j;
	for (i = 0; i < ENTRADAS + 1; i++)
	{
		for (j = 0; j < NR_NEURON_O; j++)
			printf("%lf ", w_e_o[i][j]);
		printf("\n");
	}

	for (i = 0; i < NR_NEURON_O + 1; i++)
	{
		for (j = 0; j < SAIDAS; j++)
			printf("%lf ", w_o_s[i][j]);
		printf("\n");
	}
}
double f_sigmoid					(double net)
{
	return 1 / (1 + exp(-net));
}
void   calcular_saidas				(double entradas[ENTRADAS])
{
	int i, j;

	for (i = 0; i < NR_NEURON_O; i++) {
		saida_o[i] = 0.0;
		saida_o[i] += w_e_o[0][i] * 1;

		for (j = 1; j < ENTRADAS + 1; j++)
		{
			saida_o[i] += w_e_o[j][i] * entradas[j - 1];
		}
		saida_o[i] = f_sigmoid(saida_o[i]);
	}

	for (i = 0; i < SAIDAS; i++) {
		saida_s[i] = 0.0;
		saida_s[i] += w_o_s[0][i] * 1;

		for (j = 1; j < NR_NEURON_O + 1; j++)
		{
			saida_s[i] += w_o_s[j][i] * saida_o[j - 1];
		}

		saida_s[i] = f_sigmoid(saida_s[i]);
	}
}
void   treinar_RNA					()
{
	int i, j;
	double entradas[ENTRADAS];
	double media_erro = 0.0;

	for (i = 1; i <= EPOCAS; i++)
	{
		for (j = 0; j < NR_AMOSTRAS; j++)
		{
			entradas[0] = cj_treinamento[j][0];
			entradas[1] = cj_treinamento[j][1];
			calcular_saidas(entradas);
			calcular_delta_saida(cj_treinamento[j][0] / 100);
			calcular_delta_saida(cj_treinamento[j][1] / 100);
			calcular_gradiente_oculta();
			calcular_delta_oculta();
			ajustar_pesos_sinapticos(entradas);
		}
	}
}
double calcular_erro				(double desejado, double saida)
{
	return desejado - saida;
}
void   calcular_delta_saida			(double desejado)
{
	int i;
	for (i = 0; i < SAIDAS; i++)
	{
		delta_saida[i] = calcular_erro(desejado, saida_s[i]) * (1 - saida_s[i] * saida_s[i]);
	}
}
void   calcular_gradiente_oculta	()
{
	int i, j;

	for (i = 0; i < SAIDAS; i++) {
		for (j = 1; j < NR_NEURON_O + 1; j++)
		{
			gradiente_oculta[j - 1] = delta_saida[i] * w_o_s[j][i];
		}
	}
}
void   calcular_delta_oculta		()
{
	int i;
	for (i = 0; i < NR_NEURON_O; i++)
	{
		delta_oculta[i] = gradiente_oculta[i] * saida_o[i] * (1 - saida_o[i]);
	}
}
void   ajustar_pesos_sinapticos		(double entradas[ENTRADAS])
{
	int i, j;

	for (i = 0; i < SAIDAS; i++)
	{
		w_o_s[0][i] = w_o_s[0][i] + TX_APRENDIZADO * delta_saida[i] * 1;

		for (j = 1; j < NR_NEURON_O + 1; j++)
		{
			w_o_s[j][i] = w_o_s[j][i] + TX_APRENDIZADO * delta_saida[i] * saida_o[j - 1];
		}
	}
	for (i = 0; i < NR_NEURON_O; i++)
	{
		w_e_o[0][i] = w_e_o[0][i] + TX_APRENDIZADO * delta_oculta[0] * 1;

		for (j = 1; j < ENTRADAS + 1; j++)
		{
			w_e_o[j][i] = w_e_o[j][i] + TX_APRENDIZADO * delta_oculta[i] * entradas[j - 1];
		}
	}
}

//A funçãoo de configuraçãoo é executada uma vez quando você pressiona reset ou liga a placa
void setup							()
{
	Serial.begin(9600);
	//CABEÇALHO FUNÇÕES BASICAS
	void  movTras						();
	void  movFrente						();
	void  movEsquerda					();
	void  movDireita					();
	void  curvaDireita					();
	void  curvaEsquerda					();
	float lerSensor_1					();
	float lerSensor_2					();
	void  programa_1					();
	void  programa_2					();
	//CABEÇALHO FUNÇÕES  AUXILIARES MLP
	int		callMLP						(float Sensor_1, float Sensor_2);
	void	inicializa_sinapses			();
	int		gera_nr_aleatorios			();
	void	mostrar_sinapses			();
	double	f_sigmoid					(double net);
	void	calcular_saidas				(double entradas[ENTRADAS]);
	void	treinar_RNA					();
	double	calcular_erro				(double desejado, double saida);
	void	calcular_delta_saida		(double desejado);
	void	calcular_delta_oculta		();
	void	calcular_gradiente_oculta	();
	void	ajustar_pesos_sinapticos	(double entradas[ENTRADAS]);

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
	//Seletores
	pinMode(selectPin_1, INPUT);
	pinMode(selectPin_2, INPUT);
	//Ligar Motores
	digitalWrite(MOTOR_1_Enable, HIGH);
	digitalWrite(MOTOR_2_Enable, HIGH);




	//UTILIZA��O BIBLIOTECA eFLL
										//Passo 2 - Criando o FuzzyInput distancia
	FuzzyInput* distancia = new FuzzyInput(1);												// Como parametro seu ID
																							// Criando os FuzzySet que compoem o FuzzyInput distancia
	FuzzySet*  proximo = new FuzzySet(2, 5, 5, 8);											// Distancia pequena
		distancia->addFuzzySet(proximo);													// Adicionando o FuzzySet proximo em distance
	FuzzySet*   seguro = new FuzzySet(10, 13, 13, 17);										// Distancia segura
		distancia->addFuzzySet(seguro);														// Adicionando o FuzzySet seguro em distance
	FuzzySet* distante = new FuzzySet(15, 21, 21, 24);										// Distancia grande
		distancia->addFuzzySet(distante);													// Adicionando o FuzzySet distante em distance
	fuzzy->addFuzzyInput(distancia);														// Adicionando o FuzzyInput no objeto Fuzzy

										// Passo 3 - Criando o FuzzyOutput velocidade
	FuzzyOutput* sentido = new FuzzyOutput(1);												// Como parametro seu ID
																							// Criando os FuzzySet que compoem o FuzzyOutput sentido
	FuzzySet* sentidoParado   = new FuzzySet(0, 10, 10, 20);								// Velocidade parado
		sentido->addFuzzySet(sentidoParado);												// Adicionando o FuzzySet sentidoParado em sentido
	FuzzySet* sentidoDireita  = new FuzzySet(255, 255, 255, 255);							// Velocidade para direita
		sentido->addFuzzySet(sentidoDireita);												// Adicionando o FuzzySet sentidoDireita em sentido
	FuzzySet* sentidoEsquerda = new FuzzySet(255, 255, 255, 255);							// Velocidade para esquerda
		sentido->addFuzzySet(sentidoEsquerda);												// Adicionando o FuzzySet sentidoEsquerda em sentido
	FuzzySet* sentidoFrente   = new FuzzySet(60, 80, 80, 100);								// Velocidade alta para frente
		sentido->addFuzzySet(sentidoEsquerda);												// Adicionando o FuzzySet sentidoEsquerda em sentido
	FuzzySet* sentidoTras     = new FuzzySet(60, 80, 80, 100);								// Velocidade alta para tras
		sentido->addFuzzySet(sentidoTras);													// Adicionando o FuzzySet sentidoTras em sentido
	fuzzy->addFuzzyOutput(sentido);															// Adicionando o FuzzyOutput no objeto Fuzzy

										//Passo 4 - Montando as regras Fuzzy
																							// FuzzyRule "SE distanciaDireita = pequena & distanciaEsquerda = pequena ENTAO sentido = parado"
	FuzzyRuleAntecedent* ifPertoD_PertoE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifPertoD_PertoE->joinWithAND(proximo, proximo);										// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenPara = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenPara->addOutput(sentidoParado);													// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifPertoD_PertoE, thenPara);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule01);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//---------------------  
																							// FuzzyRule "SE distanciaDireita = pequena & distanciaEsquerda = segura ENTAO sentido = direita"
	FuzzyRuleAntecedent* ifPertoD_MedioE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifPertoD_MedioE->joinWithAND(proximo, seguro);										// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVDir1 = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenVDir1->addOutput(sentidoDireita);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifPertoD_MedioE, thenVDir1);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule02);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//--------------------- 																
																							// FuzzyRule "SE distanciaDireita = pequena & distanciaEsquerda = distante ENTAO sentido = direita"																							
	FuzzyRuleAntecedent* ifPertoD_LongeE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifPertoD_LongeE->joinWithAND(proximo, distante);									// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVDir2 = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenVDir2->addOutput(sentidoDireita);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifPertoD_LongeE, thenVDir2);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule03);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//--------------------- 
																							// FuzzyRule "SE distanciaDireita = segura & distanciaEsquerda = perto ENTAO sentido = esquerda"
	FuzzyRuleAntecedent* ifMedioD_PertoE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifMedioD_PertoE->joinWithAND(seguro, proximo);										// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVEsq1 = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenVEsq1->addOutput(sentidoEsquerda);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule04 = new FuzzyRule(4, ifMedioD_PertoE, thenVEsq1);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule04);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//--------------------- 
																							// FuzzyRule "SE distanciaDireita = segura & distanciaEsquerda = segura ENTAO sentido = frente"
	FuzzyRuleAntecedent* ifMedioD_MedioE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifMedioD_MedioE->joinWithAND(seguro, seguro);										// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenReto = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenReto->addOutput(sentidoFrente);													// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule06 = new FuzzyRule(6, ifMedioD_MedioE, thenReto);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule06);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//--------------------- 
																							// FuzzyRule "SE distanciaDireita = segura & distanciaEsquerda = distante ENTAO sentido = direita"
	FuzzyRuleAntecedent* ifMedioD_LongeE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifMedioD_LongeE->joinWithAND(seguro, distante);										// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVDir3 = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenVDir3->addOutput(sentidoDireita);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule07 = new FuzzyRule(7, ifMedioD_LongeE, thenVDir3);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule07);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//--------------------- 
																							// FuzzyRule "SE distanciaDireita = distante & distanciaEsquerda = perto ENTAO sentido = esquerda"
	FuzzyRuleAntecedent* ifLongeD_PertoE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifLongeD_PertoE->joinWithAND(distante, proximo);									// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVEsq2 = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenVEsq2->addOutput(sentidoEsquerda);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule05 = new FuzzyRule(5, ifLongeD_PertoE, thenVEsq2);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule05);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//---------------------
																							// FuzzyRule "SE distanciaDireita = distante & distanciaEsquerda = segura ENTAO sentido = esquerda"
	FuzzyRuleAntecedent* ifLongeD_MedioE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifLongeD_MedioE->joinWithAND(distante, seguro);										// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVEsq4 = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenVEsq4->addOutput(sentidoEsquerda);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule08 = new FuzzyRule(8, ifLongeD_MedioE, thenVEsq4);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule08);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//---------------------
																							// FuzzyRule "SE distanciaDireita = distante & distanciaEsquerda = distante ENTAO sentido = frente"
	FuzzyRuleAntecedent* ifLongeD_LongeE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifLongeD_LongeE->joinWithAND(distante, distante);									// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVReto2 = new FuzzyRuleConsequent();							// Instancinado um Consequente para a expressao
		thenVReto2->addOutput(sentidoFrente);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule09 = new FuzzyRule(9, ifLongeD_LongeE, thenVReto2);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule09);													// Adicionando o FuzzyRule ao objeto Fuzzy
}

//A função de loop é executada repetidamente até que a alimentação seja desligada ou reinicializada
void loop							()
{
	selectState_1 = digitalRead(selectPin_1);												//Leitura estado pino seletor 1
	selectState_2 = digitalRead(selectPin_2);												//Leitura estado pino seletor 2

	// Menu de seleção do programa de IA a ser rodado
	if((selectState_1 == HIGH) && (selectState_2 == LOW))									//Verificação do estado do pino seletor 1	
	{
		programa_1();																		//Chamada do programa de Logica Fuzzy
	}
	else if ((selectState_1 == LOW) && (selectState_2 == HIGH))								//Verificação do estado do pino seletor 2
	{
		programa_2();																		//Chamada do programa de MLP
	}
	else
	{
		Serial.print("NÃO QUERO ANDAR NÃO");
	}
}