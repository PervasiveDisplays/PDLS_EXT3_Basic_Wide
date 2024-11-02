//
// Screen_EPD_EXT3.cpp
// Library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 28 Jun 2016
//
// Copyright (c) Rei Vilo, 2010-2024
// Licence Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
// For exclusive use with Pervasive Displays screens
// Portions (c) Pervasive Displays, 2010-2024
//
// Release 509: Added support for 271_PS_09
// Release 527: Added support for ESP32 PSRAM
// Release 541: Improved support for ESP32
// Release 550: Tested Xiao ESP32-C3 with SPI exception
// Release 601: Added support for screens with embedded fast update
// Release 602: Improved functions structure
// Release 604: Improved stability
// Release 607: Improved screens names consistency
// Release 608: Added screen report
// Release 609: Added temperature management
// Release 610: Removed partial update
// Release 700: Refactored screen and board functions
// Release 701: Improved functions names consistency
// Release 701: Added support for 290_KS_0F
// Release 702: Added support for 206_KS_0E
// Release 703: Added support for 152_KS_0J
// Release 800: Read OTP memory
// Release 801: Improved OTP implementation
// Release 802: Added support for 343_KS_0B
// Release 802: Added references to application notes
// Release 802: Refactored CoG functions
// Release 803: Added types for string and frame-buffer
// Release 804: Improved power management
// Release 805: Improved stability
// Release 806: New library for Wide temperature only
// Release 808: Improved stability
//

// Library header
#include "Screen_EPD_EXT3.h"

//
// === COG section
//
/// @cond
/// @see
/// * ApplicationNote_Small_Size_wide-Temperature_EPD_v03_20231031_B
/// * ApplicationNote_Small_Size_wide-Temperature_EPD_v01_20231225_A
/// * ApplicationNote_152_Size_wide-Temperature_EPD_v01_20231225_A
/// * ApplicationNote_DualChip_wideTemperature_EPD_v01_20230804
/// * ApplicationNote_SingleChip_wideTemperature_EPD_v01_20230720
//

//
// --- Large screens with K film
//

//
// --- End of Large screens with film
//

//
// --- Medium screens with K film
//
void Screen_EPD_EXT3_Fast::COG_MediumK_reset()
{
    // Application note § 2. Power on COG driver
    b_reset(5, 2, 4, 20, 5); // Medium
}

typedef struct
{
    uint8_t FORMAT_REPEAT;
    uint8_t PHLINI_BSTSWa;
    uint8_t PHHINI_BSTSWb;
    uint8_t PHLVAR_DELAYa;
    uint8_t PHHVAR_DELAYb;
    uint8_t BSTSWa_RESERVE;
    uint8_t BSTSWb_RESERVE;
    uint8_t DELAY_RESERVE;
} COG_SOFT_START_DEF;

typedef struct
{
    uint8_t LAYOUTREV;
    uint8_t COG_TYPE;
    uint8_t VENDOR;
    uint8_t WAVEFORMREV;
    uint8_t FPLLOT[6];
    uint8_t COLOR;

    uint8_t TCON;
    uint8_t DRFW[4];
    uint8_t DCTL;
    uint8_t VCOM;
    uint8_t VCOM_CTRL;
    uint8_t OSC_2nd;
    uint8_t RAM_RW[3];
    uint8_t DUW[6];
    uint8_t STV_DIR;
    uint8_t MS_SYNC;
    uint8_t BVSS;
    COG_SOFT_START_DEF SS[4];
} COG_OTP_USER_DATA;

COG_OTP_USER_DATA COG_userData;

