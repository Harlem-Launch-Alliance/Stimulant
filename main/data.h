/*****************************************************************************
 * This file is for saving and transmitting data
 * currently using an SD card and Xbee respectively
 ****************************************************************************/


void transmitData(double temp, double pressure, double altitude, Directional accel, Directional gyro, bool apogeeReached)
{
  Serial1.print(millis()); Serial1.print("\t");
  //Serial1.print(tc); Serial1.print("\t");
  Serial1.print(temp); Serial1.print("\t");
  //Serial1.print(pitch); Serial1.print("\t");
  //Serial1.print(roll); Serial1.print("\t");
  Serial1.print(accel.x); Serial1.print("\t");
  Serial1.print(accel.y); Serial1.print("\t");
  Serial1.print(accel.z); Serial1.print("\t");
  Serial1.print(gyro.x); Serial1.print("\t");
  Serial1.print(gyro.y); Serial1.print("\t");
  Serial1.print(gyro.z); Serial1.print("\t");
  Serial1.print(pressure); Serial1.print("\t"); Serial1.print("\t");    
  Serial1.print(altitude); Serial1.print("\t");
  Serial1.println(apogeeReached);
}

void recordData() //PENDING SD.write timing check
//we'll probably have to split this into two functions. One to record data to RAM, the other to dump into the SD at apogee
{
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
//    dataFile.print(Time); dataFile.print("\t");
    //dataFile.print(tc); dataFile.print("\t");
//    dataFile.print(temperature); dataFile.print("\t");
//    dataFile.print(pitch); dataFile.print("\t");
//    dataFile.print(roll); dataFile.print("\t");
//    dataFile.print(Ax); dataFile.print("\t");
//    dataFile.print(Ay); dataFile.print("\t");
//    dataFile.print(Az); dataFile.print("\t");
//    dataFile.print(Gx); dataFile.print("\t");
//    dataFile.print(Gy); dataFile.print("\t");
//    dataFile.print(Gz); dataFile.print("\t");
//    dataFile.print(pressure); dataFile.print("\t"); dataFile.print("\t");
//    dataFile.print(Altitude); dataFile.print("\t");
//    dataFile.println(apogeeReached);
    dataFile.close();                       // closing file is very important
  }
  else
    Serial1.println("error opening datalog.txt");
}
