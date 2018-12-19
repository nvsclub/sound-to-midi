#include <util/twi.h>
#include <util/delay.h>

/*
 *
 * I2C DESCRIPTION VARIABLES
 *
 * */

/* Any Variable Related to the Two Wire Library
 * can be checked below for meaning
 * TWCR   - Two Wire COntrol Register
 * TWINT  - Two Wire Interrupt Flag
 * TWSTA  - Two Wire Start Condition Bite
 * TWEN   - Two Wire ENable Bit
 * TWRD   - Two Wire Data Register
 * TWSR   - Two Wire Status register
 * TWBR   - Two Wire Bit Rate
 * TWSTO  - Two Wire Stop Condition Bit
 * TWEA   - Two Wire Enable Acknowledge Bit
 * TWIE   - Enables Interrupt Ability
 * 0x08   - TW Start
 * 0x18   - Acknowledge
 * 0x20   - No Acknowledge
 * 0xF8   - no INFO
 * */

/* DEFINE I2C VARIABLES */
#define F_SCL 100000UL
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16) / 2)

#define I2C_READ 0x01
#define I2C_WRITE 0xFE
#define I2C_ADDRESS 0xA0
/* I2C_ADDRESS - 7 bit address, last one defines Write or Read */

/* INITIALIZES MASTER FOR I2C CONNECTION */
void i2c_init_master(void);

/* INITIALIZES MASTER FOR I2C CONNECTION */
void i2c_init_slave(void);

/* STOPS MASTER CONNECTION */
void i2c_stop_master(void);

/* STOPS SLAVE CONNECTION */
void i2c_stop_slave(void);

/* START I2C COMMUNICATION BETWEEN MODULES - MASTER ONLY */
uint8_t i2c_start(uint8_t address, uint8_t mode);

/* WRITES DATA TO SLAVE - MASTER ONLY */
uint8_t i2c_write(uint8_t data);

/* TRASMITTS DATA FROM BUFFER - SLAVE ONLY */
void i2c_slave_trans(uint8_t data, uint8_t i2c_address_send, uint8_t i2c_databuffer[255]);

/* READS DATA FROM SLAVE WITH ACKNOWLEDGE - MASTER ONLY */
uint8_t i2c_read_w_ack(void);

/* READS DATA FROM SLAVE WITHOUT ACKNOWLEDGE - MASTER ONLY */
uint8_t i2c_read_w_nack(void);

/* READS DATA FROM SLAVE WITH ACKNOWLEDGE - SLAVE ONLY */
void i2c_slave_receive();

/* HANDLES SENDING MIDI MESSAGES */
void i2c_handler(uint16_t midi_msg);