void Screen_EPD_EXT3_Fast::COG_MediumK_getDataOTP()
{
    // uint8_t i = sNG;

    // const uint8_t COG_initialData581[128] =
    // {
    //     0x16, 0x95, 0x01, 0x01, 0x56, 0x43, 0x52, 0x31,
    //     0x32, 0x31, 0x00, 0x25, 0x00, 0x1f, 0x14, 0xc7,
    //     0x00, 0x00, 0x1f, 0x50, 0x04, 0x1f, 0x00, 0x50,
    //     0x00, 0x1f, 0x03, 0x02, 0x00, 0x02, 0xfe, 0xc3,
    //     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    //     0x84, 0x50, 0x00, 0x00, 0x01, 0x1f, 0x9f, 0x81,
    //     0x8a, 0x0a, 0x00, 0x00, 0x01, 0x1f, 0x9f, 0x81,
    //     0x8c, 0x0a, 0x02, 0x00, 0x01, 0x7f, 0xff, 0x83,
    //     0x86, 0x0a, 0x0e, 0xff, 0x00, 0x7f, 0xff, 0x81,
    //     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    //     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    //     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    //     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    //     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    //     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    //     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    // };
    //
    // const uint8_t * COG_initialData;
    //
    // COG_initialData = COG_initialData581;
    //

    // uint8_t i = sNG;

    // Read OTP
    uint8_t ui8 = 0;
    uint16_t _readBytes = 128;
    u_flagOTP = false;
    uint8_t COG_initialData[128] = {0};

    COG_MediumK_reset();

    digitalWrite(b_pin.panelDC, LOW); // Command
    digitalWrite(b_pin.panelCS, LOW); // Select
    hV_HAL_SPI3_write(0xb9);
    delay(5);

    digitalWrite(b_pin.panelDC, HIGH); // Data
    ui8 = hV_HAL_SPI3_read(); // Dummy
    // hV_HAL_log(LEVEL_DEBUG, "Dummy read 0x%02x", ui8);

    // Populate COG_data
    for (uint16_t index = 0; index < _readBytes; index += 1)
    {
        COG_initialData[index] = hV_HAL_SPI3_read(); // Read OTP
    }

    // End of OTP reading
    digitalWrite(b_pin.panelCS, HIGH); // Unselect

    // Check
    uint8_t _chipId = 0x16;
    if (COG_initialData[0] == _chipId)
    {
        COG_userData.LAYOUTREV = COG_initialData[0x00];
        COG_userData.COG_TYPE = COG_initialData[0x01];
        COG_userData.VENDOR = COG_initialData[0x02];
        COG_userData.WAVEFORMREV = COG_initialData[0x03];
        for (uint8_t i = 0; i < 6; i++)
        {
            COG_userData.FPLLOT[i] = COG_initialData[0x04 + i];
        }
        COG_userData.COLOR = COG_initialData[0x0a];

        COG_userData.TCON = COG_initialData[0x0b];
        for (uint8_t i = 0; i < 4; i++)
        {
            COG_userData.DRFW[i] = COG_initialData[0x0c + i];
        }

        COG_userData.DCTL = COG_initialData[0x10];

        COG_userData.VCOM = COG_initialData[0x11];
        COG_userData.VCOM_CTRL = COG_initialData[0x1f];
        for (uint8_t i = 0; i < 3; i++)
        {
            COG_userData.RAM_RW[i] = COG_initialData[0x12 + i];
        }
        for (uint8_t i = 0; i < 6; i++)
        {
            COG_userData.DUW[i] = COG_initialData[0x15 + i];
        }
        COG_userData.STV_DIR = COG_initialData[0x1b];
        COG_userData.MS_SYNC = COG_initialData[0x1c];
        COG_userData.BVSS = COG_initialData[0x1d];
        for (uint8_t i = 0; i < 4; i++)
        {
            COG_userData.SS[i].FORMAT_REPEAT = COG_initialData[0x28 + (8 * i)];
            COG_userData.SS[i].PHLINI_BSTSWa = COG_initialData[0x28 + (8 * i) + 1];
            COG_userData.SS[i].PHHINI_BSTSWb = COG_initialData[0x28 + (8 * i) + 2];
            COG_userData.SS[i].PHLVAR_DELAYa = COG_initialData[0x28 + (8 * i) + 3];
            COG_userData.SS[i].PHHVAR_DELAYb = COG_initialData[0x28 + (8 * i) + 4];
            COG_userData.SS[i].BSTSWa_RESERVE = COG_initialData[0x28 + (8 * i) + 5];
            COG_userData.SS[i].BSTSWb_RESERVE = COG_initialData[0x28 + (8 * i) + 6];
            COG_userData.SS[i].DELAY_RESERVE = COG_initialData[0x28 + (8 * i) + 7];
        }
        // i = sOK;
        // u_flagOTP = true;
    }
}

void Screen_EPD_EXT3_Fast::COG_MediumK_initial(uint8_t updateMode)
{
    uint8_t workDCTL[2];
    workDCTL[0] = COG_userData.DCTL; // DCTL
    workDCTL[1] = 0x00;

    // FILM_K already checked
    b_sendIndexData(0x01, workDCTL, 1); // Wide
}

void Screen_EPD_EXT3_Fast::COG_MediumK_sendImageData(uint8_t updateMode)
{
    // Application note § 3.2 Input image to the EPD
    FRAMEBUFFER_TYPE nextBuffer = s_newImage;
    FRAMEBUFFER_TYPE previousBuffer = s_newImage + u_pageColourSize;

    uint8_t iDCTL[2];
    iDCTL[0] = COG_userData.DCTL;
    iDCTL[1] = 0x00;
    b_sendIndexData(0x01, &iDCTL[0], 2); //DCTL 0x10 of MTP

    // Send image data

    b_sendIndexData(0x13, &COG_userData.DUW[0], 6);
    b_sendIndexData(0x90, &COG_userData.DRFW[0], 4);

    b_sendIndexData(0x12, &COG_userData.RAM_RW[0], 3);
    // send first frame
    b_sendIndexData(0x10, nextBuffer, u_pageColourSize); // First frame

    b_sendIndexData(0x12, &COG_userData.RAM_RW[0], 3);
    // send second frame
    b_sendIndexData(0x11, previousBuffer, u_pageColourSize); // Second frame

    // Copy next frame to previous frame
    memcpy(previousBuffer, nextBuffer, u_pageColourSize); // Copy displayed next to previous
}

