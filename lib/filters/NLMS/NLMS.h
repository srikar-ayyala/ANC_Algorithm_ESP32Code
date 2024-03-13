#ifndef NLMS_FILTER_H
#define NLMS_FILTER_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstddef>
#endif

#include "iostream"
#include "vector.h"

class NLMS
{
private:
    float alpha;
    int order;
    float a[50];
    float c[50];
    // vector<float> c;
    int xHead;
    float x[50];
    float dcap[50];
    // vector<float> dcap;
    float y[50];
    // vector<float> y;
    float error;
    const float weightOverflowLimit = 1e8;
    bool stopFilter;

public:
    NLMS()
    {
        alpha = order = 0;
        xHead = 0;
        error = 1e9;
        for (int i = 0; i < order; i++)
        {
            x[i] = 0;
            a[i] = 0;
            c[i] = dcap[i] = y[i] = 0;
        }
        stopFilter = true;
    }
    NLMS(int order, float alpha)
    {
        this->order = 50;
        this->alpha = alpha;
        xHead = 0;
        error = 0;
        for (int i = 0; i < order; i++)
        {
            x[i] = 0;
            a[i] = 0;
            c[i] = dcap[i] = y[i] = 0;
        }
        stopFilter = false;
    }

    float updateWeights()
    {
        if (stopFilter == true)
        {
            std::cout << "Filter has stopped. ";
            std::cout << "order: " << order << " error: " << error << std::endl;
            return error;
        }

        float normX = 0, normDCap = 0;
        for (int i = 0; i < order; i++)
        {
            normX += x[i] * x[i];
            normDCap += dcap[i] * dcap[i];
        }
        if (normX != 0)
        {
            int tempXHead = xHead;
            for (int i = 0; i < order; i++)
            {
                a[i] = a[i] + ((x[tempXHead] * alpha * error) / normX);
                // a[i] = a[i] + x[tempXHead] * (alpha * error);
                tempXHead++;
                if (tempXHead >= order)
                {
                    tempXHead = 0;
                }
            }
        }
        if (normDCap != 0)
        {
            int tempXHead = xHead;
            for (int i = 0; i < order; i++)
            {
                c[i] = c[i] + ((dcap[tempXHead] * alpha * error) / normDCap);
                tempXHead++;
                if (tempXHead >= order)
                {
                    tempXHead = 0;
                }
            }
        }

        for (int i = 0; i < order; i++)
        {
            if (a[i] > weightOverflowLimit || c[i] > weightOverflowLimit)
            {
                stopFilter = true;
                break;
            }
        }
        return error;
    }
    float getOutput(float refInp, float errorInp)
    {
        if (stopFilter == true)
        {
            std::cout << "Filter has stopped. ";
            std::cout << "order: " << order << " error: " << error << std::endl;
            return 0;
        }
        error = errorInp;

        x[xHead] = refInp;
        dcap[xHead] = error + y[order - 1];
        xHead++;
        if (xHead >= order)
        {
            xHead = 0;
        }

        float output = 0;
        int tempXHead = xHead;
        for (int i = 0; i < order; i++)
        {
            output += a[i] * x[tempXHead] + c[i] * dcap[tempXHead];
            tempXHead++;
            if (tempXHead >= order)
            {
                tempXHead = 0;
            }
        }
        return output;
    }
    void pushX(int16_t val)
    {
        x[xHead] = val;
        xHead++;
        if (xHead >= order)
            xHead = 0;
    }
    void showX()
    {
        Serial.println("X:");
        for (int i = 0; i < order; i++)
        {
            Serial.print(x[i]);
            Serial.print(" ");
        }
        Serial.println("");
    }
    void showA()
    {
        Serial.println("A:");
        for (int i = 0; i < order; i++)
        {
            Serial.print(a[i]);
            Serial.print(" ");
        }
        Serial.println("");
    }
};

#endif