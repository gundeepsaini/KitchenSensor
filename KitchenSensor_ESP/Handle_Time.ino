
//--------------------------------------------------------------------------

// NTP Sync Interval is set to 60 mins internally


// This database is autogenerated from IANA timezone databas - https://www.iana.org/time-zones
#include <TZ.h>
#include <coredecls.h>    // settimeofday_cb()

// initial time (can also be given by an external RTC)
#define RTC_UTC_TEST 946684800 // 946684800 = Sat 01 Jan 2000 12:00:00 AM UTC

// Timezone definition - from TZ.h
#define MYTZ TZ_Europe_Berlin
//#define MYTZ TZ_Asia_Kolkata

//--------------------------------------------------------------------------

static timeval tv;
static timespec tp;
static time_t now;
extern "C" int clock_gettime(clockid_t unused, struct timespec *tp);

struct time_struct
{
  bool valid;
  bool DST;  
  int hr;
  int min;
  int sec;
  int day;
  int month;
  int year;
  int yearDay;
  int weekDay;
};
time_struct time_now;

//--------------------------------------------------------------------------



void Time_NTP_Config()
{
  // setup RTC time - it will be used until NTP server will send us real current time
  time_t rtc = RTC_UTC_TEST;
  timeval tv = { rtc, 0 };
  timezone tz = { 0, 0 };
  settimeofday(&tv, &tz);

  // install callback - called when settimeofday is called (by SNTP or us)
  // once enabled (by DHCP), SNTP is updated every hour
  settimeofday_cb(Time_NTP_Synced);
  configTime(MYTZ, "pool.ntp.org");
  
  Time_NTP_updateVar();
  if(!time_now.valid)
    Serial.printf("Time is currently not set!\n");
  else
    Time_NTP_Display();
}


void Time_NTP_Synced() 
{  
  Serial.println("NTP Time synchronised");  
  Time_NTP_Display();
}


void Time_NTP_Display()
{
  // Print time in human readable format
  time_t now1;  
  time(&now1);

  Serial.print("Time:     ");
  Serial.println(ctime(&now1));
}


void Time_NTP_updateVar()
{  
  time_t now2;
  struct tm * timeinfo2;
  
  time(&now2);
  timeinfo2 = localtime(&now2); 

  // Check definitions: http://www.cplusplus.com/reference/ctime/tm/
  time_now.DST    = int(timeinfo2->tm_isdst);
  time_now.hr     = int(timeinfo2->tm_hour);
  time_now.min    = int(timeinfo2->tm_min);
  time_now.sec    = int(timeinfo2->tm_sec);
  time_now.day    = int(timeinfo2->tm_mday);
  time_now.month  = int(timeinfo2->tm_mon)  + 1;
  time_now.year   = int(timeinfo2->tm_year) + 1900;
  time_now.yearDay = int(timeinfo2->tm_yday);
  time_now.weekDay = int(timeinfo2->tm_wday);

  if(time_now.year > 2000)
    time_now.valid = 1;
  else
    time_now.valid = 0;  

}


void Time_NTP_DisplayAdvanced()
{
  Time_NTP_updateVar();

  Serial.print("DST:"); 
  Serial.print(time_now.DST);
  
  Serial.print("  |  hh:mm:ss  "); 
  Serial.print(time_now.hr);
  Serial.print(":"); 
  Serial.print(time_now.min);
  Serial.print(":");
  Serial.print(time_now.sec);
  
  Serial.print("  |  dd.mm.yyyy  "); 
  Serial.print(time_now.day);
  Serial.print(".");
  Serial.print(time_now.month);
  Serial.print(".");
  Serial.println(time_now.year);
  
  Serial.print("Days since Sunday (0-6):"); 
  Serial.print(time_now.weekDay);
  Serial.print("  |  Days since January 1 (0-365):"); 
  Serial.println(time_now.yearDay);
}