void Screen_EPD_EXT3_Fast::COG_MediumK_update(uint8_t updateMode)
{
    // COG_MediumK_initial();
    // // _reset(5, 2, 4, 20, 5);
    // COG_MediumK_getDataOTP(); // get user data from initial data

    uint8_t data4[] = {0x7d};
    b_sendIndexData(0x05, data4, 1);
    delay(50);
    uint8_t data5[] = {0x00};
    b_sendIndexData(0x05, data5, 1);
    delay(1);
    b_sendIndexData(0xD8, &COG_userData.MS_SYNC, 1);
    b_sendIndexData(0xD6, &COG_userData.BVSS, 1);
    uint8_t data9[] = {0x10};
    b_sendIndexData(0xa7, data9, 1);
    delay(2);
    b_sendIndexData(0xa7, data5, 1);
    delay(10);

    // Temperature sequence
    b_sendIndexData(0x44, data5, 1);
    uint8_t data11[] = {0x80};
    b_sendIndexData(0x45, data11, 1);
    b_sendIndexData(0xa7, data9, 1);
    delay(2);
    b_sendIndexData(0xa7, data5, 1);
    delay(10);
    uint8_t data12[] = {0x06};
    b_sendIndexData(0x44, data12, 1);

    uint8_t data13[] = {0xc1};
    b_sendIndexData(0x45, data13, 1); //>> IF Fast, (0x45, FU_IDX)

    b_sendIndexData(0xa7, data9, 1);
    delay(2);
    b_sendIndexData(0xa7, data5, 1);
    delay(10);
    b_sendIndexData(0x60, &COG_userData.TCON, 1);
    b_sendIndexData(0x61, &COG_userData.STV_DIR, 1);
    b_sendIndexData(0x02, &COG_userData.VCOM, 1);
    b_sendIndexData(0x03, &COG_userData.VCOM_CTRL, 1);

    // _DCDC_softStart();
    uint8_t iPH[2], iREP;
    for (uint8_t value = 0; value < 4; value++)
    {
        iPH[0] = COG_userData.SS[value].PHLINI_BSTSWa;
        iPH[1] = COG_userData.SS[value].PHHINI_BSTSWb;
        if (COG_userData.SS[value].FORMAT_REPEAT & 0x80) // Format 1
        {
            for (iREP = 0; iREP < (COG_userData.SS[value].FORMAT_REPEAT & 0x7f); iREP++)
            {
                b_sendIndexData(0x09, &COG_userData.SS[value].BSTSWa_RESERVE, 1);
                iPH[0] += COG_userData.SS[value].PHLVAR_DELAYa;
                iPH[1] += COG_userData.SS[value].PHHVAR_DELAYb;
                b_sendIndexData(0x51, iPH, 2);
                b_sendIndexData(0x09, &COG_userData.SS[value].BSTSWb_RESERVE, 1);
                if (COG_userData.SS[value].DELAY_RESERVE & 0x80)
                {
                    delay(COG_userData.SS[value].DELAY_RESERVE & 0x7f); // ms
                }
                // else;
                delayMicroseconds(10 * COG_userData.SS[value].DELAY_RESERVE & 0x7f); // 10us
            }
        }
        else // Format 2
        {
            for (iREP = 0; iREP < (COG_userData.SS[value].FORMAT_REPEAT & 0x7f); iREP++)
            {
                b_sendIndexData(0x09, &COG_userData.SS[value].PHLINI_BSTSWa, 1);
                if (COG_userData.SS[value].PHLVAR_DELAYa & 0x80)
                {
                    delay(COG_userData.SS[value].PHLVAR_DELAYa & 0x7f); // ms
                }
                // else;
                delayMicroseconds(10 * COG_userData.SS[value].PHLVAR_DELAYa & 0x7f); // 10us delaySysTick_10us

                b_sendIndexData(0x09, &COG_userData.SS[value].PHHINI_BSTSWb, 1);
                if (COG_userData.SS[value].PHHVAR_DELAYb & 0x80)
                {
                    delay(COG_userData.SS[value].PHHVAR_DELAYb & 0x7f); // ms
                }
                // else;
                delayMicroseconds(10 * COG_userData.SS[value].PHHVAR_DELAYb & 0x7f); // 10us delaySysTick_10us
            }
        }
    }

    // _displayRefresh();
    uint8_t data18[] = {0x3c};
    b_sendIndexData(0x15, data18, 1); //Display Refresh
    delay(5);

    // _DCDC_softShutdown();
    // See Screen_EPD_EXT3_Fast::COG_MediumK_powerOff
}

void Screen_EPD_EXT3_Fast::COG_MediumK_powerOff()
{
    // Application note § 5. Turn-off DC/DC

    // DC-DC off
    b_waitBusy();

    // // FILM_K already checked
    // b_sendCommandData8(0x09, 0x7f);
    // b_sendCommandData8(0x05, 0x3d);
    // b_sendCommandData8(0x09, 0x7e);
    // delay(60);
    // b_sendCommandData8(0x09, 0x00);
    uint8_t data19[] = {0x7f};
    b_sendIndexData(0x09, data19, 1);
    uint8_t data20[] = {0x3d};
    b_sendIndexData(0x05, data20, 1);
    uint8_t data555[] = {0x7e};
    b_sendIndexData(0x09, data555, 1);
    delay(60);
    uint8_t data55[] = {0x00};
    b_sendIndexData(0x09, data55, 1);

    // b_waitBusy(HIGH); // added
/*
    digitalWrite(b_pin.panelDC, LOW);
    digitalWrite(b_pin.panelCS, LOW);
    digitalWrite(b_pin.panelReset, LOW);
    // digitalWrite(panelON_PIN, LOW); // PANEL_OFF# = 0
*/
    digitalWrite(b_pin.panelCS, HIGH); // CS# = 1
}
//
// --- End of Medium screens with K film
//

//
// --- Small screens with K film
//
void Screen_EPD_EXT3_Fast::COG_SmallK_reset()
{
    // Application note § 2. Power on COG driver
    b_reset(5, 5, 10, 5, 5); // Small

    // Check after reset
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_150_KS_0J:
        case eScreen_EPD_152_KS_0J:

            if (digitalRead(b_pin.panelBusy) == HIGH)
            {
                mySerial.println();
                mySerial.println("hV * Incorrect type for 1.52-Wide");
                while (0x01);
            }
            break;

        default:

            break;
    }
}

