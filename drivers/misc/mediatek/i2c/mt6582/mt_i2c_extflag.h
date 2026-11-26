#ifndef __MT_I2C_EXTFLAG_H__
#define __MT_I2C_EXTFLAG_H__

/* MTK-specific I2C flags carried in i2c_client.ext_flag / i2c_msg.ext_flag. */
#define I2C_A_FILTER_MSG	0x8000	/* filter out error messages */
#define I2C_A_CHANGE_TIMING	0x4000	/* change timing parameters */
#define I2C_MASK_FLAG		(0x00ff)
#define I2C_DMA_FLAG		(0x2000)
#define I2C_WR_FLAG		(0x1000)
#define I2C_RS_FLAG		(0x0800)
#define I2C_HS_FLAG		(0x0400)
#define I2C_ENEXT_FLAG		(0x0200)
#define I2C_DISEXT_FLAG		(0x0000)
#define I2C_PUSHPULL_FLAG	(0x0100)
#define I2C_POLL_FLAG		(0x4000)
#define I2C_CH2_FLAG		(0x2000)
#define I2C_POLLING_FLAG	(0x00000001)
#define I2C_3DCAMERA_FLAG	(0x00000004)
#define I2C_DIRECTION_FLAG	(0x00000008)

#endif /* __MT_I2C_EXTFLAG_H__ */
