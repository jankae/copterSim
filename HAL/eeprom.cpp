#include "eeprom.h"

using namespace std;

struct Eeprom eeprom;
struct Config config;

//TODO fill all functions
/*
 * writes a single byte to the eeprom
 */
void eeprom_WriteByte(uint16_t adr, uint8_t data) {

}
/*
 * reads a single byte from the eeprom
 */
void eeprom_ReadByte(uint16_t adr, uint8_t *data) {
	*data = 0xFF;
}
/*
 * writes a page (see EEPROM_PAGE_SIZE) to the eeprom
 * @param page 	number of the eeprom page to be writte
 * @param data 	pointer to the data to be written
 * @param size 	number of bytes to be written, might be
 * 				any value between 1 and EEPROM_PAGE_SIZE
 */
void eeprom_WritePage(uint16_t page, uint8_t *data, uint8_t size) {

}
/*
 * reads a block of data from the eeprom
 */
void eeprom_ReadBlock(uint16_t adr, uint8_t *data, uint16_t size) {

}
/*
 * writes a block of data to the eeprom
 */
void eeprom_WriteBlock(uint16_t adr, uint8_t *data, uint16_t size) {

}
/*
 * loads config values from the external i2c eeprom
 */
void eeprom_LoadConfig(void) {
	ifstream file;
	file.open("config", ios::in | ios::binary);
	if (!file.is_open()) {
		cout << "ERROR: no eeprom config file" << endl;
		return;
	}
	char version, subversion;
	file.read(&version, 1);
	file.read(&subversion, 1);
	if (version != VERSION) {
		cout << "ERROR: Wrong eeprom config file version. File not compatible."
				<< endl;
		return;
	}
	if (subversion != SUBVERSION) {
		cout << "WARNING: Wrong eeprom config file subversion." << endl;
		return;
	}
	file.read((char*) &config, sizeof(config));
	file.close();
	cout << "eeprom config file loaded" << endl;
	eeprom.configLoaded = SET;
}
/*
 * saves the config values to the eeprom
 */
void eeprom_SaveConfig(void) {

}
