#ifndef LMS_FILTER_H
#define LMS_FILTER_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstddef>
#endif

#include "iostream"
#include "vector.h"

class LMS
{
private:
    float alpha;
    int order;
    vector<float> a;
    // vector<float> c;
    vector<float> x;
    // vector<float> dcap;
    vector<float> y;
    float error;
    const float weightOverflowLimit = 1e8;
    bool stopFilter;

public:
    LMS()
    {
        alpha = order = 0;
        a = vector<float>();
        // c = vector<float>();
        x = vector<float>();
        // dcap = vector<float>();
        y = vector<float>();
        error = 1e9;
        stopFilter = true;
    }
    LMS(int order, float alpha)
    {
        this->order = order;
        this->alpha = alpha;
        a = vector<float>(order);a.fill(0);
        // c = vector<float>(order);c.fill(0);
        x = vector<float>(order);x.fill(0);
        // dcap = vector<float>(order); dcap.fill(0);
        y = vector<float>(order);y.fill(0);
        error = 0;
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
        a = a + x * (alpha * error);
        // c = c + dcap * (alpha * error);
        
        // if (a > weightOverflowLimit || c > weightOverflowLimit)
        if (a > weightOverflowLimit)
        {
            stopFilter = true;
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
        x.push(refInp);
        // dcap.push(error + y[y.size() - 1]);
        // float output = x * a + dcap * c;
        float output = x * a;
        y.push(output);
        return output;
    }
};

#endif