#include "printf.h"
#include "raspi/uart0.h"

uint32_t sprintf(char *dst, const char *fmt, ...) {
        __builtin_va_list args;
        __builtin_va_start(args, fmt);
        return vsprintf(dst, fmt, args);
}

uint32_t printf(const char *fmt, ...) {
        __builtin_va_list args;
        __builtin_va_start(args, fmt);
        return vprintf(fmt, args);
}

//this is not thread safe and a horrible hack
//TODO: some way to do this better
char *vsprintf_dst;

uint32_t vsprintf(char *dst, const char *fmt, __builtin_va_list args) {
        if(!dst) return 0;
        vsprintf_dst = dst;
        return rprintf([](int c) { *vsprintf_dst++ = c; }, fmt, args);
}

uint32_t vprintf(const char *fmt, __builtin_va_list args) {
        return rprintf(&uart_putc, fmt, args);
}

uint32_t rprintf(void(*putc)(int),
                 const char *fmt,
                 __builtin_va_list args) {
        if(!fmt || !putc) return 0;
        uint32_t count = 0;
        int64_t arg = 0;
        int32_t sign, len; //i;
        char *p, tmpstr[19];
        while(*fmt) {
                // argument access
                if(*fmt=='%') {
                        fmt++;
                        // literal %
                        if(*fmt=='%') goto put;
                        len=0;
                        // size modifier
                        while(*fmt>='0' && *fmt<='9') {
                                len *= 10;
                                len += *fmt-'0';
                                fmt++;
                        }
                        // skip long modifier
                        if(*fmt=='l') {
                                fmt++;
                        }
                        // character
                        if(*fmt=='c') {
                                arg = __builtin_va_arg(args, int);
                                putc(arg);
                                count++;
                                fmt++;
                                continue;
                        // decimal number
                        } else if(*fmt=='d') {
                                arg = __builtin_va_arg(args, int);
                                // check input
                                sign = 0;
                                if((int)arg < 0) {
                                        arg *= -1;
                                        sign++;
                                }
                                if(arg>99999999999999999L)
                                        arg=99999999999999999L;
                                // convert to string
                                p = &tmpstr[18];
                                *p = '\0';
                                do {
                                        *(--p) = '0' + (arg % 10);
                                        arg /= 10;
                                } while(arg && p > tmpstr);
                                if(sign) *(--p) = '-';
                                if(len > 0 && len < 18)
                                        while(p > &tmpstr[18 - len])
                                                *(--p) = ' ';
                                goto cpystr;
                        // hex number
                        } else if(*fmt=='x') {
                                arg = __builtin_va_arg(args, long int);
                                p = &tmpstr[16];
                                *p = '\0';
                                do {
                                        char n=arg & 0xf;
                                        *(--p) = n + (n>9?0x37:0x30);
                                        arg >>= 4;
                                } while(arg && p > tmpstr);
                                // padding, only leading zeros
                                if(len>0 && len<=16)
                                        while(p > &tmpstr[16 - len])
                                                *(--p) = '0';
                                goto cpystr;
                        // string
                        } else if(*fmt=='s') {
                                p = __builtin_va_arg(args, char*);
cpystr:
                                if(!p) p = "(null)";
                                while(*p)
                                        putc(*p++), count++;
                        }
                } else
put:
                        putc(*fmt), count++;
                fmt++;
        }
        putc('\0');
        return count;
}
