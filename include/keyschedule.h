#ifndef KEYSCHEDULE_H
#define KEYSCHEDULE_H

#include <stdint.h>

void derive_subkeys(const uint32_t k[8], uint32_t rk[17][4]);

#endif // KEYSCHEDULE_H
