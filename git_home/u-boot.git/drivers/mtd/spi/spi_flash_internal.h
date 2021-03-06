/*
 * SPI flash internal definitions
 *
 * Copyright (C) 2008 Atmel Corporation
 */

/* Common parameters -- kind of high, but they should only occur when there
 * is a problem (and well your system already is broken), so err on the side
 * of caution in case we're dealing with slower SPI buses and/or processors.
 */
#define SPI_FLASH_PROG_TIMEOUT		(2 * CONFIG_SYS_HZ)
#define SPI_FLASH_PAGE_ERASE_TIMEOUT	(5 * CONFIG_SYS_HZ)
#define SPI_FLASH_SECTOR_ERASE_TIMEOUT	(10 * CONFIG_SYS_HZ)
#define SPI_FLASH_BERASE_TIMEOUT(f)	((f)->berase_timeout * CONFIG_SYS_HZ)
#define FAST_READ_DUMMY_BYTE		1

/* Common commands */
#define CMD_READ_ID			0x9f

#define CMD_READ_ARRAY_SLOW		0x03
#define CMD_READ_ARRAY_FAST		0x0b

/* Spansion new commands for 4-byte address access */
#define CMD_4READ_ARRAY_FAST		0x0c
#define CMD_4PAGE_PROGRAM		0x12

#define CMD_WRITE_STATUS		0x01
#define CMD_PAGE_PROGRAM		0x02
#define CMD_WRITE_DISABLE		0x04
#define CMD_READ_STATUS			0x05
#define CMD_WRITE_ENABLE		0x06

/* Common status */
#define STATUS_WIP			0x01

#if defined(CONFIG_HW29765265P16P0P256P2X2P2X2) || \
	defined(CONFIG_HW29765285P16P0P128) || \
	defined(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4PVERIZON) || \
	defined(CONFIG_HW29765619P0P256P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765619P0P256P512P2X2P2X2P4X4PVERIZON) || \
	defined(CONFIG_HW29765641P0P256P512P2X2P2X2P2X2) || \
	defined(CONFIG_HW29765641P0P128P512P2X2P2X2P2X2) || \
	defined(CONFIG_HW29765641P0P128P256P2X2P2X2P2X2) || \
	defined(CONFIG_HW29766182P0P256P512P2X2P2X2P2X2P1) || \
	defined(CONFIG_HW29766105P0P128P512P2X2P2X2) || \
	defined(CONFIG_HW29765906P0P256P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765913P0P512P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765913P0P512P512P2X2P2X2P4X4PVERIZON) || \
	defined(CONFIG_HW29765913P0P128P256P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765913P0P128P256P2X2P2X2P4X4PVERIZON) || \
	defined(CONFIG_HW29765352P0P4096P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765352P32P0P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765515P0P4096P512P2X2P2X2P2X2) || \
	defined(CONFIG_HW29766106P32P0P256P2X2P2X2)
/* SPI Nand commands */
#ifdef CONFIG_SPI_NAND
#define SPI_NAND_CMD_WRITE_ENABLE	0x06
#define SPI_NAND_CMD_WRITE_DISABLE	0x04
#define SPI_NAND_CMD_GET_FEATURE	0x0f
#define SPI_NAND_CMD_SET_FEATURE	0x1f
#define SPI_NAND_CMD_PAGE_READ_TO_CACHE	0x13
#define SPI_NAND_CMD_READ_FROM_CACHE	0x03
#define SPI_NAND_CMD_READ_ID		0x9f
#define SPI_NAND_CMD_BLOCK_ERASE	0xd8
#define SPI_NAND_CMD_RESET		0xff
#define SPI_NAND_CMD_PROGRAM_LOAD	0x02
#define SPI_NAND_CMD_PROGRAM_EXECUTE	0x10
#define ATH_SPI_NAND_BLK_PROT		0xa0
#endif
#endif

/* Send a single-byte command to the device and read the response */
int spi_flash_cmd(struct spi_slave *spi, u8 cmd, void *response, size_t len);

/*
 * Send a multi-byte command to the device and read the response. Used
 * for flash array reads, etc.
 */
