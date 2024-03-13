#ifndef CustomEspI2CInput_H
#define CustomEspI2CInput_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstddef>
#endif

#include <driver/i2s.h>
#include "cmath"

int32_t left = 0, right = 0; // Sample Rate Change Point

const int recSize = 45000;
int recIndex, recIndex2;
int16_t recording[recSize]; // Sample Rate Change Point

// Use I2S Processor 0
static const i2s_port_t I2S_PORT = I2S_NUM_0;

// Define input buffer length
static const int bufferLen = 128;
static const int bufferCnt = 2;
static int64_t sBuffer[bufferLen]; // Sample Rate Change Point // Channel Format Change Point
static size_t bytesIn = 0;

void printRecording()
{
    for (int i = 0; i < recIndex2; i++)
    {
        Serial.print(">Data:");
        Serial.println(recording[i]);
    }
    Serial.print(">Len:");
    Serial.println(recIndex2);
    // Serial.print(">Samples:");
    // Serial.println(sampleVal);
    //   Serial.print(">Times:");
    // Serial.println(countx);
}

void setupInputI2S()
{
    static const int I2S_WS = 27;
    static const int I2S_SD = 32;
    static const int I2S_SCK = 33;
    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 44100,
        // .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,  // Sample Rate Change Point
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        // .bits_per_sample = i2s_bits_per_sample_t(32),
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, // Channel Format Change Point
        // .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,  // Channel Format Change Point
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = 0,
        .dma_buf_count = bufferCnt,
        .dma_buf_len = bufferLen,
        .use_apll = 0,
    };

    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);

    const i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_SD};

    i2s_set_pin(I2S_PORT, &pin_config);

    i2s_start(I2S_PORT);

    for (int i = 0; i < recSize; i++)
    {
        recording[i] = 0;
    }
}

void getInput()
{
    esp_err_t errorStatus = i2s_read(I2S_PORT, &sBuffer, bufferLen, &bytesIn, 0);
    if (errorStatus == ESP_OK)
    {
        // Read I2S data buffer
        int16_t samples_read = bytesIn / 8;
        for (int16_t i = 0; i < samples_read; ++i)
        {
            left = (sBuffer[i] & 0xffffffff);          // Sample Rate Change Point // Channel Format Change Point
            right = ((sBuffer[i] >> 32) & 0xffffffff); // Sample Rate Change Point
            // mean1 += (sBuffer[i] & 0xffff);  // Sample Rate Change Point
            // mean2 += ((sBuffer[i] >> 16) & 0xffff); // Sample Rate Change Point
            if (recIndex < recSize)
            {
                recording[recIndex] = ((left >> 16) & 0xffff);
                recIndex++;
                recIndex2 = recIndex;
            }
        }
    }
}

void pushTestDataInRecording()
{
    recIndex = 0;
    recIndex2 = 0;
    for (float i = 0; i < recSize; i++)
    {
        if (recIndex < recSize)
        {
            // recording[recIndex] = 100 * sin(2 * PI * i / 1000);
            // recording[recIndex] = 10000 * sin(2 * PI * i / (0.0002 * 44100));
            recording[recIndex] = 10000 * sin(2 * PI * i / (0.0002 * 44100)) * sin(2 * PI * i / (0.02 * 44100));
            recIndex++;
            recIndex2 = recIndex;
        }
    }
    recIndex = 0;
}

void runInputContinous()
{
    recIndex = 0;
    recIndex2 = 0;
    getInput();
    recIndex = 0;
}

void runInputForTimeInterval(unsigned long timeInterval)
{
    recIndex = 0;
    recIndex2 = 0;
    unsigned long timeRec = millis();
    while (millis() - timeRec <= 1000)
    {
        getInput();
    }
    recIndex = 0;
}

// class SterioResult
// {
// public:
//     int16_t left, right; // Change here to change sample bit size
//     bool isSet;
//     SterioResult()
//     {
//         left = 0;
//         right = 0;
//         isSet = false;
//     }
//     void setVals(int16_t l, int16_t r, bool is) // Change here to change sample bit size
//     {
//         left = l;
//         right = r;
//         isSet = is;
//     }
// };

