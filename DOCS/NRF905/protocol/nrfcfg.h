#define WC  				0x00						//Write Configuration Register Command
#define RC  				0x10						//Read  Configuration Register Command
#define WTP 				0x20						//Write TX Payload Command
#define RTP 				0x21						//Read  TX Payload Command
#define WTA 				0x22						//Write TX Address Command
#define RTA 				0x23						//Read  TX Address Command
#define RRP  				0x24						//Read  RX Payload Command
#define WC3                 0x03                        //Write Configuration from byte 3

#define CH_NO_FREQ_422_4MHz  0x000                     //����Ƶ��422.4MHz(433MHzƵ������Ƶ��)
#define CH_NO_FREQ_422_5MHz  0x001                     //����Ƶ��422.5MHz
#define CH_NO_FREQ_425_0MHz  0x01a                     //����Ƶ��425.0MHz
#define CH_NO_FREQ_427_5MHz  0x033                     //����Ƶ��427.5MHz

#define CH_NO_FREQ_430_0MHz  0x04c                     //����Ƶ��430.0MHz
#define CH_NO_FREQ_433_0MHz  0x06a                     //����Ƶ��433.0MHz(433MHzƵ�λ�׼Ƶ��)
#define CH_NO_FREQ_433_1MHz  0x06b                     //����Ƶ��433.1MHz
#define CH_NO_FREQ_433_2MHz  0x06c                     //����Ƶ��433.2MHz
#define CH_NO_FREQ_434_7MHz  0x07b                     //����Ƶ��434.7MHz
#define CH_NO_FREQ_473_5MHz  0x1ff                     //����Ƶ��473.5MHz(433MHzƵ������Ƶ��)

#define CH_NO_FREQ_844_8MHz  0x000                     //����Ƶ��844.8MHz(868MHzƵ������Ƶ��)

#define CH_NO_FREQ_862_0MHz  0x056                     //����Ƶ��862.0MHz
#define CH_NO_FREQ_868_0MHz  0x074                     //����Ƶ��868.0MHz(868MHzƵ�λ�׼Ƶ��)
#define CH_NO_FREQ_868_2MHz  0x075                     //����Ƶ��868.2MHz
#define CH_NO_FREQ_868_4MHz  0x076                     //����Ƶ��868.4MHz
#define CH_NO_FREQ_869_8MHz  0x07d                     //����Ƶ��869.8MHz
#define CH_NO_FREQ_895_8MHz  0x0ff                     //����Ƶ��895.8MHz
#define CH_NO_FREQ_896_0MHz  0x100                     //����Ƶ��896.0MHz
#define CH_NO_FREQ_900_0MHz  0x114                     //����Ƶ��900.0MHz
#define CH_NO_FREQ_902_2MHz  0x11f                     //����Ƶ��902.2MHz
#define CH_NO_FREQ_902_4MHz  0x120                     //����Ƶ��902.4MHz
#define CH_NO_FREQ_915_0MHz  0x15f                     //����Ƶ��915.0MHz(915MHzƵ�λ�׼Ƶ��)
#define CH_NO_FREQ_927_8MHz  0x19f                     //����Ƶ��927.8MHz

#define CH_NO_FREQ_947_0MHz  0x1ff                     //����Ƶ��947.0MHz(915MHzƵ������Ƶ��)


#define CH_NO_FREQ           CH_NO_FREQ_430_0MHz       //Change if other frequency
#define CH_NO_BYTE           CH_NO_FREQ & 0xff         //����Ƶ�ʵ�8λ       Byte0       01101100
#define CH_NO_BIT8           CH_NO_FREQ >> 8           //����Ƶ�ʵ�9λ       Byte1.0     0

