#include <string.h>

#include "platform.h"

#ifdef USE_TARGET_CONFIG

#include "config/feature.h"
#include "config/config.h"
#include "common/maths.h"
#include "fc/rc_controls.h"
#include "fc/rc_modes.h"
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
#include "io/ledstrip.h"
#include "sensors/battery.h"
#include "io/vtx.h"

void targetConfiguration(void)
{
    featureConfigMutable()->enabledFeatures |= FEATURE_RX_SPI;
    featureConfigMutable()->enabledFeatures &= ~FEATURE_RX_SERIAL;
    rxSpiConfigMutable()->rx_spi_protocol = RX_SPI_EXPRESSLRS;

    modeActivationConditionsMutable(0)->modeId = BOXARM;
    modeActivationConditionsMutable(0)->auxChannelIndex = AUX1 - NON_AUX_CHANNEL_COUNT;
    modeActivationConditionsMutable(0)->range.startStep = CHANNEL_VALUE_TO_STEP(1700);
    modeActivationConditionsMutable(0)->range.endStep = CHANNEL_VALUE_TO_STEP(2100);
    modeActivationConditionsMutable(1)->modeId = BOXANGLE;
    modeActivationConditionsMutable(1)->auxChannelIndex = AUX2 - NON_AUX_CHANNEL_COUNT;
    modeActivationConditionsMutable(1)->range.startStep = CHANNEL_VALUE_TO_STEP(900);
    modeActivationConditionsMutable(1)->range.endStep = CHANNEL_VALUE_TO_STEP(1300);

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
    strcpy(pilotConfigMutable()->craftName, USBD_PRODUCT_STRING);
}

#endif
