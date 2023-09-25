#ifndef MLX90393_H_INCLUDED
#define MLX90393_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <string.h>

class MLX90393
{
private:

    /* VARIAVEIS PRIVATE -------------------------------------------------*/
    uint8_t status;

public:

    /* VARIAVEIS ---------------------------------------------------------*/
    uint16_t statusRegister[32];

    /* ENUM --------------------------------------------------------------*/

    typedef enum {
        BURST_MODE = 0x10,
        start_WAKE_UP = 0x20,
        start_SINGLE_MEASUREMENT_MODE = 0x30,
        READ_MEASUREMENT = 0x40,
        READ_REGISTER = 0x50,
        WRITE_REGISTER = 0x60,
        EXIT_MODE = 0x80,
        MEMORY_RECALL = 0xD0,
        MEMORY_STORE = 0xE0,
        RESET = 0xF0,
    } modo;

    /* STRUCT ------------------------------------------------------------*/
    struct txyz
    {
       uint16_t t;
       uint16_t x;
       uint16_t y;
       uint16_t z;
    };

    struct txyzDate
    {
       float t;
       float x;
       float y;
       float z;
    };
    
    txyz axis;
    txyzDate date;

    struct PARAMETROS
    {
        uint8_t ANDRESS;
        uint16_t MASK;
        uint8_t BIT;
        uint8_t DATE;
    };

    PARAMETROS HALLCONF =         {0x0, 0xF, 0, 0xC};
    PARAMETROS GAIN_SEL =         {0x0, 0x7, 4, 0x5};
    PARAMETROS Z_SERIES =         {0x0, 0x1, 7, 0x0};
    PARAMETROS BIST =             {0x0, 0x1, 8, 0x0};
    PARAMETROS ANA_RESERD_LOW =   {0x0, 0x1, 9, 0x0};
    PARAMETROS BUSTER_DATA_RATE = {0x1, 0x3F, 0, 0x0};
    PARAMETROS BURST_SEL_ZYXT =   {0x1, 0xF, 6, 0x0};
    PARAMETROS TCMP_EN =          {0x1, 0x1, 10, 0x0};
    PARAMETROS EXT_TRIG =         {0x1, 0x1, 11, 0x0};
    PARAMETROS WOC_DIFF =         {0x1, 0x1, 12, 0x0};
    PARAMETROS COMM_MODE =        {0x1, 0x3, 13, 0x0};
    PARAMETROS TRIG_INT_ =        {0x1, 0x1, 15, 0x0};
    PARAMETROS OSR =              {0x2, 0x3, 0, 0x0};
    PARAMETROS DIG_FILT =         {0x2, 0x7, 2, 0x5};
    PARAMETROS RES_X =            {0x2, 0x3, 5, 0x1};
    PARAMETROS RES_Y =            {0x2, 0x3, 7, 0x1};
    PARAMETROS RES_Z =            {0x2, 0x3, 8, 0x1};
    PARAMETROS OSR2 =             {0x2, 0x3, 11, 0x0};
    PARAMETROS SENS_TC_LT =       {0x3, 0xFF, 0, 0x40};
    PARAMETROS SENS_TC_HT =       {0x3, 0xFF, 8, 0x56};
    PARAMETROS OFFSET_X =         {0x4, 0xFFFF, 0, 0x0};
    PARAMETROS OFFSET_Y =         {0x5, 0xFFFF, 0, 0x0};
    PARAMETROS OFFSET_Z =         {0x6, 0xFFFF, 0, 0x0};
    PARAMETROS WOXY_THRESHOLD =   {0x7, 0xFFFF, 0, 0x0};
    PARAMETROS WOZ_THRESHOLD =    {0x8, 0xFFFF, 0, 0x0};
    PARAMETROS WOT_THRESHOLD =    {0x9, 0xFFFF, 0, 0x0};


    /* function prototypes -----------------------------------------------*/
    MLX90393();

