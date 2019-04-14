#include "DS1302.h"


/*SCLK--B12 IO--B13 CE--B14 */

#define DS_SDA_DATA GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) //IO
#define DS_SDA_HIGH GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define DS_SDA_LOW GPIO_ResetBits(GPIOB, GPIO_Pin_13)

#define DS_SCK_HIGH GPIO_SetBits(GPIOB, GPIO_Pin_12) //SCL
#define DS_SCK_LOW GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define DS_RST_HIGH GPIO_SetBits(GPIOB, GPIO_Pin_14) //CE
#define DS_RST_LOW GPIO_ResetBits(GPIOB, GPIO_Pin_14)

static void Delay_us(uint16_t us)
{
    uint16_t a;
    while(us)
    {
        a = 16;
        while(a--);
        us--;
    }
    return;
}


void DS1302_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOB, GPIO_Pin_14);							//CE拉低
    GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13); //拉高
}

void DS_IO_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DS_IO_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//写入一个字节
void write_one_byte(unsigned char data)
{
    unsigned char i;
    DS_IO_OUT();
    for (i = 0; i < 8; i++)
    {
        DS_SCK_LOW;
        if (data & 0x01) //从低位开始
        {
            DS_SDA_HIGH;
        }
        else
        {
            DS_SDA_LOW;
        }
        Delay_us(2);
        DS_SCK_HIGH;
        data >>= 1;
        Delay_us(2);
    }
}

//读一个字节
unsigned char read_one_byte(void)
{
    unsigned char i, data;
    DS_IO_IN();
    for (i = 0; i < 8; i++)
    {
        data >>= 1;
        DS_SCK_HIGH;
        if (DS_SDA_DATA == 1)
        {
            data |= 0x80;
        }
        else
        {
            data &= 0x7F;
        }
        Delay_us(2);
        DS_SCK_LOW;
    }
    return data;
}

//ds1302写寄存器写数据
void ds1302_write_data(unsigned char reg, unsigned char data)
{

    DS_SCK_LOW;
    DS_RST_LOW;          //初始rst为低
    Delay_us(2);
    DS_RST_HIGH;		 //sck为低时rst才可置高
    write_one_byte(reg); //sck低到高
    Delay_us(5);
    DS_SCK_LOW;
    write_one_byte(data); //sck低到高
    Delay_us(5);
    //DS_SCK_HIGH;
    DS_RST_LOW;
}
//读ds1302寄存器数据
unsigned char ds1302_read_data(unsigned char reg)
{
    unsigned char temp;
    DS_SCK_LOW;
    DS_RST_LOW;          //初始rst为低
    Delay_us(2);
    DS_RST_HIGH;         //sck为低时rst才可置高
    Delay_us(2);
    write_one_byte(reg); //sck低到高
    Delay_us(5);
    DS_SCK_LOW;
    temp = read_one_byte(); //sck由高到低
    Delay_us(5);
    //DS_SCK_HIGH;
    DS_RST_LOW;
    return temp;
}

unsigned char hex_to_bcd(unsigned char hex_data)
{
    unsigned char temp;
    temp = (hex_data / 10 * 16 + hex_data % 10);
    return temp;
}

unsigned char bcd_to_hex(unsigned char bcd_data)
{
    unsigned char temp;
    temp = (bcd_data / 16 * 10 + bcd_data % 16);
    return temp;
}

//设置寄存器时间
void time_set(unsigned char DSyear,
              unsigned char DSweek,
              unsigned char DSmonth,
              unsigned char DSday,
              unsigned char DShour,
              unsigned char DSminute,
              unsigned char DSsecond)
{
    ds1302_write_data(0x8e, 0x00); //关闭写保护

    ds1302_write_data(writeYear, DSyear);//写入hex格式数据
    ds1302_write_data(writeWeek, DSweek);
    ds1302_write_data(WriteMonth, DSmonth);
    ds1302_write_data(WriteDay, DSday);
    ds1302_write_data(WriteHour, DShour);
    ds1302_write_data(WriteMinute, DSminute);
    ds1302_write_data(WriteSecond, DSsecond);

    ds1302_write_data(0x8e, 0x80);//开启写保护
}


//读取寄存器时间
void time_read(unsigned char *DSyear,
               unsigned char *DSweek,
               unsigned char *DSmonth,
               unsigned char *DSday,
               unsigned char *DShour,
               unsigned char *DSminute,
               unsigned char *DSsecond)
{
    *DSyear = ds1302_read_data(ReadYear);
    *DSweek = ds1302_read_data(ReadWeek);
    *DSmonth = ds1302_read_data(ReadMonth);
    *DSday = ds1302_read_data(ReadDay);
    *DShour = ds1302_read_data(ReadHour);
    *DSminute = ds1302_read_data(ReadMinute);
    *DSsecond = ds1302_read_data(ReadSecond);
}