void Screen_EPD_EXT3_Fast::COG_SmallK_getDataOTP()
{
    // Read OTP
    uint8_t ui8 = 0;
    uint16_t _readBytes = 0;
    u_flagOTP = false;

    // Application note § 3. Read OTP memory
    // Register 0x50 flag
    // Additional settings for fast update, 154 206 213 266 271A 370 and 437 screens (s_flag50)
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_154_PS_0C:
        case eScreen_EPD_154_KS_0C:
        case eScreen_EPD_206_KS_0E:
        case eScreen_EPD_213_PS_0E:
        case eScreen_EPD_213_KS_0E:
        case eScreen_EPD_266_PS_0C:
        case eScreen_EPD_266_KS_0C:
        case eScreen_EPD_271_KS_0C: // 2.71(A)
        case eScreen_EPD_370_PS_0C:
        case eScreen_EPD_370_PS_0C_Touch:
        case eScreen_EPD_370_KS_0C:
        case eScreen_EPD_437_PS_0C:
        case eScreen_EPD_437_KS_0C:

            s_flag50 = true;
            break;

        default:

            s_flag50 = false;
            break;
    }

    // Screens with no OTP
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_150_KS_0J:
        case eScreen_EPD_152_KS_0J:
        case eScreen_EPD_290_KS_0F:

            u_flagOTP = true;
            mySerial.println("hV . OTP check passed - embedded PSR");
            return; // No PSR
            break;

        default:

            break;
    }

    // GPIO
    // COG_SmallK_reset(); // Although not mentioned, reset to ensure stable state

    // Read OTP
    _readBytes = 2;
    ui8 = 0;

    uint16_t offsetA5 = 0x0000;
    uint16_t offsetPSR = 0x0000;

    digitalWrite(b_pin.panelDC, LOW); // Command
    digitalWrite(b_pin.panelCS, LOW); // CS low = Select
    hV_HAL_SPI3_write(0xa2);
    digitalWrite(b_pin.panelCS, HIGH); // CS high = Unselect
    delay(10);

    digitalWrite(b_pin.panelDC, HIGH); // Data
    digitalWrite(b_pin.panelCS, LOW); // CS low = Select
    ui8 = hV_HAL_SPI3_read(); // Dummy
    digitalWrite(b_pin.panelCS, HIGH); // CS high = Unselect
    // mySerial.println(formatString("hV . Dummy read 0x%02x", ui8));

    digitalWrite(b_pin.panelCS, LOW); // CS low = Select
    ui8 = hV_HAL_SPI3_read(); // First byte to be checked
    digitalWrite(b_pin.panelCS, HIGH); // CS high = Unselect
    // mySerial.println(formatString("hV . ui 0x%02x", ui8));

    // Check bank
    uint8_t bank = ((ui8 == 0xa5) ? 0 : 1);

    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_271_KS_09:
        case eScreen_EPD_271_KS_09_Touch:

            offsetPSR = 0x004b;
            offsetA5 = 0x0000;

            if (bank > 0)
            {
                COG_data[0] = 0xcf;
                COG_data[1] = 0x82;
                return;
            }
            break;

        case eScreen_EPD_271_PS_09:
        // case eScreen_EPD_271_KS_09_Touch:
        case eScreen_EPD_271_PS_09_Touch:
        case eScreen_EPD_287_PS_09:

            offsetPSR = 0x004b;
            offsetA5 = 0x0000;

            break;

        case eScreen_EPD_154_KS_0C:
        case eScreen_EPD_154_PS_0C:
        case eScreen_EPD_266_KS_0C:
        case eScreen_EPD_266_PS_0C:
        case eScreen_EPD_271_KS_0C: // 2.71(A)
        case eScreen_EPD_350_KS_0C:
        case eScreen_EPD_370_KS_0C:
        case eScreen_EPD_370_PS_0C:
        case eScreen_EPD_370_PS_0C_Touch:
        case eScreen_EPD_437_PS_0C:

            offsetPSR = (bank == 0) ? 0x0fb4 : 0x1fb4;
            offsetA5 = (bank == 0) ? 0x0000 : 0x1000;

            break;

        case eScreen_EPD_206_KS_0E:
        case eScreen_EPD_213_KS_0E:
        case eScreen_EPD_213_PS_0E:

            offsetPSR = (bank == 0) ? 0x0b1b : 0x171b;
            offsetA5 = (bank == 0) ? 0x0000 : 0x0c00;
            break;

        case eScreen_EPD_417_PS_0D:
        case eScreen_EPD_417_KS_0D:

            offsetPSR = (bank == 0) ? 0x0b1f : 0x171f;
            offsetA5 = (bank == 0) ? 0x0000 : 0x0c00;
            break;

        default:

            mySerial.println(formatString("hV * OTP check failed - Screen %i-%cS-0%c not supported", u_codeSize, u_codeFilm, u_codeDriver));
            mySerial.flush();
            while (true);
            break;
    }

    // Check second bank
    if (offsetA5 > 0x0000)
    {
        for (uint16_t index = 1; index < offsetA5; index += 1)
        {
            digitalWrite(b_pin.panelCS, LOW); // CS low = Select
            ui8 = hV_HAL_SPI3_read();
            digitalWrite(b_pin.panelCS, HIGH); // CS high = Unselect
        }

        digitalWrite(b_pin.panelCS, LOW); // CS low = Select
        ui8 = hV_HAL_SPI3_read(); // First byte to be checked
        digitalWrite(b_pin.panelCS, HIGH); // CS high = Unselect

        if (ui8 != 0xa5)
        {
            mySerial.println();
            mySerial.println(formatString("hV * OTP check failed - Bank %i, first 0x%02x, expected 0x%02x", bank, ui8, 0xa5));
            mySerial.flush();
            while (0x01);
        }
    }

    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_271_KS_09:
        case eScreen_EPD_271_PS_09:
        case eScreen_EPD_271_PS_09_Touch:
        // case eScreen_EPD_287_KS_09:
        case eScreen_EPD_287_PS_09:

            mySerial.println(formatString("hV . OTP check passed - Bank %i, first 0x%02x %s", bank, ui8, (bank == 0) ? "as expected" : "not checked"));
            break;

        default:

            mySerial.println(formatString("hV . OTP check passed - Bank %i, first 0x%02x as expected", bank, ui8));
            break;
    }

    // Ignore bytes 1..offsetPSR
    for (uint16_t index = offsetA5 + 1; index < offsetPSR; index += 1)
    {
        digitalWrite(b_pin.panelCS, LOW); // CS low = Select
        ui8 = hV_HAL_SPI3_read();
        digitalWrite(b_pin.panelCS, HIGH); // CS high = Unselect
    }

    // Populate COG_data
    for (uint16_t index = 0; index < _readBytes; index += 1)
    {
        digitalWrite(b_pin.panelCS, LOW); // CS low = Select
        ui8 = hV_HAL_SPI3_read(); // Read OTP
        COG_data[index] = ui8;
        digitalWrite(b_pin.panelCS, HIGH); // CS high = Unselect
    }

    u_flagOTP = true;
}

