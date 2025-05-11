#include "shift.h"

void refresh(uint8_t frm[ROWC][COLC], uint8_t latch_pin_n, uint8_t oe_pin_n){
	if (!(frm&&*frm))
		return;
	
  //digitalWrite(oe_pin_n, HIGH);
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
  digitalWrite(latch_pin_n, LOW);
  NOP;
  digitalWrite(latch_pin_n, HIGH);
  //digitalWrite(oe_pin_n, LOW);
}