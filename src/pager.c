#include "pager.h"

#include "cpu.h"
#include <stdio.h>

#define PORT 6502
#define PAGE_SIZE 256

uint8_t pageBuf[PAGE_SIZE];
int sockfd, n;
struct sockaddr_in servaddr;
bool init = false;

uint8_t currentPage = 1;

void send_page(void)
{
    m6502Word pw = {.h=currentPage, .l=0};
    memcpy(pageBuf, mainMemory + pw.w, PAGE_SIZE);

    sendto(sockfd, pageBuf, PAGE_SIZE, 0, (struct sockaddr*)NULL, sizeof(servaddr));
}

void parse_page_request(void)
{

}

void step_pager(void)
{
    if (!init)
    {
        init = true;

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(PORT);
        servaddr.sin_family = AF_INET;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);

        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        {
            perror("Connect failed\n");
        }
    }

    parse_page_request();
    send_page();
}
