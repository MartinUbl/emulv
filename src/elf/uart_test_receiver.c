#define BIT(n) (1U << (n))

#define BASE_ADDRESS 0x40013800

#define STAT *(int *)(BASE_ADDRESS + 0x0) //Status register
#define DATA *(int *)(BASE_ADDRESS + 0x4) //Data register
#define BAUD *(int *)(BASE_ADDRESS + 0x08) //Baud rate register
#define CTL0 *(int *)(BASE_ADDRESS + 0x0C) //Control register 0
#define CTL1 *(int *)(BASE_ADDRESS + 0x10) //Control register 1
#define CTL2 *(int *)(BASE_ADDRESS + 0x14) //Control register 2
#define GP *(int *)(BASE_ADDRESS + 0x18) //Guard time and prescaler register

int main() {
	//---------------------------------------
	//READING DATA FROM UART (RECEIVER MODE)
	//---------------------------------------
	
	//Set WL bit to 0 (8 bit frames), REN bit to 1 (transmitter enabled), UEN bit to 1 (Enable UART)
    CTL0 = 0b00000000000000000010000000000100;
	//Set STB bits to 0 (stop bits format)
	CTL1 = 0b00000000000000000000000000000000;

	//Will receive 1 character
	int msg_num = 1;
	int content = 0;
	
	//Receiver loop
	while(1) {
		int stat_content = STAT;
		
		//Check if RBNE (Receiver buffer not empty) bit is set to 1
		if(stat_content & BIT(5)) {
			//Buffer is not empty, we can read data
			
			content = DATA;
			msg_num--;
			
			if(msg_num == 0)
				break;
		}		
	}

	return content;	
}