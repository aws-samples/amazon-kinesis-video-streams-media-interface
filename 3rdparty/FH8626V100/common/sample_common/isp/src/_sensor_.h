#ifndef ____sensor___h__
#define ____sensor___h__

extern struct isp_sensor_if *start_sensor(void);
extern char *get_isp_sensor_param(char *sensor_name, int flags, int *olen);
extern void free_isp_sensor_param(char *param);

#endif /*____sensor___h__*/

