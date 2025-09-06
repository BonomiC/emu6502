#ifndef PAGER_H_
#define PAGER_H_

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void send_page(void);
void parse_page_request(void);
void step_pager(void);

#endif /* PAGER_H_ */
