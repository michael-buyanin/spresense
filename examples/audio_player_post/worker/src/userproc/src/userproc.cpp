/****************************************************************************
 * audio_player_post/worker/src/userproc/src/userproc.cpp
 *
 *   Copyright 2018 Sony Semiconductor Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Sony Semiconductor Solutions Corporation nor
 *    the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include "userproc.h"

/*--------------------------------------------------------------------*/
/*                                                                    */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
void UserProc::init(InitParam *param)
{
  param->result.result_code = PostprocCommand::ExecOk;
}

/*--------------------------------------------------------------------*/
void UserProc::exec(ExecParam *param)
{
  /* !!tentative!! simply copy from input to output */

  memcpy(param->exec_cmd.output.addr,
         param->exec_cmd.input.addr,
         param->exec_cmd.input.size);

  param->exec_cmd.output.size = param->exec_cmd.input.size;

  if (m_toggle)
  {
    /* RC filter example */

    int16_t *ls = (int16_t*)param->exec_cmd.output.addr;
    int16_t *rs = ls + 1;

    static int16_t ls_l = 0;
    static int16_t rs_l = 0;

    if (!ls_l && !rs_l)
      {
        ls_l = *ls;
        rs_l = *rs;
      }

    for (uint32_t cnt = 0; cnt < param->exec_cmd.input.size; cnt += 4)
      {
        *ls = (ls_l * 99 / 100) + (*ls * 1 / 100);
        *rs = (rs_l * 99 / 100) + (*rs * 1 / 100);

        ls_l = *ls;
        rs_l = *rs;

        ls += 2;
        rs += 2;
      }
  }

  param->result.result_code = PostprocCommand::ExecOk;
}

/*--------------------------------------------------------------------*/
void UserProc::flush(FlushParam *param)
{
  param->flush_cmd.output.size = 0;

  param->result.result_code = PostprocCommand::ExecOk;
}

/*--------------------------------------------------------------------*/
void UserProc::set(SetParam *param)
{
  m_toggle = param->postswitch;

  param->result.result_code = PostprocCommand::ExecOk;
}

