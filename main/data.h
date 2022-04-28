/*****************************************************************************
 * This file is for saving and transmitting data
 * currently using an SD card and Xbee respectively
 ****************************************************************************/
const int chipSelect = BUILTIN_SDCARD;
String filename = "datalog.csv";

void transmitData(double altitude, gpsReading gps, char phase)
{
  Serial1.print(phase); //0: ONPAD, 1: ASCENDING, 2: DESCENDING, 3: POST_FLIGHT
  Serial1.print(" ");
  Serial1.print(gps.latitude, 4);
  Serial1.print(" ");
  Serial1.print(gps.longitude, 4);
  Serial1.print(" ");
  Serial1.println(altitude,1);
}

void setupSD(String date){
  if (!SD.begin(chipSelect)){
    Serial1.println("MicroSD card: failed or not present");
    return;
  }
  else
    Serial1.println("MicroSD card: successful");
  filename = "datalog-".concat(date).concat(".csv");
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.println("Time(ms),Altitude(m),Latitude,Longitude,GyroX,GyroY,GyroZ,AccelX,AccelY,AccelZ,AttitudeX,AttitudeY,AttitudeZ,state");
    dataFile.close();
  }
  else {
    Serial1.println("error opening datalog.txt");
  }
}

void recordData() //PENDING SD.write timing check
//we'll probably have to split this into two functions. One to record data to RAM, the other to dump into the SD at apogee
{
  File dataFile = SD.open(filename, FILE_WRITE);
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
