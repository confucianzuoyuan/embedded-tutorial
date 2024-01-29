//
// Created by zuoyuan on 2024/1/26.
//

#ifndef STM32F103C8T6_PET_LOCATION_PROJECT_EBYTE_CORE_H
#define STM32F103C8T6_PET_LOCATION_PROJECT_EBYTE_CORE_H

#include "ebyte_conf.h"

#if   defined(EBYTE_E22_400M22S) || defined(EBYTE_E22_900M22S)
#include "ebyte_e22x.h"

#elif defined(EBYTE_E220_400M22S) || defined(EBYTE_E220_900M22S)

#include "ebyte_e220x.h"

#elif defined(EBYTE_E10_400M20S) || defined(EBYTE_E10_900M20S)
#include "ebyte_e10x.h"

#elif defined(EBYTE_E07_900M10S)
#include "ebyte_e07x.h"

#elif defined(EBYTE_E49_400M20S)||defined(EBYTE_E49_900M20S)
#include "ebyte_e49x.h"

#else
#error No product selected !
#endif

typedef struct {
    void ( *Init )(void);

    void ( *Send )(uint8e_t *buffer, uint8e_t size, uint32e_t timeout);

    void ( *EnterSleepMode )(uint8e_t command);

    void ( *EnterReceiveMode )(uint32e_t timeout);

    void ( *StartPollTask)(void);

    void ( *InterruptTrigger)(void);

    uint32e_t ( *GetName )(void);

    uint8e_t ( *GetDriverVersion )(void);
} Ebyte_RF_t;

extern const Ebyte_RF_t Ebyte_RF;

#endif //STM32F103C8T6_PET_LOCATION_PROJECT_EBYTE_CORE_H
