__attribute__((interrupt)) void division_by_zero_error(void*) {
    logger_err("Division by zero!");
    hcf();
}

void init_os_interupts() {
    set_idt_gate(0, (uint64_t)&division_by_zero_error, 0x28, 0x8E);
}