#include "shift.h"

void refresh(uint8_t (*frm)[COLC], uint8_t rows, uint8_t cols, uint8_t latch, uint8_t enable){
	if (!(frm&&*frm))
		return;
	
  digitalWrite(enable, HIGH);	//blank display

  SPI.beginTransaction(SPISettings(SRSPEED, SRORDER, SRMODE));
	//Last value first
  //SRORDER: LSBFIRST
	for(int j=cols;j>0;j--){
		for(int i=rows;i>0;i--){
    int received = SPI.transfer(frm[i-1][j-1]);
		}
	}
  SPI.endTransaction();

  digitalWrite(latch, HIGH);	//latch new values
  NOP;
  digitalWrite(latch, LOW);

  digitalWrite(enable, LOW);	//enable display
}