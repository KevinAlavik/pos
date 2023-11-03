void logger_ok(char* text) {
    serial_printw(SERIAL_PORT, "\033[0;32m[ OK ]\033[0m ");
    serial_printw(SERIAL_PORT, text);
    serial_printw(SERIAL_PORT, "\r\n");
}

void logger_warn(char* text) {
    serial_printw(SERIAL_PORT, "\033[0;33m[ WARN ]\033[0m ");
    serial_printw(SERIAL_PORT, text);
    serial_printw(SERIAL_PORT, "\r\n");
}

void logger_err(char* text) {
    serial_printw(SERIAL_PORT, "\033[0;31m[ ERR ]\033[0m ");
    serial_printw(SERIAL_PORT, text);
    serial_printw(SERIAL_PORT, "\r\n");
}

void logger_dbg(char* text) {
    serial_printw(SERIAL_PORT, "\033[0;37m[ DBG ]\033[0m ");
    serial_printw(SERIAL_PORT, text);
    serial_printw(SERIAL_PORT, "\r\n");
}
