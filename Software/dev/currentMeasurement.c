/*
 *
 *@brief Uses the ADC peripheral to read individual chassis motor current
 *
 *@author Alex Wong     thwongaz@connect.ust.hk
 *
 */

#include <currentMeasurement.h>
#include "ch.h"
#include "hal.h"
#include "string.h"

//Stores raw adc sampled data for all channels
static adcsample_t s_currentSamples[ADC_BUF_DEPTH][ADC_NUM_CHANNELS];

//Stores oversampled adc data for increased ENOB
static float s_currentOverSampled[ADC_NUM_CHANNELS];

/*
 * @brief   ADC end of conversion callback for current measurement
 *          Performs averaging for oversampling
 * @param   adcp    Pointer to adc driver
 * @param   buffer  Pointer to adc sample buffer
 * @param   n       Size of sample buffer
 */
static void adcProcessSamplingEOC(ADCDriver *adcp, adcsample_t *buffer, size_t n)
{

    (void) adcp;
    (void) buffer;
    (void) n;
    static uint8_t s_sampleCount;
    static uint8_t s_channelCount;
    static uint32_t s_tempSum;

    for (s_channelCount = 0; s_channelCount < ADC_NUM_CHANNELS; s_channelCount++)
    {
        s_tempSum = 0;
        for (s_sampleCount = 0; s_sampleCount < ADC_BUF_DEPTH; s_sampleCount++)
        {
            s_tempSum += s_currentSamples[s_sampleCount][s_channelCount];
        }
        s_currentOverSampled[s_channelCount] = (float) s_tempSum / ADC_BUF_DEPTH;
    }

}

/*
 * @brief   ADC error callback for current measurement
 * @param   adcp    Pointer to adc driver
 * @param   error   Specifies error type
 */
static void adcProcessSamplingError(ADCDriver *adcp, adcerror_t err)
{

    (void) adcp;
    (void) err;

}

/*
 * ADCCLK = 42MHz
 * Conversion time (clk cycles) = Sampling time (clk cycles) + number of bits
 * Target sampling freq for each channel >= 10kHz
 * Oversample by 16 times for 2 extra bits of resolution
 * Raw sampling frequency >= 10000 * 4 * 16 = 640kHz
 * Sampling time <= 53 clk cycles
 */
static const ADCConversionGroup ADC_CONFIG =
{
    TRUE,                                       //enable circular buffer
    ADC_NUM_CHANNELS,                           //no. of channels
    adcProcessSamplingEOC,                      //end of conversion callback
    adcProcessSamplingError,                    //error callback
    0,                                          //CR1
    ADC_CR2_SWSTART,                            //CR2
    ADC_SMPR1_SMP_AN12(ADC_SAMPLE_28) |
    ADC_SMPR1_SMP_AN13(ADC_SAMPLE_28) |
    ADC_SMPR1_SMP_AN14(ADC_SAMPLE_28) |
    ADC_SMPR1_SMP_AN15(ADC_SAMPLE_28),          //SMPR1
    0,                                          //SMPR2
    ADC_SQR1_NUM_CH(ADC_NUM_CHANNELS),          //SQR1
    0,                                          //SQR2
    ADC_SQR3_SQ4_N(ADC_CHANNEL_IN15) |
    ADC_SQR3_SQ3_N(ADC_CHANNEL_IN14) |
    ADC_SQR3_SQ2_N(ADC_CHANNEL_IN13) |
    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN12)            //SQR3
};

/*
 * @brief Init analog input pins, init ADC and start continuous conversion
 */
void currentMeasurementInit(void)
{

    palSetLineMode(LINE_ADC_MOTOR1, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(LINE_ADC_MOTOR2, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(LINE_ADC_MOTOR3, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(LINE_ADC_MOTOR4, PAL_MODE_INPUT_ANALOG);

    memset(s_currentSamples, 0, sizeof(s_currentSamples));
    memset(s_currentOverSampled, 0, sizeof(s_currentOverSampled));

    adcStart(CURRENT_MEASUREMENT_ADC_DRIVER, NULL);

    adcStartConversion(CURRENT_MEASUREMENT_ADC_DRIVER, &ADC_CONFIG,
    (adcsample_t*) s_currentSamples, ADC_NUM_CHANNELS * ADC_BUF_DEPTH);

}

