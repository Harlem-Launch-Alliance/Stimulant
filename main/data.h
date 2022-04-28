/*****************************************************************************
 * This file is for saving and transmitting data
 * currently using an SD card and Xbee respectively
 ****************************************************************************/


void transmitData(double altitude, gpsReading gps, bool apogeeReached)
{
  if(apogeeReached)
    Serial1.print("1"); //descending or on ground
  else
    Serial1.print("0"); //ascending or on pad
  Serial1.print(" ");
  Serial1.print(gps.latitude, 4);
  Serial1.print(" ");
  Serial1.print(gps.longitude, 4);
  Serial1.print(" ");
  Serial1.print(altitude,1);
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
