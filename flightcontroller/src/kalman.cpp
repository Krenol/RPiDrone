#include "kalman.h"
#include <Arduino.h>

void Kalman::init(double xHat[2], double p[2][2], double q[2][2], double r) 
{
    m_r = r;
    for (int i = 0; i < 2; i++)
    {
        m_xHat[i] = xHat[i];
        m_xHatBar[i] = 0;
        m_k[i] = 0;
        for (int j = 0; j < 2; j++)
        {
            m_p[i][j] = p[i][j];
            m_q[i][j] = q[i][j];
            m_pBar[i][j] = 0;
        }
    }
    zeroTime();
    initialized_ = true;
}

void Kalman::zeroTime() 
{
    dt_ = 0;
    millis_ = millis();
}

double Kalman::getAngles(double angularVel, double angle) 
{
    if(!initialized_) return 0;
    setDt();
    updateStep(angularVel, angle);
    return m_xHat[0];
}

void Kalman::setDt() 
{
    long m = millis();
    dt_ = m - millis_;
    millis_ = m;
}

void Kalman::updateStep(double angularVel, double angle) 
{
    // Prediction
    m_xHatBar[0] = m_xHat[0] + dt_ * (angularVel - m_xHat[1]);
    m_xHatBar[1] = m_xHat[1];

    m_pBar[0][0] = m_p[0][0] + dt_ * ((dt_ * m_p[1][1]) - m_p[1][0] - m_p[0][1] + m_q[0][0]);
    m_pBar[0][1] = m_p[0][1] + dt_ * (m_q[0][1] - m_p[1][1]);
    m_pBar[1][0] = m_p[1][0] + dt_ * (m_q[1][0] - m_p[1][1]);
    m_pBar[1][1] = m_p[1][1] + dt_ * (m_q[1][1]);

    // Update
    m_k[0] = m_pBar[0][0] / (m_pBar[0][0] + m_r);
    m_k[1] = m_pBar[1][0] / (m_pBar[0][0] + m_r);

    m_xHat[0] = m_xHatBar[0] + m_k[0] * (angle - m_xHatBar[0]);
    m_xHat[1] = m_xHatBar[1] + m_k[1] * (angle - m_xHatBar[0]);

    m_p[0][0] = m_pBar[0][0] - (m_k[0] * m_pBar[0][0]);
    m_p[0][1] = m_pBar[0][1] - (m_k[0] * m_pBar[0][1]);
    m_p[1][0] = m_pBar[1][0] - (m_k[1] * m_pBar[0][0]);
    m_p[1][1] = m_pBar[1][1] - (m_k[1] * m_pBar[0][1]);
}
