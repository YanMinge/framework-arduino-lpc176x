#include "rtc.h"

int rtc_initialize (void)
{
	static int init = 0;

	// static int init;
	RTC_TIME_Type rtcTime;

	if (init) /* Already initialized */
		return 1;

	/* RTC Block section ------------------------------------------------------ */
	RTC_Init(LPC_RTC);

	/* Set current time for RTC */
	/* Current time is 8:00:00PM, 2019-08-08 */
	rtcTime.SEC     = 0;
	rtcTime.MIN     = 0;
	rtcTime.HOUR    = 20;
	rtcTime.DOM     = 8;
	rtcTime.DOW     = 4;
	rtcTime.MONTH   = 8;
	rtcTime.YEAR    = 2019;
	RTC_SetFullTime(LPC_RTC, &rtcTime);

	RTC_CntIncrIntConfig(LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);

	RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE | RTC_INT_ALARM);

	NVIC_EnableIRQ((IRQn_Type) RTC_IRQn);

	/* Enable rtc (starts increase the tick counter and second counter register) */
	RTC_Cmd(LPC_RTC, ENABLE);

	init = 1;

	return 1;
}

int rtc_gettime (RTC *rtc)
{
	RTC_TIME_Type rtcTime;

	RTC_GetFullTime(LPC_RTC, &rtcTime);

	rtc->sec = rtcTime.SEC;
	rtc->min = rtcTime.MIN;
	rtc->hour = rtcTime.HOUR;
	rtc->wday = rtcTime.DOW;
	rtc->mday = rtcTime.DOM;
	rtc->month = rtcTime.MONTH;
	rtc->year = rtcTime.YEAR;

	return 1;
}

int rtc_settime (const RTC *rtc)
{
	RTC_TIME_Type rtcTime;

	rtcTime.SEC     = rtc->sec;
	rtcTime.MIN     = rtc->min;
	rtcTime.HOUR    = rtc->hour;
	rtcTime.DOW     = rtc->wday;
	rtcTime.DOM     = rtc->mday;
	rtcTime.MONTH   = rtc->month;
	rtcTime.YEAR  = rtc->year;

	RTC_SetFullTime(LPC_RTC, &rtcTime);

	return 1;
}

/**
 * @brief User Provided Timer Function for FatFs module
 * @return  Nothing
 * @note  This is a real time clock service to be called from FatFs module.
 * Any valid time must be returned even if the system does not support a real time clock.
 * This is not required in read-only configuration.
 */
DWORD get_fattime(void)
{
	RTC rtc;

	/* Get local time */
	rtc_gettime(&rtc);

	/* Pack date and time into a DWORD variable */
	return ((DWORD) (rtc.year - 2000) << 25)
			 | ((DWORD) rtc.month << 21)
			 | ((DWORD) rtc.mday << 16)
			 | ((DWORD) rtc.hour << 11)
			 | ((DWORD) rtc.min << 5)
			 | ((DWORD) rtc.sec >> 1);
}
