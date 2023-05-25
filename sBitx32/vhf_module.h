

typedef struct {    // datos I2C vhf-module
      uint8_t comtype;
      long freqrx;    // frequency RX
      long freqtx;    // frequency TX
      uint8_t volume;    // volume
      uint8_t sqlevel;   // squelch level
      uint16_t ctcsstx;      // ctcss tx
      uint16_t ctcssrx;      // ctcss rx
      uint8_t preemph;   // preemph 0/1
      uint8_t highpass;   // High pass filter 0/1
      uint8_t lowpass;   // Low pass filter 0/1
} datavhftype;
      datavhftype datavhf;
      uint8_t *buffvhf = (uint8_t *) &datavhf; // acceder a datavhftype como bytes

int sendtovhf(int valueType)
{
  Serial2.print("Sending comtype="); Serial2.println(valueType);
    Serial2.print("comtype:"); Serial2.println(datavhf.comtype);
    Serial2.print("freqrx:"); Serial2.println(datavhf.freqrx);
    Serial2.print("freqtx:"); Serial2.println(datavhf.freqtx);
    Serial2.print("volume:"); Serial2.println(datavhf.volume);
    Serial2.print("sqlevel:"); Serial2.println(datavhf.sqlevel);
    Serial2.print("ctcsstx:"); Serial2.println(datavhf.ctcsstx);
    Serial2.print("ctcssrx:"); Serial2.println(datavhf.ctcssrx);
    Serial2.print("highpass:"); Serial2.println(datavhf.highpass);
    Serial2.print("lowpass:"); Serial2.println(datavhf.lowpass);
    Serial2.println("===================================");
  if (valueType > 0)
    {
    datavhf.comtype=valueType;
    Wire.beginTransmission(VHFMODULE_ADDRESS);  
    Wire.write(buffvhf, sizeof(datavhf));                  
    Wire.endTransmission();
    }
  return 0;
}

void setvhffreqrx(double freqrx)
{
    datavhf.comtype=1;              // set freq rx
    datavhf.freqrx=freqrx;
    sendtovhf(datavhf.comtype);
}

void setvhffreqtx(double freqtx)
{
    datavhf.comtype=2;              // set freq tx
    datavhf.freqtx=freqtx;
    sendtovhf(datavhf.comtype);
}

void setvhfvolume(uint8_t volume)
{
    datavhf.comtype=3;              // set volume
    datavhf.volume=volume;
    sendtovhf(datavhf.comtype);
}

void setvhfpreemph(uint8_t preemph)
{
    datavhf.comtype=4;              // set preemph
    datavhf.preemph=preemph;
    sendtovhf(datavhf.comtype);
}
void setvhfhighpass(uint8_t highpass)
{
    datavhf.comtype=5;              // set highpass
    datavhf.highpass=highpass;
    sendtovhf(datavhf.comtype);
}

void setvhflowpass(uint8_t lowpass)
{
    datavhf.comtype=6;              // set lowpass
    datavhf.lowpass=lowpass;
    sendtovhf(datavhf.comtype);
}

void setvhfsqlevel(uint8_t sqlevel)
{
    datavhf.comtype=7;              // set sqlevel
    datavhf.sqlevel=sqlevel;
    sendtovhf(datavhf.comtype);
}

void setvhfctcsstx(uint8_t ctcsstx)
{
    datavhf.comtype=8;              // set ctcsstx
    datavhf.ctcsstx=ctcsstx;
    sendtovhf(datavhf.comtype);
}

void setvhfctcssrx(uint8_t ctcssrx)
{
    datavhf.comtype=9;              // set ctcssrx
    datavhf.ctcssrx=ctcssrx;
    sendtovhf(datavhf.comtype);
}
