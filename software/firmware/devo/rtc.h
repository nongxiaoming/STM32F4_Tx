#ifndef _RTC_H_
#define _RTC_H_

#if HAS_RTC
    #define NUM_RTC 2
#else
    #define NUM_RTC 0
#endif

#define RTC_STARTYEAR 2012

enum { DAY=0, MONTH, YEAR, SECOND, MINUTE, HOUR };

// initialize RTC clock
void RTC_Init(void);

// set date value (deviation epoch = seconds since 1.1.2012, 00:00:00)
void RTC_SetValue(uint32_t value);

// set time/date (don't change date/time)
void RTC_SetTime(uint16_t hour, uint16_t minute, uint16_t second);
void RTC_SetDate(uint16_t year, uint16_t month, uint16_t day);

// get date value (deviation epoch = seconds since 1.1.2012, 00:00:00)
uint32_t RTC_GetValue(void);

// get serial time (seconds since 01.01.2012, 0:00:00 - "deviation epoch")
uint32_t RTC_GetSerial(int year, int month, int day, int hour, int minute, int second);

// format time string
void RTC_GetTimeString(char *str, uint32_t value);
void RTC_GetTimeStringShort(char *str, uint32_t value);  // no seconds
void RTC_GetDateString(char *str, uint32_t date);        // without century
void RTC_GetDateStringLong(char *str, uint32_t date);    // with century

// return only time/date-value
uint32_t RTC_GetTimeValue(uint32_t time);
uint32_t RTC_GetDateValue(uint32_t time);

// to avoid sizeof() in rtc_config
int RTC_GetNumberTimeFormats(void);
int RTC_GetNumberDateFormats(void);

// return formatted time/date string as stated in tx config
void RTC_GetTimeFormatted(char *str, uint32_t time);
void RTC_GetDateFormatted(char *str, uint32_t date);
void RTC_GetMonthFormatted(char *str, unsigned month); // for written month names
void RTC_GetTimeFormattedBigbox(char *str, uint32_t time); // only this fits in big box
void RTC_GetDateFormattedBigbox(char *str, uint32_t date); // only this fits in big box
void RTC_GetDateFormattedOrder(unsigned index, uint8_t *left, uint8_t *middle, uint8_t *right); // for ordering the input fields

//return RTC name
const char *RTC_Name(char *str, int i);
#endif
