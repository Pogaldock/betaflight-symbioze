/*
 * SYMBIOZE: baked board defaults for the BetaFPV Air65 (BETAFPVG473).
 *
 * A plain custom hex boots with an EMPTY config after a full-chip-erase
 * flash: no vtxtable -> the VTX doesn't transmit -> black goggles + ESC
 * no-signal beeps that look exactly like a boot failure (field-confirmed).
 * Vendor hexes avoid this by shipping the config inside the image; this is
 * our equivalent — the factory essentials (verified against a working
 * board's `diff all`) applied on every config reset, so the firmware boots
 * flying-ready with video, OSD, arm switch and tune in place.
 *
 * Compiled only when the CI passes -DUSE_TARGET_CONFIG -DSYMBIOZE_AIR65_DEFAULTS.
 */

#include <string.h>

#include "platform.h"

#ifdef USE_TARGET_CONFIG

#include "config/config.h"
#include "config/feature.h"

#include "common/axis.h"

#include "fc/rc_modes.h"

#include "drivers/dshot.h"
#include "drivers/motor.h"
#include "drivers/osd.h"

#include "flight/imu.h"
#include "flight/mixer.h"
#include "flight/pid.h"

#include "io/vtx.h"

#include "osd/osd.h"

#include "pg/dyn_notch.h"
#include "pg/motor.h"
#include "pg/vtx_table.h"

#include "sensors/battery.h"
#include "sensors/current.h"
#include "sensors/gyro.h"

