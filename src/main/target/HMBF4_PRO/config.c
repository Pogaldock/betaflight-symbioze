#include <string.h>

#include "platform.h"

#ifdef USE_TARGET_CONFIG

#include "config/feature.h"
#include "drivers/dshot.h"
#include "drivers/motor.h"
#include "drivers/osd.h"
#include "osd/osd.h"
#include "pg/adc.h"
#include "pg/motor.h"
#include "pg/rx.h"
#include "pg/rx_spi.h"
#include "pg/vcd.h"
#include "pg/vtx_table.h"
#include "rx/rx_spi.h"
#include "sensors/battery.h"
#include "io/vtx.h"

void targetConfiguration(void)
{
    featureConfigMutable()->enabledFeatures |= FEATURE_RX_SPI;
    featureConfigMutable()->enabledFeatures &= ~FEATURE_RX_SERIAL;
    rxSpiConfigMutable()->rx_spi_protocol = RX_SPI_EXPRESSLRS;

    osdConfigMutable()->displayPortDevice = OSD_DISPLAYPORT_DEVICE_MAX7456;
    osdElementConfigMutable()->item_pos[OSD_MAIN_BATT_VOLTAGE] = OSD_PROFILE_1_FLAG | OSD_POS(24, 10);
    osdElementConfigMutable()->item_pos[OSD_RSSI_VALUE] = OSD_PROFILE_1_FLAG | OSD_POS(1, 11);
    osdElementConfigMutable()->item_pos[OSD_ITEM_TIMER_2] = OSD_PROFILE_1_FLAG | OSD_POS(1, 10);
    osdElementConfigMutable()->item_pos[OSD_FLYMODE] = OSD_PROFILE_1_FLAG | OSD_POS(18, 10);
    osdElementConfigMutable()->item_pos[OSD_VTX_CHANNEL] = OSD_PROFILE_1_FLAG | OSD_POS(8, 10);
    osdElementConfigMutable()->item_pos[OSD_CURRENT_DRAW] = OSD_PROFILE_1_FLAG | OSD_POS(23, 11);
    osdElementConfigMutable()->item_pos[OSD_CRAFT_NAME] = OSD_PROFILE_1_FLAG | OSD_POS(8, 11);
    osdElementConfigMutable()->item_pos[OSD_WARNINGS] = OSD_PROFILE_1_FLAG | OSD_PROFILE_FLAG(2) | OSD_PROFILE_FLAG(3) | OSD_POS(9, 6);

    vcdProfileMutable()->video_system = VIDEO_SYSTEM_NTSC;
    vtxSettingsConfigMutable()->band = 4;
    vtxSettingsConfigMutable()->channel = 4;
    vtxSettingsConfigMutable()->power = 1;
    batteryConfigMutable()->vbatmincellvoltage = 330;
    batteryConfigMutable()->vbatwarningcellvoltage = 350;
    batteryConfigMutable()->vbatmaxcellvoltage = 440;
    motorConfigMutable()->digitalIdleOffsetValue = 1000;
    motorConfigMutable()->dev.useDshotTelemetry = DSHOT_TELEMETRY_ON;
    motorConfigMutable()->dev.motorPwmProtocol = PWM_TYPE_DSHOT300;
    motorConfigMutable()->motorPoleCount = 12;
}

#endif
