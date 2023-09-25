#include "MLX90393.h"

    /* Construction ------------------------------------------------------*/

    MLX90393::MLX90393(){
        statusRegister[0] = HALLCONF.DATE | GAIN_SEL.DATE << GAIN_SEL.BIT | Z_SERIES.DATE << Z_SERIES.BIT | BIST.DATE << BIST.BIT | ANA_RESERD_LOW.DATE << ANA_RESERD_LOW.BIT;
        statusRegister[1] = BUSTER_DATA_RATE.DATE | BURST_SEL_ZYXT.DATE << BURST_SEL_ZYXT.BIT | TCMP_EN.DATE << TCMP_EN.BIT | EXT_TRIG.DATE << EXT_TRIG.BIT | WOC_DIFF.DATE << WOC_DIFF.BIT | COMM_MODE.DATE << COMM_MODE.BIT | TRIG_INT_.DATE << TRIG_INT_.BIT;
        statusRegister[2] = OSR.DATE | DIG_FILT.DATE << DIG_FILT.BIT | RES_X.DATE << RES_X.BIT | RES_Y.DATE << RES_Y.BIT | RES_Z.DATE << RES_Z.BIT | OSR2.DATE << OSR2.BIT;
        statusRegister[3] = SENS_TC_LT.DATE | SENS_TC_HT.DATE << SENS_TC_HT.BIT;
        statusRegister[4] = OFFSET_X.DATE;
        statusRegister[5] = OFFSET_Y.DATE;
        statusRegister[6] = OFFSET_Z.DATE;
        statusRegister[7] = WOXY_THRESHOLD.DATE;
        statusRegister[8] = WOZ_THRESHOLD.DATE;
        statusRegister[9] = WOT_THRESHOLD.DATE;

        for (int i = 0; i < 10; i++)
        {
            WRITE_REGISTER(i, statusRegister[i]);
        }
    }

    /* Fundamental -------------------------------------------------------*/
    void MLX90393::EnviarCmd(uint8_t* cmd, uint16_t Size){
        // Start SPI Transmission
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_SPI_Transmit(&hspi2, cmd, Size, HAL_MAX_DELAY);
        HAL_SPI_Receive(&hspi2, &status, 1, HAL_MAX_DELAY);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
        HAL_Delay(20);
    }
    void MLX90393::LerCmd(uint8_t* cmd, uint8_t* buf, uint16_t Size, uint16_t Size1){
        // Start SPI Transmission
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_SPI_Transmit(&hspi2, cmd, Size, HAL_MAX_DELAY);
        HAL_SPI_Receive(&hspi2, &status, 1, HAL_MAX_DELAY);
        HAL_SPI_Receive(&hspi2, buf, Size1, HAL_MAX_DELAY);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
        HAL_Delay(20);
    }

    /* Command List ------------------------------------------------------*/

    void MLX90393::start_BURST_MODE(txyz zyxt){
        uint8_t cmd = modo::BURST_MODE | zyxt.z << 3 | zyxt.y << 2 | zyxt.x < 1 | zyxt.t;
        EnviarCmd(&cmd, 1);

    }
    void MLX90393::start_WAKE_UP(txyz zyxt){
        uint8_t cmd = modo::start_WAKE_UP | zyxt.z << 3 | zyxt.y << 2 | zyxt.x < 1 | zyxt.t;
        EnviarCmd(&cmd, 1);
    }
    void MLX90393::start_SINGLE_MEASUREMENT_MODE(txyz zyxt){
        uint8_t cmd = modo::start_SINGLE_MEASUREMENT_MODE | zyxt.z << 3 | zyxt.y << 2 | zyxt.x < 1 | zyxt.t;
        EnviarCmd(&cmd, 1);
    }
    void MLX90393::READ_MEASUREMENT(txyz zyxt){

        uint8_t cmd = modo::READ_MEASUREMENT | zyxt.z << 3 | zyxt.y << 2 | zyxt.x < 1 | zyxt.t;
        uint8_t bufSize = 2*(status & 0x3)+2;
        uint8_t buf[bufSize];

        LerCmd(&cmd, buf, 1, bufSize);

        axis.t = buf[0] << 8 | buf[1];
        axis.x = buf[2] << 8 | buf[3];
        axis.y = buf[4] << 8 | buf[5];
        axis.z = buf[6] << 8 | buf[7];

    }
    void MLX90393::READ_REGISTER(uint8_t andress, uint16_t *registe){
        uint8_t readRegister[2];
        uint8_t registeBuf[2];

        readRegister[0] = modo::READ_REGISTER;
        readRegister[1] = andress << 2;

        LerCmd(readRegister, registeBuf, 2, 2);
        *registe = registeBuf[0] << 8 | registeBuf[1];
    }
    void MLX90393::WRITE_REGISTER(uint8_t andress, uint16_t date){

        uint8_t writeRegister[4];

        writeRegister[0] = modo::WRITE_REGISTER;
        writeRegister[1] = date >> 8;
        writeRegister[2] = date && 0xFF;
        writeRegister[3] = andress << 2;

        EnviarCmd(writeRegister, 4);
    }
    void MLX90393::EXIT_MODE(){
        uint8_t cmd = modo::EXIT_MODE;
        EnviarCmd(&cmd, 1);
    }
    void MLX90393::MEMORY_RECALL(){
        uint8_t cmd = modo::EXIT_MODE;
        EnviarCmd(&cmd, 1);
    }
    void MLX90393::MEMORY_STORE(){
        uint8_t cmd = modo::MEMORY_STORE;
        EnviarCmd(&cmd, 1);
    }
    void MLX90393::RESET(){
        uint8_t cmd = modo::RESET;
        EnviarCmd(&cmd, 1);
    }

    /* Parametros registrador --------------------------------------------*/

    /* set function ------------------------------------------------------*/

    void MLX90393::set_PARAMETROS(PARAMETROS parms, uint8_t date){
        statusRegister[parms.ANDRESS] = (statusRegister[parms.ANDRESS] & ~(parms.MASK << parms.BIT)) | date << parms.BIT;
        WRITE_REGISTER(parms.ANDRESS, statusRegister[parms.ANDRESS]);
    }
    
    void MLX90393::set_HALLCONF(uint8_t data){
        set_PARAMETROS(HALLCONF, data);
    }
    void MLX90393::set_GAIN_SEL(uint8_t data){
        set_PARAMETROS(GAIN_SEL, data);
    }
    void MLX90393::set_Z_SERIES(uint8_t data){
        set_PARAMETROS(Z_SERIES, data);
    }
    void MLX90393::set_BIST(uint8_t data){
        set_PARAMETROS(BIST, data);
    }
    void MLX90393::set_ANA_RESERD_LOW(uint8_t data){
        set_PARAMETROS(ANA_RESERD_LOW, data);
    }
    void MLX90393::set_BUSTER_DATA_RATE(uint8_t data){
        set_PARAMETROS(BUSTER_DATA_RATE, data);
    }
    void MLX90393::set_BURST_SEL_ZYXT(uint8_t data){
        set_PARAMETROS(BURST_SEL_ZYXT, data);
    }
    void MLX90393::set_TCMP_EN(uint8_t data){
        set_PARAMETROS(TCMP_EN, data);
    }
    void MLX90393::set_EXT_TRIG(uint8_t data){
        set_PARAMETROS(EXT_TRIG, data);
    }
    void MLX90393::set_WOC_DIFF(uint8_t data){
        set_PARAMETROS(WOC_DIFF, data);
    }
    void MLX90393::set_COMM_MODE(uint8_t data){
        set_PARAMETROS(COMM_MODE, data);
    }
    void MLX90393::set_TRIG_INT_(uint8_t data){
        set_PARAMETROS(TRIG_INT_, data);
    }
    void MLX90393::set_OSR(uint8_t data){
        set_PARAMETROS(OSR, data);
    }
    void MLX90393::set_DIG_FILT(uint8_t data){
        set_PARAMETROS(DIG_FILT, data);
    }
    void MLX90393::set_OSR2(uint8_t data){
        set_PARAMETROS(OSR2, data);
    }
    void MLX90393::set_RES_X(uint8_t data){
        set_PARAMETROS(RES_X, data);
    }
    void MLX90393::set_RES_Y(uint8_t data){
        set_PARAMETROS(RES_Y, data);
    }
    void MLX90393::set_RES_Z(uint8_t data){
        set_PARAMETROS(RES_Z, data);
    }
    void MLX90393::set_OFFSET_X(uint8_t data){
        set_PARAMETROS(OFFSET_X, data);
    }
    void MLX90393::set_OFFSET_Y(uint8_t data){
        set_PARAMETROS(OFFSET_Y, data);
    }
    void MLX90393::set_OFFSET_Z(uint8_t data){
        set_PARAMETROS(OFFSET_Z, data);
    }
    void MLX90393::set_WOXY_THRESHOLD(uint8_t data){
        set_PARAMETROS(WOXY_THRESHOLD, data);
    }
    void MLX90393::set_WOZ_THRESHOLD(uint8_t data){
        set_PARAMETROS(WOZ_THRESHOLD, data);
    }
    void MLX90393::set_WOT_THRESHOLD(uint8_t data){
        set_PARAMETROS(WOT_THRESHOLD, data);
    }
    void MLX90393::set_SENS_TC_HT(uint8_t data){
        set_PARAMETROS(SENS_TC_HT, data);
    }
    void MLX90393::set_SENS_TC_LT(uint8_t data){
        set_PARAMETROS(SENS_TC_LT, data);
    }

    /* get function ------------------------------------------------------*/

    void MLX90393::get_PARAMETROS(PARAMETROS parms, uint8_t *date){
        READ_REGISTER(parms.ANDRESS, &statusRegister[parms.ANDRESS]);
        *date = statusRegister[parms.ANDRESS] >> parms.BIT & parms.MASK;
    }

    void MLX90393::get_HALLCONF(uint8_t *date){
        get_PARAMETROS(HALLCONF, date);
    }
    void MLX90393::get_GAIN_SEL(uint8_t *date){
        get_PARAMETROS(GAIN_SEL, date);
    }
    void MLX90393::get_Z_SERIES(uint8_t *date){
        get_PARAMETROS(Z_SERIES, date);
    }
    void MLX90393::get_BIST(uint8_t *date){
        get_PARAMETROS(BIST, date);
    }
    void MLX90393::get_ANA_RESERD_LOW(uint8_t *date){
        get_PARAMETROS(ANA_RESERD_LOW, date);
    }
    void MLX90393::get_BUSTER_DATA_RATE(uint8_t *date){
        get_PARAMETROS(BUSTER_DATA_RATE, date);
    }
    void MLX90393::get_BURST_SEL_ZYXT(uint8_t *date){
        get_PARAMETROS(BURST_SEL_ZYXT, date);
    }
    void MLX90393::get_TCMP_EN(uint8_t *date){
        get_PARAMETROS(TCMP_EN, date);
    }
    void MLX90393::get_EXT_TRIG(uint8_t *date){
        get_PARAMETROS(EXT_TRIG, date);
    }
    void MLX90393::get_WOC_DIFF(uint8_t *date){
        get_PARAMETROS(WOC_DIFF, date);
    }
    void MLX90393::get_COMM_MODE(uint8_t *date){
        get_PARAMETROS(COMM_MODE, date);
    }
    void MLX90393::get_TRIG_INT_(uint8_t *date){
        get_PARAMETROS(TRIG_INT_, date);
    }
    void MLX90393::get_OSR(uint8_t *date){
        get_PARAMETROS(OSR, date);
    }
    void MLX90393::get_DIG_FILT(uint8_t *date){
        get_PARAMETROS(DIG_FILT, date);
    }
    void MLX90393::get_OSR2(uint8_t *date){
        get_PARAMETROS(OSR2, date);
    }
    void MLX90393::get_RES_X(uint8_t *date){
        get_PARAMETROS(RES_X, date);
    }
    void MLX90393::get_RES_Y(uint8_t *date){
        get_PARAMETROS(RES_Y, date);
    }
    void MLX90393::get_RES_Z(uint8_t *date){
        get_PARAMETROS(RES_Z, date);
    }
    void MLX90393::get_OFFSET_X(uint8_t *date){
        get_PARAMETROS(OFFSET_X, date);
    }
    void MLX90393::get_OFFSET_Y(uint8_t *date){
        get_PARAMETROS(OFFSET_Y, date);
    }
    void MLX90393::get_OFFSET_Z(uint8_t *date){
        get_PARAMETROS(OFFSET_Z, date);
    }
    void MLX90393::get_WOXY_THRESHOLD(uint8_t *date){
        get_PARAMETROS(WOXY_THRESHOLD, date);
    }
    void MLX90393::get_WOZ_THRESHOLD(uint8_t *date){
        get_PARAMETROS(WOZ_THRESHOLD, date);
    }
    void MLX90393::get_WOT_THRESHOLD(uint8_t *date){
        get_PARAMETROS(WOT_THRESHOLD, date);
    }
    void MLX90393::get_SENS_TC_HT(uint8_t *date){
        get_PARAMETROS(SENS_TC_HT, date);
    }
    void MLX90393::get_SENS_TC_LT(uint8_t *date){
        get_PARAMETROS(SENS_TC_LT, date);
    }

    /* calculations ------------------------------------------------------*/

    void MLX90393::convertAxis(){
        uint8_t hallconf = (statusRegister[HALLCONF.ANDRESS] & ~HALLCONF.MASK) >> HALLCONF.BIT;
        uint8_t gain_sel = (statusRegister[GAIN_SEL.ANDRESS] & ~GAIN_SEL.MASK) >> GAIN_SEL.BIT;
        uint8_t tcmp_en = (statusRegister[TCMP_EN.ANDRESS] & ~TCMP_EN.MASK) >> TCMP_EN.BIT;
        uint8_t res_x = (statusRegister[RES_X.ANDRESS] & ~RES_X.MASK) >> RES_X.BIT;
        uint8_t res_y = (statusRegister[RES_Y.ANDRESS] & ~RES_Y.MASK) >> RES_Y.BIT;
        uint8_t res_z = (statusRegister[RES_Z.ANDRESS] & ~RES_Z.MASK) >> RES_Z.BIT;

        float gainX;
        float gainY;
        float gainZ;

        float gain_gain_sel[8];

        gain_gain_sel[0] = 5;
        gain_gain_sel[1] = 4;
        gain_gain_sel[2] = 3; 
        gain_gain_sel[3] = 2.5;
        gain_gain_sel[4] = 2;
        gain_gain_sel[5] = 2.5/1.5;
        gain_gain_sel[6] = 2/1.5;
        gain_gain_sel[7] = 1;

        date.t = (axis.t-46244)/45.2+25;

        gainX = (/*(date.t-25)*0.0007 + */0.150) * res_x * ((hallconf == 0xC)? 1: (98/75)) * gain_gain_sel[gain_sel];
        gainY = (/*(date.t-25)*0.0007 + */0.150) * res_y * ((hallconf == 0xC)? 1: (98/75)) * gain_gain_sel[gain_sel];
        gainZ = (/*(date.t-25)*0.0011 + */0.242) * res_z * ((hallconf == 0xC)? 1: (98/75)) * gain_gain_sel[gain_sel];

        if(tcmp_en){
            date.x = (axis.x-32768)*gainX;
            date.y = (axis.x-32768)*gainY;
            date.z = (axis.x-32768)*gainZ;
        }else{
            if(res_x <= 1){
                date.x = ((axis.x > 32768)? axis.x:~axis.x)*gainX;
            }else{
                date.x = (axis.x-32768)*gainX;
            }
            if(res_y <= 1){
                date.y = ((axis.y > 32768)? axis.y:~axis.y)*gainY;
            }else{
                date.y = (axis.y-32768)*gainY;
            }
            if(res_z <= 1){
                date.z = ((axis.z > 32768)? axis.z:~axis.z)*gainZ;
            }else{
                date.z = (axis.x-32768)*gainZ;
            }
        }
    }
    void MLX90393::calculationTime(){}
    void MLX90393::statusModo(){}
    void MLX90393::convertTxt(){
        uint8_t tempStr[6] = "";
        sprintf((char *)tempStr,"%d", (int)date.t);
        uint8_t magXstr[6] = "";
        sprintf((char *)magXstr,"%d", (int)date.x);
        uint8_t magYstr[6] = "";
        sprintf((char *)magYstr,"%d", (int)date.y);
        uint8_t magZstr[6] = "";
        sprintf((char *)magZstr,"%d", (int)date.z);
        uint8_t text[50] = "\tTemp: ";

        strcat((char *)text, (char *)tempStr);
        strcat((char *)text, "\tMagX: ");
        strcat((char *)text, (char *)magXstr);
        strcat((char *)text, "\tMagY: ");
        strcat((char *)text, (char *)magYstr);
        strcat((char *)text, "\tMagZ: ");
        strcat((char *)text, (char *)magZstr);
        strcat((char *)text, "\r\n");
    }