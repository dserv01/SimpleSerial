#include "SimpleSerial.h"

bool SimpleSerial::nextToken(){
	tokenEnded=false;
	return Serial.available();

}

char SimpleSerial::read(){
	if(tokenEnded || !Serial.available()){Serial.println("END"); return '\0';}
	char ret = Serial.read();
	if(ret==':'){Serial.println("END"); tokenEnded=true; return '\0';}
	if(ret=='!') ret = Serial.read();
	Serial.println(ret);
	return ret;
}

bool SimpleSerial::end_of_token{
	if(tokenEnded || !Serial.available()) return true;
	if(Serial.peek()==':'){
		Serial.read(); 
		tokenEnded=true; 
		return true;
	}
	return false;
}


int getKeyword(char *keywords[], uint8_t keyword_count){
	Serial.println("BEGINSWITH");
	nextToken();
	uint8_t i;
	//Copy the array of pointers(quite small), so we can mainpulate them without affecting anything else
	char *keywords_pointer[keyword_count];
	for(i=0; i<keyword_count; ++i) keywords_pointer[i]=keywords[i];

	char x = read();
	while(x){
		for(i=0; i<keyword_count; ++i){
			if(keywords_pointer[i]==0) continue; //allready removed
			if(keywords_pointer[i][0]!=x){ //doesn't match
				keywords_pointer[i]=0; //Keyword loeschen
			} else { //still matches
				++keywords_pointer[i]; //next character
			}
		}
		x=read();
	}
	//x=='\0' and so should the matching keyword
	for(uint8_t i=0; i<keyword_count; ++i) if(keywords_pointer[i]!=0 && keywords_pointer[i][0]=='\0') return i;
	return -1;

}

bool getUInt(uint16_t* ret){
	(*ret)=0;
	if(!nextToken()) return false;
	char x = read();
	if(!x) return false; //Token empty
	while(x){
		if(x<'0' || x>'9') return false;
		(*ret)*=10; 
		(*ret)+=x-'0'; //char-digit to int
		x=read();
	}
	return true;
}

bool getInt(int *ret){
	char sign = Serial.peek();
	if(sign=='+' || sign=='-') read();//Remove sign
	uint16_t retu; 
	if(!getUInt(&retu)) return false;
	(*ret)=retu;
	if(sign=='-'){ (*ret)+=-1;}
	return true;
}

uint8_t getString(char* buffer, uint8_t keyword_count){
	nextToken();
	uint8_t i=0;
	char x;
	do {
		x=read();
		if(i==keyword_count) {
			buffer[i]=x;
			++i;
		}
	} while(x);
	return i;
}



