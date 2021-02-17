#ifndef TFT_ILI9486_CONFIG
#define TFT_ILI9486_CONFIG

#define DDR_RST		DDRA
#define PORT_RST	PORTA
#define B_RST		7

#define DDR_CS		DDRA
#define PORT_CS		PORTA
#define B_CS		6

#define DDR_RS		DDRA
#define PORT_RS		PORTA
#define B_RS		5

#define DDR_WR		DDRA
#define PORT_WR		PORTA
#define B_WR		4

#define DDR_RD		DDRA
#define PORT_RD		PORTA
#define B_RD		3

#define DDR_DATA		DDRC
#define PORT_DATA		PORTC
#define PIN_DATA		PINC

//X_MAX � Y_MAX ���������� ���������� ������: ���������� ��� ���������
//X ��� ������ �� ����������� ����� �� �����
//Y ��� ������ �� ��������� � ����� � ���
#define X_MAX	320			//�� �����������
#define Y_MAX	480			//�� ���������

#endif

