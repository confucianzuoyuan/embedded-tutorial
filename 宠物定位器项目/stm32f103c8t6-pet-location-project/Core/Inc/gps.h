//
// Created by zuoyuan on 2024/1/26.
//

#ifndef STM32F103C8T6_PET_LOCATION_PROJECT_GPS_H
#define STM32F103C8T6_PET_LOCATION_PROJECT_GPS_H

void gps_enable(void);
void gps_disable(void);
void gps_cold_start(void);
void gps_location(void);
void gps_location_info(void);

#endif //STM32F103C8T6_PET_LOCATION_PROJECT_GPS_H
