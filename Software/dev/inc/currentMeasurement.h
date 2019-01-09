/*
 *
 *@brief Uses the ADC peripheral to read chassis motor current
 *
 *@author Alex Wong thwongaz@connect.ust.hk
 *
 */
#ifndef INC_CURRENTMEASUREMENT_H_
#define INC_CURRENTMEASUREMENT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define ADC_NUM_CHANNELS                    4
#define ADC_BUF_DEPTH                       16
#define CURRENT_MEASUREMENT_ADC_DRIVER      &ADCD2

//extern float s_currentOverSampled[ADC_NUM_CHANNELS];

//Init analog input pins, init ADC and start continuous conversion
void currentMeasurementInit(void);

#ifdef __cplusplus
}
#endif
#endif /* INC_CURRENTMEASUREMENT_H_ */
