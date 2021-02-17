#include <inttypes.h>
#include <avr/io.h>

#include "bits_macros.h"
/*
* ----------------------------------------------------------------------------
* Р В Р вЂ Р В РІР‚С™Р РЋРЎв„ўTHE COFFEEWARE LICENSEР В Р вЂ Р В РІР‚С™Р РЋРЎС™ (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* Please define your platform spesific functions in this file ...
* -----------------------------------------------------------------------------
*/




/* ------------------------------------------------------------------------- */
//void nrf24_setupPins()
//{
//    set_bit(DDRB,0); // CE output
//    set_bit(DDRB,1); // CSN output
//    set_bit(DDRB,5); // SCK output
//    set_bit(DDRB,3); // MOSI output
//    clear_bit(DDRB,4); // MISO input
//}
/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(unsigned char state)
{
    if(state)
    {
        SetBit(NRF_PORT, NRF_CE);
    }
    else
    {
        ClearBit(NRF_PORT, NRF_CE);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_csn_digitalWrite(unsigned char state)
{
    if(state)
    {
        SetBit(NRF_PORT, NRF_CSN);
    }
    else
    {
        ClearBit(NRF_PORT, NRF_CSN);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_sck_digitalWrite(unsigned char state)
{
    if(state)
    {
        SetBit(NRF_PORT, NRF_SCK);
    }
    else
    {
        ClearBit(NRF_PORT, NRF_SCK);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_mosi_digitalWrite(unsigned char state)
{
    if(state)
    {
        SetBit(NRF_PORT, NRF_MOSI);
    }
    else
    {
        ClearBit(NRF_PORT, NRF_MOSI);
    }
}
/* ------------------------------------------------------------------------- */
unsigned char nrf24_miso_digitalRead(void)
{
    return BitIsSet(NRF_PIN, NRF_MISO);
}
/* ------------------------------------------------------------------------- */
