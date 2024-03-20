//angles are in radians
//rates are magnitudes of rad/s
//assuming roll == 0, pitchRate will be equivalent to y, and yawRate will be equivalent to x

#include "attitude.h"

#include "./utils/Ewma.h" 

//returns calculated pitchRate or yawRate
/**
 * @brief Calculate Pitch or Yaw rate given raw inputs and current roll orientation
 * 
 * @note Imagine your rocket has a sensor on it that detects X and Y pitch. If the Vehicle rolls 90 degrees, these sensors will become swapped. 
 *    If the vehicle rolls 180 degrees, the sensors will be reading the exact opposite of reality. This function normalizes the input using simple trig.
 * 
 * @param roll current roll orientation
 * @param parallel raw rate in one direction (X)
 * @param perpendicular raw rate in perpendicular direction (Y)
 * @return double actual rate in cardinal X direction
 */
double Attitude::getRate(const double roll, const double parallel, const double perpendicular)
{   
    //if getting rateX, parallel should be x
    return parallel * cos(roll) + perpendicular * sin(roll);
}

/**
 * @brief Filter raw data through an EWMA to compute "Zero Rate Offset" for each axis
 * 
 * @param rawGyro One sample of the gyroscope
 */
void Attitude::calibrateGyro(const Directional& rawGyro)
{
    static Directional lastSavedOffsets;
    static int16_t tickCounter;
    static Ewma xFilter(.002);
    static Ewma yFilter(.002);
    static Ewma zFilter(.002);

    xFilter.filter(rawGyro.x);
    yFilter.filter(rawGyro.y);
    zFilter.filter(rawGyro.z);

    tickCounter = (tickCounter + 1) % 500;
    //every 500 ticks (2-5 seconds), we save the last state and transfer the previous state to the go queue
    if(tickCounter == 0){
        m_zeroRateOffsets = lastSavedOffsets;
        lastSavedOffsets = Directional(xFilter.getOutput(), yFilter.getOutput(), zFilter.getOutput());
    }
}

/**
 * @brief normalize raw gyro data using zero rate offsets
 * 
 * @param[in, out] gyro raw gyro sample
 */
void Attitude::getCalibratedGyro(Directional& gyro)
{
    gyro.x = gyro.x - m_zeroRateOffsets.x;
    gyro.y = gyro.y - m_zeroRateOffsets.y;
    gyro.z = gyro.z - m_zeroRateOffsets.z;
}

/**
 * @brief Calculate current attitude (orientation)
 * 
 * @note In order to minimize error, we should assume that attitude is 0,0,0 at launch. However, 
 *  we don't actually know exactly when launch occurs. We can assume with almost 100% 
 *  certainty that launch is detected less than 500 ticks (2-5 seconds) after it has actually occured, so 
 *  as a substitute, we can start recording attitude changes 500 ticks before detected 
 *  launch. In order to achieve this, we can record changes in 500 tick increments (chunks) 
 *  prior to launch. Once launch is detected we will 'lock in' those chunks and continue 
 *  recording changes forever.
 * 
 * @param gyro raw gyro sample
 * @param hasLaunched has the rocket launched
 */
void Attitude::updateAttitude(Directional gyro, const bool hasLaunched, uint32_t sampleTime)
{
    static Directional oldChanges; //5-10 seconds ago
    static Directional lastChanges; //0-5 seconds ago
    static uint16_t tickCounter = 0;

    getCalibratedGyro(gyro);
    const uint32_t timeDiff = sampleTime - m_lastSampleTime;
    m_lastSampleTime = sampleTime;
    const double timeDiffSeconds = timeDiff / 1000000;

    tickCounter = (tickCounter + 1) % 500;

    lastChanges.z += gyro.z * timeDiffSeconds;
    m_attitude.z = oldChanges.z + lastChanges.z; //z must be set first so we can calculate x and y
    lastChanges.x += getRate(m_attitude.z/* -gyro.z/2 */, gyro.x, gyro.y) * timeDiffSeconds; //TODO: test whether or not -gyro.z/2 makes a difference
    lastChanges.y += getRate(m_attitude.z/* -gyro.z/2 */, gyro.y, gyro.x) * timeDiffSeconds;
    m_attitude.x = oldChanges.x + lastChanges.x;
    m_attitude.y = oldChanges.y + lastChanges.y;

    // every 5 seconds we delete all data that's more than 5 seconds old (prior to launch)
    // this minimizes accumalation of error before launch
    if(tickCounter == 0 && !hasLaunched){
        oldChanges = lastChanges;
        lastChanges.x = 0;
        lastChanges.y = 0;
        lastChanges.z = 0;
    }
}

Directional Attitude::getCurrentAttitude()
{
    return m_attitude;
}