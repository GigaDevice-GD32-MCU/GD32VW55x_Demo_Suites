/*!
    \file    gd32vw55x_it.c
    \brief   interrupt service routines

    \version 2026-02-27, V1.6.0, demo for GD32VW55x
*/

/*
    Copyright (c) 2026, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32vw55x_it.h"
#include "ir_decode.h"
#include "rc5_encode.h"
#include "rc5_decode.h"
#include "systick.h"

uint32_t icva[50]={0};
uint32_t icvl[50]={0};
uint8_t  n;


extern uint32_t rc5_frame_manchester_format;
extern __IO trc5_packet_struct rc5_tmp_packet;

void eclic_mtip_handler(void)
{
    ECLIC_ClearPendingIRQ(CLIC_INT_TMR);
    delay_decrement();
}

/*!
    \brief      this function handles TIMER15 update interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER15_IRQHandler(void)
{
    /* clear TIMER15 update interrupt */
    timer_interrupt_flag_clear(TIMER15, TIMER_INT_UP);
    rc5_encode_signal_generate(rc5_frame_manchester_format);


}

/*!
    \brief      this function handles IR_TIMER overflow and update interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER1_IRQHandler(void)
{
    /* ic2 interrupt*/
    if((RESET != timer_interrupt_flag_get(IR_TIMER, TIMER_INT_FLAG_CH2))){
        /* clear IR_TIMER CH1 interrupt */
        timer_interrupt_flag_clear(IR_TIMER, TIMER_INT_FLAG_CH2);
        /* get the input capture value */
        icvl[n] = timer_channel_capture_value_register_read(TIMER1,TIMER_CH_2);
        /* rc5 */
        if(n>=1){
          icva[n] = icvl[n]-icvl[n-1];
          if((n&1)==1){
            rc5_data_sampling(icva[n], 1);
          }else{
            rc5_data_sampling(icva[n], 0);
            }
        
        }
        n++;
    }

}