// esp_err_t readI2S()
// {
//     return i2s_read(I2S_PORT, &sBuffer, bufferLen, &bytesIn, 0);
// }

// SterioResult readI2SMean()
// {
//     SterioResult res;
//     res.setVals(0, 0, false);

//     esp_err_t errorStatus = readI2S();
//     if (errorStatus == ESP_OK)
//     {
//         // Read I2S data buffer
//         int16_t samples_read = bytesIn / 8;
//         if (samples_read > 0)
//         {
//             int16_t mean1 = 0, mean2 = 0; // Change here to change sample bit size
//             // for (int16_t i = 0; i < 1; ++i)
//             for (int16_t i = 0; i < samples_read; ++i)
//             {
//                 // mean1 += (sBuffer[i] & 0xffffffff); //Change here to change sample bit size
//                 // mean2 += ((sBuffer[i] >> 32) & 0xffffffff); //Change here to change sample bit size
//                 mean1 += (sBuffer[i] & 0xffff);         // Change here to change sample bit size
//                 mean2 += ((sBuffer[i] >> 16) & 0xffff); // Change here to change sample bit size
//             }

//             // Average the data reading
//             mean1 /= samples_read;
//             mean2 /= samples_read;
//             // Serial.print(">LEFT1:");
//             // Serial.println(mean1);
//             // Serial.print(">RIGHT1:");
//             // Serial.println(mean2);

//             res.setVals(mean1 * 100, mean2, true);
//             return res;
//         }
//     }

//     return res;
// }

#endif

// void readI2S()
// {
//     // I2SReadResult res;
//     size_t bytesIn = 0;
//     // res.errorResult = i2s_read(I2S_PORT, &sBuffer, 4, &bytesIn, portMAX_DELAY);
//     // res.errorResult = i2s_read(I2S_PORT, &sBuffer, bufferLen, &bytesIn, portMAX_DELAY);
//     esp_err_t result = i2s_read(I2S_PORT, &sBuffer, bufferLen, &bytesIn, portMAX_DELAY);
//     if (result == ESP_OK)
//     {
//         // Read I2S data buffer
//         int16_t samples_read = bytesIn / 8;
//         if (samples_read > 0)
//         {
//             int32_t mean1 = 0, mean2 = 0;
//             // int32_t mean3 = 0;
//             for (int16_t i = 0; i < samples_read; ++i)
//             {
//                 mean1 += (sBuffer[i] & 0xffffffff);
//                 mean2 += ((sBuffer[i] >> 32) & 0xffffffff);
//                 // mean3 += sBuffer[i];
//             }

//             // Average the data reading
//             mean1 /= samples_read;
//             mean2 /= samples_read;
//             Serial.print(">LEFT:");
//             Serial.println(mean1);
//             Serial.print(">RIGHT:");
//             Serial.println(mean2);
//             // mean3 /= samples_read;

//             // res.setVals(mean1, mean2, true);
//             // return res;
//         }
//     }
//     // res.bytesIn = bytesIn;
//     // return res;
// }

// void readI2SMean()
// {
//     // SterioResult res;

//     // I2SReadResult input =
//     readI2S();
//     // if (input.errorResult == ESP_OK)
//     // {
//     //     // Read I2S data buffer
//     //     int16_t samples_read = input.bytesIn / 8;
//     //     if (samples_read > 0)
//     //     {
//     //         int16_t mean1 = 0, mean2 = 0;
//     //         // int32_t mean3 = 0;
//     //         for (int16_t i = 0; i < samples_read; ++i)
//     //         {
//     //             mean1 += (sBuffer[i] & 0xffff);
//     //             mean2 += ((sBuffer[i] >> 16) & 0xffff);
//     //             // mean3 += sBuffer[i];
//     //         }

//     //         // Average the data reading
//     //         mean1 /= samples_read;
//     //         mean2 /= samples_read;
//     //         // mean3 /= samples_read;

//     //         res.setVals(mean1, mean2, true);
//     //         return res;
//     //     }
//     // }

//     // return res;
// }