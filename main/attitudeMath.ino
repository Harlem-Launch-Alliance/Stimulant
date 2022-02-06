//angles are in radians
//rates are magnitudes of rad/s
//GyX, GyY, GyZ are readings from gyro
//pitch, yaw, roll, pitchRate, yawRate are calculated based on readings
//assuming roll == 0, pitchRate will be equivalent to GyY, and yawRate will be equivalent to GyX


//returns calculated pitchRate or yawRate
double getRate(double roll, double parallel, double perpendicular){//parallel would be GyY to get pitchRate, with perpendicular being GyX
  return parallel * cos(roll) + perpendicular * sin(roll)
}
