#pragma once

void uart_init();

void uart_putc(int c);

int uart_getc();

void uart_puts(const char *s);

//Display a binary value in hexadecimal
//(really temporary)
void hex(int d);