void targetConfiguration(void)
{
#ifdef SYMBIOZE_AIR65_DEFAULTS
    // ---- OSD: on, and pinned to the analog MAX7456 (AUTO detection can miss it)
    featureConfigMutable()->enabledFeatures |= FEATURE_OSD;
    osdConfigMutable()->displayPortDevice = OSD_DISPLAYPORT_DEVICE_MAX7456;

    // ---- VTX table (BetaFPV factory, 6 bands x 8 channels, 5 power levels)
    vtxTableConfig_t *vtx = vtxTableConfigMutable();
    vtx->bands = 6;
    vtx->channels = 8;
    static const uint16_t freqs[6][8] = {
        { 5865, 5845, 5825, 5805, 5785, 5765, 5745, 5725 }, // BOSCAM_A
        { 5733, 5752, 5771, 5790, 5809, 5828, 5847, 5866 }, // BOSCAM_B
        { 5705, 5685, 5665, 5645, 5885, 5905, 5925, 5945 }, // BOSCAM_E
        { 5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880 }, // FATSHARK
        { 5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917 }, // RACEBAND
        { 5362, 5399, 5436, 5473, 5510, 5547, 5584, 5621 }, // LOWRACE
    };
    static const char *bandNames[6] = { "BOSCAM_A", "BOSCAM_B", "BOSCAM_E", "FATSHARK", "RACEBAND", "LOWRACE" };
    static const char bandLetters[6] = { 'A', 'B', 'E', 'F', 'R', 'L' };
    for (int b = 0; b < 6; b++) {
        for (int c = 0; c < 8; c++) {
            vtx->frequency[b][c] = freqs[b][c];
        }
        strncpy(vtx->bandNames[b], bandNames[b], VTX_TABLE_BAND_NAME_LENGTH);
        vtx->bandLetters[b] = bandLetters[b];
        vtx->isFactoryBand[b] = true;
    }
    vtx->powerLevels = 5;
    static const uint16_t powerValues[5] = { 1, 2, 3, 4, 0 };
    static const char *powerLabels[5] = { "25", "100", "200", "400", "PIT" };
    for (int i = 0; i < 5; i++) {
        vtx->powerValues[i] = powerValues[i];
        strncpy(vtx->powerLabels[i], powerLabels[i], VTX_TABLE_POWER_LABEL_LENGTH);
    }
    vtxSettingsConfigMutable()->band = 5;    // Raceband
    vtxSettingsConfigMutable()->channel = 8; // R8
    vtxSettingsConfigMutable()->power = 1;   // 25 mW — safe bench default
    vtxSettingsConfigMutable()->freq = 5917;

    // ---- switches (factory: ARM, ANGLE, HORIZON, BEEPER, FLIP OVER)
    modeActivationConditionsMutable(0)->modeId = BOXARM;
    modeActivationConditionsMutable(0)->auxChannelIndex = 0;
    modeActivationConditionsMutable(0)->range.startStep = CHANNEL_VALUE_TO_STEP(1700);
    modeActivationConditionsMutable(0)->range.endStep = CHANNEL_VALUE_TO_STEP(2100);
    modeActivationConditionsMutable(1)->modeId = BOXANGLE;
    modeActivationConditionsMutable(1)->auxChannelIndex = 1;
    modeActivationConditionsMutable(1)->range.startStep = CHANNEL_VALUE_TO_STEP(900);
    modeActivationConditionsMutable(1)->range.endStep = CHANNEL_VALUE_TO_STEP(1300);
    modeActivationConditionsMutable(2)->modeId = BOXHORIZON;
    modeActivationConditionsMutable(2)->auxChannelIndex = 1;
    modeActivationConditionsMutable(2)->range.startStep = CHANNEL_VALUE_TO_STEP(1300);
    modeActivationConditionsMutable(2)->range.endStep = CHANNEL_VALUE_TO_STEP(1700);
    modeActivationConditionsMutable(3)->modeId = BOXBEEPERON;
    modeActivationConditionsMutable(3)->auxChannelIndex = 3;
    modeActivationConditionsMutable(3)->range.startStep = CHANNEL_VALUE_TO_STEP(1700);
    modeActivationConditionsMutable(3)->range.endStep = CHANNEL_VALUE_TO_STEP(2100);
    modeActivationConditionsMutable(4)->modeId = BOXFLIPOVERAFTERCRASH;
    modeActivationConditionsMutable(4)->auxChannelIndex = 2;
    modeActivationConditionsMutable(4)->range.startStep = CHANNEL_VALUE_TO_STEP(1700);
    modeActivationConditionsMutable(4)->range.endStep = CHANNEL_VALUE_TO_STEP(2100);

    // ---- motors / mixer (0702SE whoop, DSHOT300 bidir for RPM filtering)
    motorConfigMutable()->dev.motorPwmProtocol = PWM_TYPE_DSHOT300;
    motorConfigMutable()->dev.useDshotTelemetry = DSHOT_TELEMETRY_ON;
    motorConfigMutable()->motorPoleCount = 12;
    mixerConfigMutable()->yaw_motors_reversed = true;

    // ---- battery / current
    batteryConfigMutable()->vbatmaxcellvoltage = 440;
    batteryConfigMutable()->vbatwarningcellvoltage = 345;
    currentSensorADCConfigMutable()->offset = -600;

    // ---- gyro filtering (factory whoop tune)
    gyroConfigMutable()->gyro_lpf1_static_hz = 0;
    gyroConfigMutable()->gyro_lpf1_dyn_min_hz = 0;
    dynNotchConfigMutable()->dyn_notch_count = 1;
    dynNotchConfigMutable()->dyn_notch_q = 500;
    dynNotchConfigMutable()->dyn_notch_min_hz = 120;
    dynNotchConfigMutable()->dyn_notch_max_hz = 500;

    imuConfigMutable()->small_angle = 180;
    pidConfigMutable()->pid_process_denom = 2;

    // ---- PID tune (factory profiles: "1219S" stock props, "HQ 31mm")
    pidProfile_t *p0 = pidProfilesMutable(0);
    strncpy(p0->profileName, "1219S", MAX_PROFILE_NAME_LENGTH);
    p0->pid[PID_PITCH].P = 71; p0->pid[PID_PITCH].I = 101; p0->pid[PID_PITCH].D = 60; p0->pid[PID_PITCH].F = 0;
    p0->pid[PID_ROLL].P = 61;  p0->pid[PID_ROLL].I = 87;   p0->pid[PID_ROLL].D = 44;  p0->pid[PID_ROLL].F = 0;
    p0->pid[PID_YAW].P = 61;   p0->pid[PID_YAW].I = 87;    p0->pid[PID_YAW].D = 0;    p0->pid[PID_YAW].F = 0;
    p0->d_min[FD_ROLL] = 41;
    p0->d_min[FD_PITCH] = 56;
    p0->d_min_gain = 20; // the CLI's `d_max_gain` — struct kept the legacy name
    p0->tpa_rate = 60;

    pidProfile_t *p1 = pidProfilesMutable(1);
    strncpy(p1->profileName, "HQ 31mm", MAX_PROFILE_NAME_LENGTH);
    p1->pid[PID_PITCH].P = 67; p1->pid[PID_PITCH].I = 121; p1->pid[PID_PITCH].D = 51; p1->pid[PID_PITCH].F = 0;
    p1->pid[PID_ROLL].P = 61;  p1->pid[PID_ROLL].I = 110;  p1->pid[PID_ROLL].D = 41;  p1->pid[PID_ROLL].F = 0;
    p1->pid[PID_YAW].P = 61;   p1->pid[PID_YAW].I = 110;   p1->pid[PID_YAW].D = 0;    p1->pid[PID_YAW].F = 0;
    p1->d_min[FD_ROLL] = 41;
    p1->d_min[FD_PITCH] = 51;
    p1->tpa_rate = 60;

    strcpy(pilotConfigMutable()->craftName, "AIR65 R");
#endif // SYMBIOZE_AIR65_DEFAULTS
}

#endif // USE_TARGET_CONFIG