void Screen_EPD_EXT3_Fast::COG_SmallK_initial(uint8_t updateMode)
{
    // Application note § 4. Input initial command
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_150_KS_0J:
        case eScreen_EPD_152_KS_0J:

            // Soft reset
            b_sendCommand8(0x12);
            digitalWrite(b_pin.panelDC, LOW);
            b_waitBusy(LOW); // 150 and 152 specific

            // Work settings
            b_sendCommandData8(0x1a, u_temperature);

            if (updateMode == UPDATE_GLOBAL)
            {
                b_sendCommandData8(0x22, 0xd7);
            }
            else if (updateMode == UPDATE_FAST)
            {
                b_sendCommandData8(0x3c, 0xc0);
                b_sendCommandData8(0x22, 0xdf);
            }
            break;

        default:

            // Work settings
            uint8_t indexTemperature; // Temperature
            uint8_t index00_work[2]; // PSR

            // FILM_K already checked
            if (updateMode != UPDATE_GLOBAL) // Specific settings for fast update
            {
                indexTemperature = u_temperature | 0x40; // temperature | 0x40
                index00_work[0] = COG_data[0] | 0x10; // PSR0 | 0x10
                index00_work[1] = COG_data[1] | 0x02; // PSR1 | 0x02
            }
            else // Common settings
            {
                indexTemperature = u_temperature; // Temperature
                index00_work[0] = COG_data[0]; // PSR0
                index00_work[1] = COG_data[1]; // PSR1
            } // u_codeExtra updateMode

            // New algorithm
            b_sendCommandData8(0x00, 0x0e); // Soft-reset
            b_waitBusy();

            b_sendCommandData8(0xe5, indexTemperature); // Input Temperature
            b_sendCommandData8(0xe0, 0x02); // Activate Temperature

            if (u_codeSize == SIZE_290) // No PSR
            {
                b_sendCommandData8(0x4d, 0x55);
                b_sendCommandData8(0xe9, 0x02);
            }
            else
            {
                b_sendIndexData(0x00, index00_work, 2); // PSR
            }

            // Specific settings for fast update, all screens
            // FILM_K already checked
            if (updateMode != UPDATE_GLOBAL)
            {
                b_sendCommandData8(0x50, 0x07); // Vcom and data interval setting
            }
            break;
    }
}

void Screen_EPD_EXT3_Fast::COG_SmallK_sendImageData(uint8_t updateMode)
{
    // Application note § 5. Input image to the EPD
    FRAMEBUFFER_TYPE nextBuffer = s_newImage;
    FRAMEBUFFER_TYPE previousBuffer = s_newImage + u_pageColourSize;

    // Send image data
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_150_KS_0J:
        case eScreen_EPD_152_KS_0J:

            b_sendIndexData(0x24, previousBuffer, u_pageColourSize); // Previous frame, blackBuffer
            b_sendIndexData(0x26, nextBuffer, u_pageColourSize); // Next frame
            break;

        default:

            // Additional settings for fast update, 154 213 266 370 and 437 screens (s_flag50)
            if (s_flag50)
            {
                b_sendCommandData8(0x50, 0x27); // Vcom and data interval setting
            }

            b_sendIndexData(0x10, previousBuffer, u_pageColourSize); // Previous frame, blackBuffer
            b_sendIndexData(0x13, nextBuffer, u_pageColourSize); // Next frame

            // Additional settings for fast update, 154 213 266 370 and 437 screens (s_flag50)
            if (s_flag50)
            {
                b_sendCommandData8(0x50, 0x07); // Vcom and data interval setting
            }
            break;
    } // u_eScreen_EPD

    // Copy next frame to previous frame
    memcpy(previousBuffer, nextBuffer, u_pageColourSize); // Copy displayed next to previous
}

void Screen_EPD_EXT3_Fast::COG_SmallK_update(uint8_t updateMode)
{
    // Application note § 6. Send updating command
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_150_KS_0J:
        case eScreen_EPD_152_KS_0J:

            b_waitBusy(LOW); // 152 specific
            b_sendCommand8(0x20); // Display Refresh
            digitalWrite(b_pin.panelCS, HIGH); // CS# = 1
            b_waitBusy(LOW); // 152 specific
            break;

        default:

            b_waitBusy();

            b_sendCommand8(0x04); // Power on
            b_waitBusy();

            b_sendCommand8(0x12); // Display Refresh
            b_waitBusy();
            break;
    }
}

void Screen_EPD_EXT3_Fast::COG_SmallK_powerOff()
{
    // Application note § 7. Turn-off DC/DC
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_150_KS_0J:
        case eScreen_EPD_152_KS_0J:

            break;

        default:

            b_sendCommand8(0x02); // Turn off DC/DC
            b_waitBusy();
            break;
    }
}
//
// --- End of Small screens with K film
//
/// @endcond
//
// === End of COG section
//

//
// === Class section
//
Screen_EPD_EXT3_Fast::Screen_EPD_EXT3_Fast(eScreen_EPD_t eScreen_EPD_EXT3, pins_t board)
{
    u_eScreen_EPD = eScreen_EPD_EXT3;
    b_pin = board;
    s_newImage = 0; // nullptr
    COG_data[0] = 0;
}

