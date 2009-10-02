/*
 * Keyboard Upgrade -- Firmware for homebrew computer keyboard controllers.
 * Copyright (C) 2009  Robert Homann
 *
 * This file is part of the Keyboard Upgrade package.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Keyboard Upgrade package; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301  USA
 */

#if !(defined ROWS_DDR1 && defined ROWS_PORT1)
#error Please define ROWS_DDR1 and ROWS_PORT1.
#endif

#if NUM_OF_ROWS > 8
#if !(defined ROWS_DDR2 && defined ROWS_PORT2)
#error Please define ROWS_DDR2 and ROWS_PORT2.
#endif
#endif

#if NUM_OF_ROWS > 16
#error Number of rows is greater than 16---Not implemented.
#endif

/*
 * Update global array column_states with values read from the I/O pins.
 */
static char update_column_states(void)
{
  char state_changed=0;

#if NUM_OF_ROWS > 8
  ROWS_DDR2=0x00;
  ROWS_PORT2=0xff;
#endif

  for(uint8_t row=0; row < NUM_OF_ROWS; ++row)
  {
#if NUM_OF_ROWS > 8
    if(row == 8)
    {
      ROWS_DDR1=0x00;
      ROWS_PORT1=0xff;
    }

    if(row < 8)
    {
#endif
      ROWS_DDR1=_BV(row);
      ROWS_PORT1=~_BV(row);
#if NUM_OF_ROWS > 8
    }
    else
    {
      ROWS_DDR2=_BV(row&0x07);
      ROWS_PORT2=~_BV(row&0x07);
    }
#endif

    _delay_us(30);

    Columnstate cols=COLS_PIN;
    cols|=column_valid_mask[row];
    if(column_states[row] != cols)
    {
      column_states[row]=cols;
      state_changed=1;
    }
  }

  return state_changed;
}
