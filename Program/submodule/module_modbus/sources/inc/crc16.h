#ifndef CRC16
#define CRC16



#ifdef __cplusplus
extern "C" {
#endif



/* ======================================================================== */
/*  CRC16_UPDATE -- Updates a 16-bit CRC using the lookup table above.      */
/*                  Note:  The 16-bit CRC is set up as a left-shifting      */
/*                  CRC with no inversions.                                 */
/* ======================================================================== */
uint16_t crc16_update(uint16_t crc, uint8_t data);

/* ======================================================================== */
/*  CRC16_BLOCK  -- Updates a 16-bit CRC on a block of 8-bit data.          */
/*                  Note:  The 16-bit CRC is set up as a left-shifting      */
/*                  CRC with no inversions.                                 */
/* ======================================================================== */
uint16_t crc16_block(uint16_t crc, uint8_t *data, uint32_t len);



#ifdef __cplusplus
}
#endif //cpp



#endif // CRC16

