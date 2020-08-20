//-------------------------------------------------------------------------
//  /*Basic demo for accelerometer readings from Adafruit MPU6050*/
// https://learn.adafruit.com/mpu6050-6-dof-accelerometer-and-gyro?view=all
//-------------------------------------------------------------------------

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#define window_size 50

Adafruit_MPU6050 mpu;

// Define pins connecting to the L293D
const byte ENABLE_PIN = 5;
const byte DIR_PIN_A = 3;
const byte DIR_PIN_B = 4;

// set values needed for moving average filter
int idxx = 0; //x-direction
float valx = 0;
float sumx = 0;
float readingsx[window_size];
float averagedx = 0;
int idxy = 0; //y-direction
float valy = 0;
float sumy = 0;
float readingsy[window_size];
float averagedy = 0;
int idxz = 0; //z-direction
float valz = 0;
float sumz = 0;
float readingsz[window_size];
float averagedz = 0;

// set values needed for running average
int i = 5; //index
bool didhigh = false;
bool didlow = false;
int didstep = 0;
unsigned long lasttimer = 0;
unsigned long timediff = millis();
float calibTime;
float calibAvgs = 0;
unsigned long timing[5]; //an array to hold 5 entries
unsigned long timeTotal = 0;
float rms;

void setup(void) {

  //-----------------------------------------------------------------------
  //  /*INITIALIZE MPU 6050*/
  // https://learn.adafruit.com/mpu6050-6-dof-accelerometer-and-gyro?view=all
  //-----------------------------------------------------------------------

  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);


  //-----------------------------------------------------------------------
  //  /*Set up pins as outputs for controlling the motor*/
  // ACUNA_erin-L5_S1
  //-----------------------------------------------------------------------
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(DIR_PIN_A, OUTPUT);
  pinMode(DIR_PIN_B, OUTPUT);

}


void loop() {

  //-----------------------------------------------------------------------
  //  /*GATHERING SENSOR DATA*/
  // https://learn.adafruit.com/mpu6050-6-dof-accelerometer-and-gyro?view=all
  //-----------------------------------------------------------------------

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
//  delay(50);

  //-----------------------------------------------------------------------
  //  /*MOVING AVERAGE FILTER*/
  // https://maker.pro/arduino/tutorial/how-to-clean-up-noisy-sensor-data-with-a-moving-average-filter
  //-----------------------------------------------------------------------
  
  //MAF for X-direction
  sumx = sumx - readingsx[idxx]; // remove the oldest entry from the sum
  valx = a.acceleration.x; // read the next sensor value
  readingsx[idxx] = valx; // add the newest reading to the window
  sumx = sumx + valx; // add the newest reading to the sum
  idxx = (idxx + 1) % window_size;// increment the index, and wrap to 0 if exceeding window size

  averagedx = sumx / window_size;    // Divide window sum by the window size
//  Serial.println("Val: " + String(valx) + ';' + " Avg: " + String(averagedx));
  
  //MAF for Y-direction
  sumy = sumy - readingsy[idxy];
  valy = a.acceleration.y;
  readingsy[idxy] = valy;
  sumy = sumy + valy;
  idxy = (idxy + 1) % window_size;

  averagedy = sumy / window_size;
//  Serial.println("Val: " + String(valy) + ';' + " Avg: " + String(averagedy));

  //MAF for Z-direction
  sumz = sumz - readingsz[idxz];
  valz = a.acceleration.z;
  readingsz[idxz] = valz; 
  sumz = sumz + valz;
  idxz = (idxz + 1) % window_size; 

  averagedz = sumz / window_size;
//  Serial.println("Val: " + String(valz) + ';' + " Avg: " + String(averagedz));

  //-----------------------------------------------------------------------
  //  /*CHECKING STEP*/
  //
  //-----------------------------------------------------------------------

  
  rms = sqrt((pow(averagedx,2) + pow(averagedy,2) + pow(averagedz,2))/3);
  Serial.println(String(millis()) + ';' +String(valx) + ';' + String(averagedx) + ';' + String(valy) + ';' + String(averagedy) + ';' + String(valz) + ';' + String(averagedz) + ';' + String(rms));

  // only set a high point if a low point occured before it
  if ((rms >= 6.5) && (didlow == true)) {
    didhigh = true;
//    Serial.println("Did High: " + String(rms));
  }

 // if acc falls below 5.5, count as a backswing
  if ((rms <= 5.5)) {
    didlow = true;
//    Serial.println("Did Low: " + String(rms));
  }

  // if forward swing and backward swing, check the time between steps
  if ((didhigh == true) && (didlow == true)) {
    didstep++;
    timediff = millis() - lasttimer;
    lasttimer = millis();
    didlow = false;
    didhigh = false;
    timing[i] = timediff;
    i++;
    timeTotal += timing[i];
//    Serial.println("last timer " + String(lasttimer));
  }
  
  //-----------------------------------------------------------------------
  //  /*RUNNING AVERAGE FOR TIME BETWEEN STEPS*/
  //
  //-----------------------------------------------------------------------
 
  if (i>=4){
    calibTime = timeTotal;
  float avgtime = timeTotal / 5; //find avg time
  
  // check if timing between steps fits range of tolerance for average
    for (int j = 0; j <= 4; j++) {
    if ((timing[j] < avgtime * (1.5)) && (timing[j] >= avgtime * (0.5))) {
      //keep datapoint
    }

    // toss out any outliers
    else {
      i--;
      calibTime -= timing[j];
    }
  }

  if (i = 0) {
    calibAvgs = avgtime;
  }
  else {
    calibAvgs = (calibTime) / i; //find the new average
  } 

//    Serial.println("CalibAvgs" + String(calibAvgs));
    i=0;
    timeTotal=0;
  }


  //-----------------------------------------------------------------------
  //  /*TOLERANCING FAN OPERATION*/
  // ACUNA_erin-L5_S1
  //-----------------------------------------------------------------------

  // Set motor direction forwards
  digitalWrite(DIR_PIN_A, HIGH);
  digitalWrite(DIR_PIN_B, LOW);

  if ((calibAvgs <= 500)) { //if person is fast (>3mph), turn fan on
    digitalWrite(ENABLE_PIN, HIGH);
//    Serial.println("Run");
  }
  
  else {//(calibAvgs > 15000) {//if person walking slow (<3mph), leave fan off
    analogWrite(ENABLE_PIN, LOW);
//    Serial.println("Stop");
  }
  
  if ((millis()-lasttimer)>1500) { // if person is stopped, turn fan on
    digitalWrite(ENABLE_PIN, HIGH);
//    Serial.println("Run");
  }

}