void Screen_EPD_EXT3_Fast::begin()
{
    // u_eScreen_EPD = eScreen_EPD_EXT3;
    u_codeSize = SCREEN_SIZE(u_eScreen_EPD);
    u_codeFilm = SCREEN_FILM(u_eScreen_EPD);
    u_codeDriver = SCREEN_DRIVER(u_eScreen_EPD);
    u_codeExtra = SCREEN_EXTRA(u_eScreen_EPD);
    v_screenColourBits = 2; // BWR and BWRY

    // Checks
    switch (u_codeFilm)
    {
        case FILM_K: // BW, wide temperature and fast update

            break;

        default:

            debugVariant(FILM_K);
            break;
    }

    //
    // === Touch section
    //

    //
    // === End of touch section
    //

    //
    // === Large screen section
    //
    // Check panelCSS for large screens
    if (((u_codeSize == SIZE_969) or (u_codeSize == SIZE_1198)) and (b_pin.panelCSS == NOT_CONNECTED))
    {
        mySerial.println();
        mySerial.println("hV * Required pin panelCSS is NOT_CONNECTED");
        while (0x01);
    }
    //
    // === End of Large screen section
    //

    // Configure board
    switch (u_codeSize)
    {
        // case SIZE_343: // 3.43"
        case SIZE_581: // 5.81"
            // case SIZE_741: // 7.41"

            b_begin(b_pin, FAMILY_MEDIUM, 0);
            break;

        // case SIZE_969: // 9.69"
        // case SIZE_1198: // 11.98"
        //
        //    b_begin(b_pin, FAMILY_LARGE, 50);
        //    break;

        default:

            b_begin(b_pin, FAMILY_SMALL, 0);
            break;
    }

    //
    // === Touch section
    //

    //
    // === End of touch section
    //

    // Sizes
    switch (u_codeSize)
    {
        case SIZE_150: // 1.50"
        case SIZE_152: // 1.52"

            v_screenSizeV = 200; // vertical = wide size
            v_screenSizeH = 200; // horizontal = small size
            break;

        case SIZE_154: // 1.54"

            v_screenSizeV = 152; // vertical = wide size
            v_screenSizeH = 152; // horizontal = small size
            break;

        case SIZE_206: // 2.06"

            v_screenSizeV = 248; // vertical = wide size
            v_screenSizeH = 128; // horizontal = small size
            break;

        case SIZE_213: // 2.13"

            v_screenSizeV = 212; // vertical = wide size
            v_screenSizeH = 104; // horizontal = small size
            break;

        case SIZE_266: // 2.66"

            v_screenSizeV = 296; // vertical = wide size
            v_screenSizeH = 152; // horizontal = small size
            break;

        case SIZE_271: // 2.71" and 2.71"-Touch

            v_screenSizeV = 264; // vertical = wide size
            v_screenSizeH = 176; // horizontal = small size
            break;

        case SIZE_287: // 2.87"

            v_screenSizeV = 296; // vertical = wide size
            v_screenSizeH = 128; // horizontal = small size
            break;

        case SIZE_290: // 2.90"

            v_screenSizeV = 384; // vertical = wide size
            v_screenSizeH = 168; // horizontal = small size
            break;

        case SIZE_343: // 3.43" and 3.43"-Touch

            v_screenSizeV = 392; // vertical = wide size
            v_screenSizeH = 456; // horizontal = small size
            break;

        case SIZE_350: // 3.50"

            v_screenSizeV = 384; // vertical = wide size
            v_screenSizeH = 168; // horizontal = small size
            break;

        case SIZE_370: // 3.70" and 3.70"-Touch

            v_screenSizeV = 416; // vertical = wide size
            v_screenSizeH = 240; // horizontal = small size
            break;

        case SIZE_417: // 4.17"

            v_screenSizeV = 300; // vertical = wide size
            v_screenSizeH = 400; // horizontal = small size
            break;

        case SIZE_437: // 4.37"

            v_screenSizeV = 480; // vertical = wide size
            v_screenSizeH = 176; // horizontal = small size
            break;

        // Those screens are not available with embedded fast update or wide temperature
        //         case SIZE_565: // 5.65"
        //
        //             v_screenSizeV = 600; // v = wide size
        //             v_screenSizeH = 448; // h = small size
        //             break;

        case SIZE_581: // 5.81"

            v_screenSizeV = 720; // v = wide size
            v_screenSizeH = 256; // h = small size
            break;

        //         case SIZE_741: // 7.41"
        //
        //             v_screenSizeV = 800; // v = wide size
        //             v_screenSizeH = 480; // h = small size
        //             break;
        //
        //         case SIZE_969: // 9.69"
        //
        //             v_screenSizeV = 672; // v = wide size
        //             v_screenSizeH = 960; // Actually, 960 = 480 x 2, h = small size
        //             break;
        //
        //         case SIZE_1198: // 11.98"
        //
        //             v_screenSizeV = 768; // v = wide size
        //             v_screenSizeH = 960; // Actually, 960 = 480 x 2, h = small size
        //             break;

        default:

            mySerial.println();
            mySerial.println(formatString("hV * Screen %i-%cS-0%c is not supported", u_codeSize, u_codeFilm, u_codeDriver));
            while (0x01);
            break;
    } // u_codeSize
    v_screenDiagonal = u_codeSize;

    // Report
    mySerial.println(formatString("hV = Screen %s", WhoAmI().c_str()));
    mySerial.println(formatString("hV = Size %ix%i", screenSizeX(), screenSizeY()));
    mySerial.println(formatString("hV = Number %i-%cS-0%c", u_codeSize, u_codeFilm, u_codeDriver));
    mySerial.println(formatString("hV = PDLS %s v%i.%i.%i", SCREEN_EPD_EXT3_VARIANT, SCREEN_EPD_EXT3_RELEASE / 100, (SCREEN_EPD_EXT3_RELEASE / 10) % 10, SCREEN_EPD_EXT3_RELEASE % 10));
    mySerial.println();

    u_bufferDepth = v_screenColourBits; // 2 colours
    u_bufferSizeV = v_screenSizeV; // vertical = wide size
    u_bufferSizeH = v_screenSizeH / 8; // horizontal = small size 112 / 8, 1 bit per pixel

    // Force conversion for two unit16_t multiplication into uint32_t.
    // Actually for 1 colour; BWR requires 2 pages.
    u_pageColourSize = (uint32_t)u_bufferSizeV * (uint32_t)u_bufferSizeH;

#if defined(BOARD_HAS_PSRAM) // ESP32 PSRAM specific case

    if (s_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = (uint8_t *) ps_malloc(u_pageColourSize * u_bufferDepth);
        s_newImage = (uint8_t *) _newFrameBuffer;
    }

#else // default case

    if (s_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = new uint8_t[u_pageColourSize * u_bufferDepth];
        s_newImage = (uint8_t *) _newFrameBuffer;
    }

#endif // ESP32 BOARD_HAS_PSRAM

    memset(s_newImage, 0x00, u_pageColourSize * u_bufferDepth);

    setTemperatureC(25); // 25 Celsius = 77 Fahrenheit
    b_fsmPowerScreen = FSM_OFF;
    if (b_pin.panelPower != NOT_CONNECTED)
    {
        setPowerProfile(POWER_MODE_MANUAL, POWER_SCOPE_GPIO_ONLY);
    }

    // Turn SPI on, initialise GPIOs and set GPIO levels
    // Reset panel and get tables
    resume();

    // Fonts
    hV_Screen_Buffer::begin(); // Standard

    if (f_fontMax() > 0)
    {
        f_selectFont(0);
    }
    f_fontSolid = false;

    // Orientation
    setOrientation(0);

    v_penSolid = false;
    u_invert = false;

    //
    // === Touch section
    //

    //
    // === End of Touch section
    //
}

