
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckDate.h"

_CORE_BEGIN

Date::Date(){
    time(&m_Timestamp);
}

Date::Date(long timestamp){
    m_Timestamp = (time_t)timestamp;
}

Date::~Date(){}

int Date::GetYear(){
    // http://www.cplusplus.com/reference/ctime/tm/
    tm local;
    localtime_s(&local, &m_Timestamp);
    return local.tm_year + 1900;
}

int Date::GetMonth(){
    tm local;
    localtime_s(&local, &m_Timestamp);
    return local.tm_mon + 1;
}

int Date::GetDay(){
    tm local;
    localtime_s(&local, &m_Timestamp);
    return local.tm_mday;
}

int Date::GetHours(){
    tm local;
    localtime_s(&local, &m_Timestamp);
    return local.tm_hour;
}

int Date::GetMinutes(){
    tm local;
    localtime_s(&local, &m_Timestamp);
    return local.tm_min;
}

int Date::GetSeconds(){
    tm local;
    localtime_s(&local, &m_Timestamp);
    return local.tm_sec;
}

long Date::GetLong(){
    return (long)m_Timestamp;
}

_CORE_END
