// Allow use of `my_sbrk` in solutions.
void *my_sbrk(int increment) {
    (void)increment;
    return (void *)-1;
}