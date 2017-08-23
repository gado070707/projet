#include <Arduino.h>
#include "mesFonctions.h"

bool loopOperationnel(bool *enLoop){
	if(*enLoop){
		Serial.print("\n ------ Fonction Loop OK ------------ \n");
	}
	*enLoop = false;
	return *enLoop;
}
