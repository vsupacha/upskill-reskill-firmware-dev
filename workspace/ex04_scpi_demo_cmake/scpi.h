#ifndef _SCPI_H_
#define _SCPI_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "app_config.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
    SCPI_STATUS_IDLE = 0,
    SCPI_STATUS_READY = 1,
    SCPI_STATUS_ERROR = -1
} scpi_status_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
scpi_status_t SCPI_Parse_Char(char ch);
int SCPI_Execute(char *cmd_resp);
int SCPI_Error(int err_code, char *error_msg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCPI_H_ */