    /* Fundamental -------------------------------------------------------*/
    void EnviarCmd(uint8_t* cmd, uint16_t Size);
    void LerCmd(uint8_t* cmd, uint8_t* buf, uint16_t Size, uint16_t Size1);

    /* Command List ------------------------------------------------------*/

    void start_BURST_MODE(txyz zyxt);
    void start_WAKE_UP(txyz zyxt);
    void start_SINGLE_MEASUREMENT_MODE(txyz zyxt);
    void READ_MEASUREMENT(txyz zyxt);
    void READ_REGISTER(uint8_t andress, uint16_t *registe);
    void WRITE_REGISTER(uint8_t andress, uint16_t date);
    void EXIT_MODE();
    void MEMORY_RECALL();
    void MEMORY_STORE();
    void RESET();

    /* Parametros registrador --------------------------------------------*/

    /* set function ------------------------------------------------------*/

    void set_PARAMETROS(PARAMETROS parms, uint8_t date);
    
    void set_HALLCONF(uint8_t data);
    void set_GAIN_SEL(uint8_t data);
    void set_Z_SERIES(uint8_t data);
    void set_BIST(uint8_t data);
    void set_ANA_RESERD_LOW(uint8_t data);
    void set_BUSTER_DATA_RATE(uint8_t data);
    void set_BURST_SEL_ZYXT(uint8_t data);
    void set_TCMP_EN(uint8_t data);
    void set_EXT_TRIG(uint8_t data);
    void set_WOC_DIFF(uint8_t data);
    void set_COMM_MODE(uint8_t data);
    void set_TRIG_INT_(uint8_t data);
    void set_OSR(uint8_t data);
    void set_DIG_FILT(uint8_t data);
    void set_OSR2(uint8_t data);
    void set_RES_X(uint8_t data);
    void set_RES_Y(uint8_t data);
    void set_RES_Z(uint8_t data);
    void set_OFFSET_X(uint8_t data);
    void set_OFFSET_Y(uint8_t data);
    void set_OFFSET_Z(uint8_t data);
    void set_WOXY_THRESHOLD(uint8_t data);
    void set_WOZ_THRESHOLD(uint8_t data);
    void set_WOT_THRESHOLD(uint8_t data);
    void set_SENS_TC_HT(uint8_t data);
    void set_SENS_TC_LT(uint8_t data);

    /* get function ------------------------------------------------------*/

    void get_PARAMETROS(PARAMETROS parms, uint8_t *date);

    void get_HALLCONF(uint8_t *date);
    void get_GAIN_SEL(uint8_t *date);
    void get_Z_SERIES(uint8_t *date);
    void get_BIST(uint8_t *date);
    void get_ANA_RESERD_LOW(uint8_t *date);
    void get_BUSTER_DATA_RATE(uint8_t *date);
    void get_BURST_SEL_ZYXT(uint8_t *date);
    void get_TCMP_EN(uint8_t *date);
    void get_EXT_TRIG(uint8_t *date);
    void get_WOC_DIFF(uint8_t *date);
    void get_COMM_MODE(uint8_t *date);
    void get_TRIG_INT_(uint8_t *date);
    void get_OSR(uint8_t *date);
    void get_DIG_FILT(uint8_t *date);
    void get_OSR2(uint8_t *date);
    void get_RES_X(uint8_t *date);
    void get_RES_Y(uint8_t *date);
    void get_RES_Z(uint8_t *date);
    void get_OFFSET_X(uint8_t *date);
    void get_OFFSET_Y(uint8_t *date);
    void get_OFFSET_Z(uint8_t *date);
    void get_WOXY_THRESHOLD(uint8_t *date);
    void get_WOZ_THRESHOLD(uint8_t *date);
    void get_WOT_THRESHOLD(uint8_t *date);
    void get_SENS_TC_HT(uint8_t *date);
    void get_SENS_TC_LT(uint8_t *date);

    /* calculations ------------------------------------------------------*/

    void convertAxis();
    void calculationTime();
    void statusModo();
    void convertTxt();

};

#endif 