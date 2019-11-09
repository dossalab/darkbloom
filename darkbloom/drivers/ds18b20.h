#ifndef DS18B20_H
#define DS18B20_H

int ds18b20_init(void);
int ds18b20_convert(void);
int ds18b20_get_temp(int *temp);

#endif

