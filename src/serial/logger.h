void logger_ok(char* text) {
    serial_printw(SERIAL_PORT, "[ \033[0;32mOK\033[0m ] ");
    serial_printw(SERIAL_PORT, text);
    serial_printw(SERIAL_PORT, "\r\n");
}

void logger_warn(char* text) {
    serial_printw(SERIAL_PORT, "[ \033[0;33mWARN\033[0m ] ");
    serial_printw(SERIAL_PORT, text);
    serial_printw(SERIAL_PORT, "\r\n");
}

void logger_err(char* text) {
    serial_printw(SERIAL_PORT, "[ \033[0;31mERR\033[0m ] ");
    serial_printw(SERIAL_PORT, text);
    serial_printw(SERIAL_PORT, "\r\n");
}

void logger_dbg(char* text) {
    serial_printw(SERIAL_PORT, "[ \033[0;37mDBG\033[0m ] ");
    serial_printw(SERIAL_PORT, text);
    serial_printw(SERIAL_PORT, "\r\n");
}