#define AUTO_RETRAN          0x20                      //�ط����ݰ�          Byte1.5     0
#define RX_RED_PWR           0x10                      //���յ͹���ģʽ      Byte1.4     0
#define PA_PWR__10dBm        0x00                      //��������-10dBm      Byte1.3~2   00
#define PA_PWR_2dBm          0x04                      //��������+2dBm       Byte1.3~2
#define PA_PWR_6dBm          0x08                      //��������+6dBm       Byte1.3~2
#define PA_PWR_10dBm         0x0c                      //��������+10dBm      Byte1.3~2   
#define HFREQ_PLL_433MHz     0x00                      //������433MHzƵ��    Byte1.1     0
#define HFREQ_PLL_868MHz     0x02                      //������868MHzƵ��    Byte1.1
#define HFREQ_PLL_915MHz     0x02                      //������915MHzƵ��    Byte1.1

#define TX_AFW_1BYTE         0x01*16                      //���͵�ַ����1�ֽ�   Byte2.7~4
#define TX_AFW_2BYTE         0x02*16                      //���͵�ַ����2�ֽ�   Byte2.7~4
#define TX_AFW_3BYTE         0x03*16                      //���͵�ַ����3�ֽ�   Byte2.7~4
#define TX_AFW_4BYTE         0x04*16                      //���͵�ַ����4�ֽ�   Byte2.7~4   100
#define RX_AFW_1BYTE         0x01                      //���յ�ַ����1�ֽ�   Byte2.3~0
#define RX_AFW_2BYTE         0x02                      //���յ�ַ����2�ֽ�   Byte2.3~0
#define RX_AFW_3BYTE         0x03                      //���յ�ַ����3�ֽ�   Byte2.3~0
#define RX_AFW_4BYTE         0x04                      //���յ�ַ����4�ֽ�   Byte2.3~0   100
#define RX_PW_3BYTE          3                         //�������ݿ���1�ֽ�   Byte3.5~0

#define RX_PW_32BYTE         32                        //�������ݿ���32�ֽ�  Byte3.5~0   00100000
#define TX_PW_3BYTE          3                         //�������ݿ���1�ֽ�   Byte4.5~0 

#define TX_PW_32BYTE         32                        //�������ݿ���32�ֽ�  Byte4.5~0   00100000

#define CRC_MODE_16BIT       0x80                      //CRC16ģʽ           Byte9.7     1
#define CRC_MODE_8BIT        0x00                      //CRC8ģʽ            Byte9.7     
#define CRC_EN               0x40                      //CRCʹ��             Byte9.6     1
#define CRC16_EN             0xc0                      //CRC16ģʽʹ��       Byte9.7~6   11
#define CRC8_EN              0x40                      //CRC8ģʽʹ��        Byte9.7~6
#define XOF_20MHz            0x20                      //����������Ƶ��20MHz Byte9.5~3
#define XOF_16MHz            0x18                      //����������Ƶ��16MHz Byte9.5~3   100
#define XOF_12MHz            0x10                      //����������Ƶ��12MHz Byte9.5~3
#define XOF_8MHz             0x08                      //����������Ƶ��8MHz  Byte9.5~3
#define XOF_4MHz             0x00                      //����������Ƶ��4MHz  Byte9.5~3
#define UP_CLK_EN            0x40                      //����ʱ��ʹ��        Byte9.2     1
#define UP_CLK_FREQ_500kHz   0x03                      //����ʱ��Ƶ��500kHz  Byte9.1~0   11
#define UP_CLK_FREQ_1MHz     0x02                      //����ʱ��Ƶ��1MHz    Byte9.1~0
#define UP_CLK_FREQ_2MHz     0x01                      //����ʱ��Ƶ��2MHz    Byte9.1~0
#define UP_CLK_FREQ_4MHz     0x00                      //����ʱ��Ƶ��4MHz    Byte9.1~0

#define UP_CLK_EN_500kHz     0x43                      //����ʱ��Ƶ��500kHz  Byte9.2~0   111
#define UP_CLK_EN_1MHz       0x42                      //����ʱ��Ƶ��1MHz    Byte9.2~0
#define UP_CLK_EN_2MHz       0x41                      //����ʱ��Ƶ��2MHz    Byte9.2~0
#define UP_CLK_EN_4MHz       0x40                      //����ʱ��Ƶ��4MHz    Byte9.2~0
