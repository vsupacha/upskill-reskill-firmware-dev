#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "scpi.h"
#include "app_config.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    SCPI_PARSER_IDLE,
    SCPI_PARSER_CMD,
    SCPI_PARSER_PARAM
} scpi_parser_state_t;

typedef struct {
    char cmd_buf[MAX_SCPI_BUF_SIZE];    // command string
    char param_buf[MAX_SCPI_BUF_SIZE];  // parameter string
    uint8_t cmd_buf_index;              // cmd_buf index
    uint8_t param_buf_index;            // param_buf index
    bool ready_flag;                    // Terminator (\n) found
    bool is_query;                      // query command (?)
} scpi_cmd_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
scpi_parser_state_t  scpi_parser_state = SCPI_PARSER_IDLE;
scpi_cmd_t scpi_cmd = {
    .cmd_buf_index = 0,
    .param_buf_index = 0,
    .ready_flag = false,
    .is_query = false
};

/* Private functions prototype -----------------------------------------------*/
static inline void scpi_reset_cmd() {
    memset(scpi_cmd.cmd_buf, 0, MAX_SCPI_BUF_SIZE);
    memset(scpi_cmd.param_buf, 0, MAX_SCPI_BUF_SIZE);
    scpi_cmd.cmd_buf_index = 0;
    scpi_cmd.param_buf_index = 0;
    scpi_cmd.ready_flag = false;
    scpi_cmd.is_query = false;
}

scpi_status_t SCPI_Parse_Char(char ch) {
    scpi_status_t scpi_status = SCPI_STATUS_IDLE; 

    // if previous command not finish, error
    if (scpi_cmd.ready_flag) {
        scpi_parser_state = SCPI_PARSER_IDLE;
        return SCPI_STATUS_ERROR;
    }

    // check EOL
    if (ch == '\n') {
        if (scpi_cmd.cmd_buf_index > 0) {
            scpi_cmd.cmd_buf[scpi_cmd.cmd_buf_index] = 0;
            scpi_cmd.param_buf[scpi_cmd.param_buf_index] = 0;
            scpi_cmd.ready_flag = true;
            scpi_parser_state = SCPI_PARSER_IDLE;
            return SCPI_STATUS_READY;
        } else {
            scpi_parser_state = SCPI_PARSER_IDLE;
            return SCPI_STATUS_IDLE;
        }
    }

    // check buffer overflow
    if ((scpi_cmd.cmd_buf_index >= MAX_SCPI_BUF_SIZE-1) || (scpi_cmd.param_buf_index >= MAX_SCPI_BUF_SIZE-1)) {
        scpi_reset_cmd();
        scpi_parser_state = SCPI_PARSER_IDLE;
        return SCPI_STATUS_ERROR;
    }

    // state machine to parse SCPI command
    switch (scpi_parser_state) {
        case SCPI_PARSER_IDLE:
            if ((ch == ':') || (ch == '*')) {
                scpi_cmd.cmd_buf[0] = ch;
                scpi_cmd.cmd_buf_index = 1;
                scpi_parser_state = SCPI_PARSER_CMD;
            } else {
                scpi_reset_cmd();
            }
            break;
        case SCPI_PARSER_CMD:
            if (ch != ' ') {
                if (ch != '\r') {
                    scpi_cmd.cmd_buf[scpi_cmd.cmd_buf_index++] = ch;
                    if (ch == '?') {
                        scpi_cmd.is_query = true;
                    }
                } else {
                    // ignore \r
                }
            } else {
                scpi_parser_state = SCPI_PARSER_PARAM;
            }
            break; 
        case SCPI_PARSER_PARAM:
            if (ch != '\r') {
                scpi_cmd.param_buf[scpi_cmd.param_buf_index++] = ch;
            } else {
                // ignore \r
            }
            break;
        default:
            scpi_parser_state = SCPI_PARSER_IDLE;
    }

    return scpi_status;
}

int SCPI_Execute(char *cmd_resp) {
    int resp_len = 0;
    cmd_resp[0] = 0;

    if (scpi_cmd.ready_flag) {
        if (strcmp(scpi_cmd.cmd_buf, "*IDN?") == 0) {
            resp_len = sniprintf(cmd_resp, MAX_SCPI_BUF_SIZE, "TU,USB2ADC,V1.0\r\n");
        }
    } else {
        resp_len = -1;
    }
    scpi_reset_cmd();

    return resp_len;
}

int SCPI_Error(int err_code, char *error_msg) {
    // Handle error message, e.g., print to console or log
    int err_len = 0;

    error_msg[0] = 0;
    scpi_reset_cmd();
    return err_len;
}