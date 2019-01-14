/* 
* DataLinkProtocol.h
*
* Created: 12.1.19 18:16:43
* Author: filmu
*/


#ifndef __DATALINKPROTOCOL_H__
#define __DATALINKPROTOCOL_H__

#include <avr/io.h>

class DataLink
{
public:
	virtual void Transmit(const char data_tx[]) = 0;
	virtual void Transmit(const char data_tx[], int length) = 0;
	
	virtual uint8_t GetReceiveBufferLength() = 0;
	virtual void GetReceiveData(uint8_t* out_data) = 0;
	
	virtual void GetReceiveData(uint8_t* out_data, uint8_t length) = 0;
	
	virtual CircularBuffer* GetReceiveBuffer();
	
}; //DataLinkProtocol

#endif //__DATALINKPROTOCOL_H__
