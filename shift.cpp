#include "shift.h"

void refresh(uint8_t frm[ROWC][COLC], uint8_t latch, uint8_t enable){
	if (!(frm&&*frm))
		return;
	
  digitalWrite(enable, HIGH);
  SPI.beginTransaction(SPISettings(SRSPEED, SRORDER, SRMODE));

	//Last value first
  //SRORDER: LSBFIRST
	for(int i=ROWC;i>0;i--){
		for(int j=COLC;j>0;j--){
		Serial.print(frm[i-1][j-1]);
		Serial.print("		");
    int received = SPI.transfer(frm[i-1][j-1]);
		}
		Serial.println();
	}
  SPI.endTransaction();
  digitalWrite(latch, HIGH);
  NOP;
  digitalWrite(latch, LOW);
  digitalWrite(enable, LOW);
}