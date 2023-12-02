/*
 * lock.h
 *
 *  Created on: Nov 20, 2023
 *      Author: saaci
 */

#ifndef INC_LOCK_H_
#define INC_LOCK_H_


#include <stdint.h>


void lock_init(void);
void lock_sequence_handler(uint8_t key);
void lock_keylock(uint8_t key);
static uint8_t lock_open_lock(void);


#endif /* INC_LOCK_H_ */
