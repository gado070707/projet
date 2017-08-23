//Crée par gado070707 le 23/08/2017

#include "Capteur.h"
#include <Arduino.h>
#define PIN 0
#define PERIODEINITIALISATION 300000

bool Capteur::ms_declenchement=true;
int Capteur::ms_compteur(0);

Capteur::Capteur() : m_pin(PIN), m_etat(LOW), m_periodeInitialisation(PERIODEINITIALISATION), m_initialisation(false), 
m_initialisationComplete(false), m_initialisationPrintfs(false), m_millisDepart(0), m_millisFin(0){++ms_compteur;}

Capteur::Capteur(int p) : m_pin(p), m_etat(LOW), m_periodeInitialisation(PERIODEINITIALISATION){++ms_compteur;}

Capteur::Capteur(int p, int e) : m_pin(p), m_etat(e), m_periodeInitialisation(PERIODEINITIALISATION){++ms_compteur;}

Capteur::Capteur(int p, int e, int periode) : m_pin(p), m_etat(e), m_periodeInitialisation(periode){++ms_compteur;}

int Capteur::getPin(){return m_pin;}

bool Capteur::getReadEtat(){m_etat = digitalRead(m_pin);return m_etat;}

void Capteur::outHigh(){digitalWrite(m_pin,HIGH);}

void Capteur::outLow(){digitalWrite(m_pin,LOW);}

void Capteur::input(){pinMode(m_pin, INPUT);}

void Capteur::output(){pinMode(m_pin, OUTPUT);}

void Capteur::bipEnContinue(unsigned long tempsEntreLesBips, unsigned long nombreDeBIP){
for(int i(0);i<nombreDeBIP;i++){
      this->outHigh();
      delay(tempsEntreLesBips);
      this->outLow();
      delay(tempsEntreLesBips);
    }
}

bool Capteur::periodeInitialisation(){
	if(getDeclenchement()){
		m_millisDepart = millis();
		m_millisFin = m_millisDepart + PERIODEINITIALISATION;
		ms_declenchement = false;
		if(millis() < m_millisFin){
			if(!m_initialisationPrintfs){
				Serial.println("\n ------ Initialisation en cours ------------ \n");
				m_initialisationPrintfs = true;
			}
		delay(3000);
		Serial.print(".");
		m_initialisation = true;
		return m_initialisation;
		}
	}
	else{
		m_initialisation = false;
		Serial.println("------ Initialisation termine ------ ");
		Serial.println("------ Capteur de mouvement operationnel ------ ");
		Serial.println("------------- OK ----------------- ");
		m_initialisationComplete = true;
	}
	return m_initialisationComplete;
}

int Capteur::getPeriodeInitialisation(){
	return m_periodeInitialisation;
}

bool Capteur::getInitialisation(){
	return m_initialisation;
}

bool Capteur::getInitialisationComplete(){
	return m_initialisationComplete;
}

bool Capteur::getDeclenchement(){
	return ms_declenchement;
}

int Capteur::nombreInstances(){ return ms_compteur; }

Capteur::~Capteur(){--ms_compteur;}
