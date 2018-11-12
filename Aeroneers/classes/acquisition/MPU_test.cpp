#include <MPU9250_Master_I2C.h>
#include <stdio.h>
#include <wiringPi.h>
#include <errmsg.h>

static const MPUIMU::Gscale_t GSCALE     = MPUIMU::GFS_250DPS;
static const MPUIMU::Ascale_t ASCALE     = MPUIMU::AFS_2G;
static const MPU9250::Mscale_t MSCALE    = MPU9250::MFS_16BITS;
static const MPU9250::Mmode_t MMODE      = MPU9250::M_100Hz;
static const uint8_t SAMPLE_RATE_DIVISOR = 0x04;

static MPU9250_Master_I2C imu(ASCALE, GSCALE, MSCALE, MMODE, SAMPLE_RATE_DIVISOR);

int main()
{
  // Setup WirinPi
  wiringPiSetup();

  // Start the MPU9250
  switch (imu.begin()) {

      case MPUIMU::ERROR_IMU_ID:
          errmsg("Bad IMU device ID");
          break;
      case MPUIMU::ERROR_MAG_ID:
          errmsg("Bad magnetometer device ID");
          break;
      case MPUIMU::ERROR_SELFTEST:
          //errmsg("Failed self-test");
          break;
      default:
          printf("MPU6050 online!\n");
          break;
  }

  while(true)
  {
      static float ax, ay, az, gx, gy, gz, mx, my, mz, temperature;

      // If INTERRUPT_PIN goes high, either all data registers have new data
      // or the accel wake on motion threshold has been crossed
      if(true) { //gotNewData) {   // On interrupt, read data

          //gotNewData = false;     // reset gotNewData flag

          if (imu.checkNewData())  { // data ready interrupt is detected

              imu.readAccelerometer(ax, ay, az);
              imu.readGyrometer(gx, gy, gz);
              imu.readMagnetometer(mx, my, mz);
              temperature = imu.readTemperature();
          }
      }

      // Report at 4 Hz
      static uint32_t msec_prev;
      uint32_t msec_curr = millis();

      if (msec_curr-msec_prev > 10) {

          msec_prev = msec_curr;

          printf("\n");

          printf("ax = %d  ay = %d  az = %d mg\n", (int)(1000*ax), (int)(1000*ay), (int)(1000*az));
          printf("gx = %+2.2f  gy = %+2.2f  gz = %+2.2f deg/s\n", gx, gy, gz);
          printf("mx = %d  my = %d  mz = %d mG\n", (int)mx, (int)my, (int)mz);

          // Print temperature in degrees Centigrade
          printf("Gyro temperature is %+1.1f degrees C\n", temperature);
      }
  }


}
