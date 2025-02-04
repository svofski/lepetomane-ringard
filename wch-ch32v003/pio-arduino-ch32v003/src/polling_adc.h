#include "ch32v003fun.h"

namespace polling_adc {

/*
 * initialize adc for polling
 */
void init( void );

/*
 * start conversion, wait and return result
 */
uint16_t get( void );

void begin_continuous(void);
uint16_t get_continuous(void);

}