int spi_flash_cmd_read(struct spi_slave *spi, const u8 *cmd,
		size_t cmd_len, void *data, size_t data_len);

int spi_flash_cmd_read_fast(struct spi_flash *flash, u32 offset,
		size_t len, void *data);

/*
 * Send a multi-byte command to the device followed by (optional)
 * data. Used for programming the flash array, etc.
 */
int spi_flash_cmd_write(struct spi_slave *spi, const u8 *cmd, size_t cmd_len,
		const void *data, size_t data_len);

/*
 * Write the requested data out breaking it up into multiple write
 * commands as needed per the write size.
 */
int spi_flash_cmd_write_multi(struct spi_flash *flash, u32 offset,
		size_t len, const void *buf);

/*
 * Enable writing on the SPI flash.
 */
static inline int spi_flash_cmd_write_enable(struct spi_flash *flash)
{
	return spi_flash_cmd(flash->spi, CMD_WRITE_ENABLE, NULL, 0);
}

/*
 * Disable writing on the SPI flash.
 */
static inline int spi_flash_cmd_write_disable(struct spi_flash *flash)
{
	return spi_flash_cmd(flash->spi, CMD_WRITE_DISABLE, NULL, 0);
}

/*
 * Same as spi_flash_cmd_read() except it also claims/releases the SPI
 * bus. Used as common part of the ->read() operation.
 */
int spi_flash_read_common(struct spi_flash *flash, const u8 *cmd,
		size_t cmd_len, void *data, size_t data_len);

/* Send a command to the device and wait for some bit to clear itself. */
int spi_flash_cmd_poll_bit(struct spi_flash *flash, unsigned long timeout,
			   u8 cmd, u8 poll_bit);

/*
 * Send the read status command to the device and wait for the wip
 * (write-in-progress) bit to clear itself.
 */
int spi_flash_cmd_wait_ready(struct spi_flash *flash, unsigned long timeout);

/* Erase sectors. */
int spi_flash_cmd_erase(struct spi_flash *flash, u8 erase_cmd,
			u32 offset, size_t len);

/* Erase block */
int spi_flash_cmd_erase_block(struct spi_flash *flash, u8 erase_cmd,
			u32 offset, size_t len);

/* Manufacturer-specific probe functions */
struct spi_flash *spi_flash_probe_spansion(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_atmel(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_eon(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_macronix(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_sst(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_stmicro(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_winbond(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_giga(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_fram_probe_ramtron(struct spi_slave *spi, u8 *idcode);
#if defined(CONFIG_HW29765265P16P0P256P2X2P2X2) || \
	defined(CONFIG_HW29765285P16P0P128) || \
	defined(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4PVERIZON) || \
	defined(CONFIG_HW29765619P0P256P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765619P0P256P512P2X2P2X2P4X4PVERIZON) || \
	defined(CONFIG_HW29765641P0P256P512P2X2P2X2P2X2) || \
	defined(CONFIG_HW29765641P0P128P512P2X2P2X2P2X2) || \
	defined(CONFIG_HW29765641P0P128P256P2X2P2X2P2X2) || \
	defined(CONFIG_HW29766182P0P256P512P2X2P2X2P2X2P1) || \
	defined(CONFIG_HW29766105P0P128P512P2X2P2X2) || \
	defined(CONFIG_HW29765906P0P256P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765913P0P512P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765913P0P512P512P2X2P2X2P4X4PVERIZON) || \
	defined(CONFIG_HW29765913P0P128P256P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765913P0P128P256P2X2P2X2P4X4PVERIZON) || \
	defined(CONFIG_HW29765352P0P4096P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765352P32P0P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765515P0P4096P512P2X2P2X2P2X2) || \
	defined(CONFIG_HW29766106P32P0P256P2X2P2X2)
struct spi_flash *spi_nand_flash_probe(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_nor_probe_generic(struct spi_slave *spi, u8 *idcode);

/*
 * Send the read status command to the spi nand device and wait for the wip
 * (write-in-progress) bit to clear itself.
 */
int spi_nand_flash_cmd_wait_ready(struct spi_flash *flash, u8 status_bit, u8 *status,
                                  unsigned long timeout);
#endif
