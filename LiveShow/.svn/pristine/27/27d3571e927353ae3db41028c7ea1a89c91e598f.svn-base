#include "dse.h"
#include "bitstream.h"
#include "counters.h" /* the 3GPP instrumenting tool */

void CDse_Read(HANDLE_BIT_BUF bs,
	       long *byteBorder)
{
  char data_byte_align_flag;
  short cnt, i;

  COUNT_sub_start("CDse_Read");

  FUNC(2);
  GetBits(bs, 4);

  FUNC(2); MOVE(1);
  data_byte_align_flag = GetBits(bs, 1);

  FUNC(2); MOVE(1);
  cnt = GetBits(bs, 8);

  ADD(1); BRANCH(1);
  if (cnt == 255) {
    ADD(1); FUNC(2);
    cnt += GetBits(bs, 8);
  }

  BRANCH(1);
  if (data_byte_align_flag) {
    FUNC(2);
    ByteAlign(bs, byteBorder);
  }

  LOOP(1);
  for (i = 0; i < cnt; i++) {
    FUNC(2);
    GetBits(bs, 8);
  }
}