STRING_TYPE Screen_EPD_EXT3_Fast::WhoAmI()
{
    char work[64] = {0};
    u_WhoAmI(work);

    return formatString("iTC %i.%02i\"%s", v_screenDiagonal / 100, v_screenDiagonal % 100, work);
}

void Screen_EPD_EXT3_Fast::suspend(uint8_t suspendScope)
{
    if (((suspendScope & FSM_GPIO_MASK) == FSM_GPIO_MASK) and (b_pin.panelPower != NOT_CONNECTED))
    {
        if ((b_fsmPowerScreen & FSM_GPIO_MASK) == FSM_GPIO_MASK)
        {
            b_suspend();
        }
    }
}

void Screen_EPD_EXT3_Fast::resume()
{
    // Target   FSM_ON
    // Source   FSM_OFF
    //          FSM_SLEEP
    if (b_fsmPowerScreen != FSM_ON)
    {
        if ((b_fsmPowerScreen & FSM_GPIO_MASK) != FSM_GPIO_MASK)
        {
            b_resume(); // GPIO

            s_reset(); // Reset

            b_fsmPowerScreen |= FSM_GPIO_MASK;
        }

        // Check type and get tables
        if (u_flagOTP == false)
        {
            s_getDataOTP(); // 3-wire SPI read OTP memory

            s_reset(); // Reset
        }

        // Start SPI, with unicity check
        switch (u_eScreen_EPD)
        {
            case eScreen_EPD_150_KS_0J:
            case eScreen_EPD_152_KS_0J: // 1.52" tested with 4, 8, 16 and 32 MHz
            case eScreen_EPD_206_KS_0E: // 2.06" tested with 4, 8 and 16 MHz
            case eScreen_EPD_290_KS_0F:

                hV_HAL_SPI_begin(16000000);
                break;

            default:

                hV_HAL_SPI_begin(); // Standard 8 MHz
                break;
        }
    }
}

void Screen_EPD_EXT3_Fast::s_reset()
{
    switch (b_family)
    {
        //         case FAMILY_LARGE:
        //
        //             COG_LargeK_reset();
        //             break;

        case FAMILY_MEDIUM:

            COG_MediumK_reset();
            break;

        case FAMILY_SMALL:

            COG_SmallK_reset();
            break;

        default:

            break;
    }
}

void Screen_EPD_EXT3_Fast::s_getDataOTP()
{
    hV_HAL_SPI_end(); // With unicity check

    hV_HAL_SPI3_begin(); // Define 3-wire SPI pins

    // Get data OTP
    switch (b_family)
    {
        //         case FAMILY_LARGE:
        //
        //             COG_LargeK_getDataOTP();
        //             break;

        case FAMILY_MEDIUM:

            COG_MediumK_getDataOTP();
            break;

        case FAMILY_SMALL:

            COG_SmallK_getDataOTP();
            break;

        default:

            break;
    }
}

