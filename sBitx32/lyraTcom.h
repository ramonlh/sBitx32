
typedef struct {    // datos I2C lyraT
      uint8_t comtype;
      uint16_t min_f;    // lim inferior filtro
      uint16_t max_f;    // lim superior filtro
      uint16_t gain;     // gain
      uint16_t spspan;   // spectrum span 
      uint16_t spatt;      // spectrum att
      uint16_t volume;   // volume
      uint16_t ssbmode;  // lsb/usb
      uint16_t cwmode;   // cwmode
} datalyratype;
      datalyratype datalyra;
      uint8_t *bufflyra = (uint8_t *) &datalyra; // acceder a datalyratype como bytes

int sendtoLyraT(int valueType)
{
  if (valueType > 0)
    {
    datalyra.comtype=valueType;
    Wire.beginTransmission(LYRAT_ADDRESS);  
    Wire.write(bufflyra, sizeof(datalyra));                  
    Wire.endTransmission();
    if (datalyra.comtype==3)  // spectrum
      {
        ////////////////////////////////////////////////////
      long tini=millis();
      int i=0;
      int bytesrec = Wire.requestFrom(LYRAT_ADDRESS, BUFFER_I2C_LEN, 1);
      while(Wire.available()) {
        spval[i] = Wire.read();
        if (spval[i]>= 128) spval[i]=0;
        i++;
        }
      }
    }
  return 0;
}

void sendFilterLyraT(uint16_t min_f, uint16_t max_f)
{
    datalyra.comtype=1;   // set filter
    datalyra.min_f=min_f;
    datalyra.max_f=max_f;  
    sendtoLyraT(datalyra.comtype);
}

void sendGainLyraT(uint16_t gain)
{
    datalyra.comtype=2;   // set gain
    datalyra.gain=gain;  
    sendtoLyraT(datalyra.comtype);
}

void getSpectrumLyraT()
{
    datalyra.comtype=3;   // get spectrum
    sendtoLyraT(datalyra.comtype);
}

void sendVolumeLyraT(uint16_t volume)
{
    datalyra.comtype=4;   // set volume
    datalyra.volume=volume;  
    sendtoLyraT(datalyra.comtype);
}

void sendSpectrumAttLyraT(uint16_t spatt)
{
    datalyra.comtype=5;   // set spectrum scale
    datalyra.spatt=spatt;  
    sendtoLyraT(datalyra.comtype);
}

void sendSpectrumSpanLyraT(uint16_t spspan)
{
    datalyra.comtype=6;   // set spectrum span
    datalyra.spspan=spspan;  
    sendtoLyraT(datalyra.comtype);
}
