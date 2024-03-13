#ifndef CustomEspI2COutput_H
#define CustomEspI2COutput_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstddef>
#endif

#include <driver/i2s.h>

int16_t getSquareSignal()
{
  static const uint16_t Volume = 0x3ff;  // Highest value the peak will get to, bigger gives louder vol, but
                                         // too high can lead to distortion on cheap or small speakers
  static const int16_t Peak = Volume;    // Max height/peak
  static const int16_t Trough = -Volume; // Max low/trough

  static int16_t OutputValue = Peak;               // Value to send to each channel (left/right), start off at the peak
  static const uint16_t WaveLength = 200;          // Bigger =longer wavelength and higher frequency
  static uint16_t TimeAtPeakOrTrough = WaveLength; // Var to count down how long we hold at either peak or trough
                                                   // (high/low)
  uint32_t Value32Bit;                             // This holds the value we actually send to the I2S buffers, it basically
                                                   // will hold the "OutputValue" but combined for both left/right channels

  size_t BytesWritten; // Returned by the I2S write routine, we are not interested in it but
                       // must supply it as a param to the routine.

  if (TimeAtPeakOrTrough == 0) // If 0 then we're ready to switch to a peak (high) or trough (low)
  {
    if (OutputValue == Peak) // If we were a peak (high), switch to trough (low) value
      OutputValue = Trough;
    else // else we were a trough (low) switch to peak (high)
      OutputValue = Peak;
    TimeAtPeakOrTrough = WaveLength; // Reset this counter back to max, ready to countdown again
  }
  TimeAtPeakOrTrough--; // Decrement this counter that controls wavelenght/ frequency

  return OutputValue;
}

// Use I2S Processor 0
static const i2s_port_t I2S_PORT1 = I2S_NUM_1;

// Define input buffer length
static const int bufferLen1 = 512;

void setupOutputI2S()
{
  // Connections to INMP441 I2S microphone
  static const int I2S_WS1 = 13;
  static const int I2S_SD1 = 12;
  static const int I2S_SCK1 = 14;

  static const i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = 44100,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_MSB),
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
      .dma_buf_count = 10,                      // 8 buffers
      .dma_buf_len = bufferLen1,                // 1K per buffer, so 8K of buffer space
      .use_apll = 0,
      .tx_desc_auto_clear = true,
      .fixed_mclk = -1};

  static const i2s_pin_config_t pin_config = {
      .bck_io_num = I2S_SCK1,          // The bit clock connectiom, goes to pin 27 of ESP32
      .ws_io_num = I2S_WS1,            // Word select, also known as word select or left right clock
      .data_out_num = I2S_SD1,         // Data out from the ESP32, connect to DIN on 38357A
      .data_in_num = I2S_PIN_NO_CHANGE // we are not interested in I2S data into the ESP32
  };

  i2s_driver_install(I2S_PORT1, &i2s_config, 0, NULL); // ESP32 will allocated resources to run I2S
  i2s_set_pin(I2S_PORT1, &pin_config);                 // Tell it the pins you will be using
}

void sendOutput(int16_t output)
{
  uint32_t Value32Bit;
  size_t BytesWritten;

  int16_t res = output * 100;

  Value32Bit = (res << 16) | (res & 0xffff); // R & L
  i2s_write(I2S_NUM_1, &Value32Bit, 4, &BytesWritten, portMAX_DELAY);
}

#endif