void Screen_EPD_EXT3_Fast::s_flush(uint8_t updateMode)
{
    // Resume
    if (b_fsmPowerScreen != FSM_ON)
    {
        resume();
    }

    switch (b_family)
    {
        //         case FAMILY_LARGE:
        //
        //             COG_LargeK_initial(updateMode); // Initialise
        //             COG_LargeK_sendImageData(updateMode); // Send image data
        //             COG_LargeK_update(updateMode); // Update
        //             COG_LargeK_powerOff(); // Power off
        //             break;

        case FAMILY_MEDIUM:

            COG_MediumK_initial(updateMode); // Initialise
            COG_MediumK_sendImageData(updateMode); // Send image data
            COG_MediumK_update(updateMode); // Update
            COG_MediumK_powerOff(); // Power off
            break;

        case FAMILY_SMALL:

            COG_SmallK_initial(updateMode); // Initialise
            COG_SmallK_sendImageData(updateMode); // Send image data
            COG_SmallK_update(updateMode); // Update
            COG_SmallK_powerOff(); // Power off
            break;

        default:

            break;
    }

    // Suspend
    if (u_suspendMode == POWER_MODE_AUTO)
    {
        suspend(u_suspendScope);
    }
}

uint8_t Screen_EPD_EXT3_Fast::flushMode(uint8_t updateMode)
{
    updateMode = checkTemperatureMode(updateMode);

    switch (updateMode)
    {
        case UPDATE_FAST:
        case UPDATE_GLOBAL:

            s_flush(UPDATE_FAST);
            break;

        default:

            mySerial.println();
            mySerial.println("hV ! PDLS - UPDATE_NONE invoked");
            break;
    }

    return updateMode;
}

void Screen_EPD_EXT3_Fast::flush()
{
    flushMode(UPDATE_FAST);
}

void Screen_EPD_EXT3_Fast::clear(uint16_t colour)
{
    if (colour == myColours.grey)
    {
        // black = 0-1, white = 0-0
        for (uint16_t i = 0; i < u_bufferSizeV; i++)
        {
            uint8_t pattern = (i % 2) ? 0b10101010 : 0b01010101;
            for (uint16_t j = 0; j < u_bufferSizeH; j++)
            {
                s_newImage[i * u_bufferSizeH + j] = pattern;
            }
        }
    }
    else if ((colour == myColours.white) xor u_invert)
    {
        // physical black 0-0
        memset(s_newImage, 0x00, u_pageColourSize);
    }
    else
    {
        // physical white 1-0
        memset(s_newImage, 0xff, u_pageColourSize);
    }
}

void Screen_EPD_EXT3_Fast::regenerate(uint8_t mode)
{
    clear(myColours.black);
    flush();
    delay(100);

    clear(myColours.white);
    flush();
    delay(100);
}

void Screen_EPD_EXT3_Fast::s_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    // Orient and check coordinates are within screen
    if (s_orientCoordinates(x1, y1) == RESULT_ERROR)
    {
        return;
    }

    // Convert combined colours into basic colours
    bool flagOdd = ((x1 + y1) % 2 == 0);

    if (colour == myColours.grey)
    {
        if (flagOdd)
        {
            colour = myColours.black; // black
        }
        else
        {
            colour = myColours.white; // white
        }
    }

    // Coordinates
    uint32_t z1 = s_getZ(x1, y1);
    uint16_t b1 = s_getB(x1, y1);

    // Basic colours
    if ((colour == myColours.white) xor u_invert)
    {
        // physical black 0-0
        bitClear(s_newImage[z1], b1);
    }
    else if ((colour == myColours.black) xor u_invert)
    {
        // physical white 1-0
        bitSet(s_newImage[z1], b1);
    }
}

void Screen_EPD_EXT3_Fast::s_setOrientation(uint8_t orientation)
{
    v_orientation = orientation % 4;
}

bool Screen_EPD_EXT3_Fast::s_orientCoordinates(uint16_t & x, uint16_t & y)
{
    bool _flagResult = RESULT_ERROR;
    switch (v_orientation)
    {
        case 3: // checked, previously 1

            if ((x < v_screenSizeV) and (y < v_screenSizeH))
            {
                x = v_screenSizeV - 1 - x;
                _flagResult = RESULT_SUCCESS;
            }
            break;

        case 2: // checked

            if ((x < v_screenSizeH) and (y < v_screenSizeV))
            {
                x = v_screenSizeH - 1 - x;
                y = v_screenSizeV - 1 - y;
                swap(x, y);
                _flagResult = RESULT_SUCCESS;
            }
            break;

        case 1: // checked, previously 3

            if ((x < v_screenSizeV) and (y < v_screenSizeH))
            {
                y = v_screenSizeH - 1 - y;
                _flagResult = RESULT_SUCCESS;
            }
            break;

        default: // checked

            if ((x < v_screenSizeH) and (y < v_screenSizeV))
            {
                swap(x, y);
                _flagResult = RESULT_SUCCESS;
            }
            break;
    }

    return _flagResult;
}

uint32_t Screen_EPD_EXT3_Fast::s_getZ(uint16_t x1, uint16_t y1)
{
    uint32_t z1 = 0;
    // According to 11.98 inch Spectra Application Note
    // at http://www.pervasivedisplays.com/LiteratureRetrieve.aspx?ID=245146
    switch (u_codeSize)
    {
        case SIZE_969:
        case SIZE_1198:

            if (y1 >= (v_screenSizeH >> 1))
            {
                y1 -= (v_screenSizeH >> 1); // rebase y1
                z1 += (u_pageColourSize >> 1); // buffer second half
            }
            z1 += (uint32_t)x1 * (u_bufferSizeH >> 1) + (y1 >> 3);
            break;

        default:

            z1 = (uint32_t)x1 * u_bufferSizeH + (y1 >> 3);
            break;
    }
    return z1;
}

uint16_t Screen_EPD_EXT3_Fast::s_getB(uint16_t x1, uint16_t y1)
{
    uint16_t b1 = 0;

    b1 = 7 - (y1 % 8);

    return b1;
}

uint16_t Screen_EPD_EXT3_Fast::s_getPoint(uint16_t x1, uint16_t y1)
{
    return 0x0000;
}
//
// === End of Class section
//

//
// === Touch section
//

//
// === End of Touch section
//

