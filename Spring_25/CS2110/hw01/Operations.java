/**
 * CS 2110 Spring 2025 HW1
 * Part 3 - Coding with bitwise operators
 *
 * @author YOUR NAME HERE
 *
 * Global rules for this file:
 * - All of these functions must be completed in ONE line. That means they
 *   should be of the form "return [...];". No partial credit will be awarded
 *   for any method that isn't completed in one line.
 *
 * - You may not use conditionals.
 * - You may not declare any variables.
 * - You may not use casting.
 * - You may not use the unsigned right shift operator (>>>)
 * - You may not write any helper methods, or call any method from this or
 *   another file to implement any method. Recursive solutions are not
 *   permitted.
 * - You may not use addition or subtraction. However, you may use the
 *   unary negation operator (-) to negate a number (except in changeSign).
 * - You may not use multiplication, division or modulus.
 *
 * - Basically, the global rules are you can only use logical (&&, ||) and
 *   bitwise (&, |, ^, >>, <<, ~) operators, as well as the unary negation (-)
 *   operator, all in one line.
 *
 * Method-specific rules for this file:
 * - You may use addition with the number 1 only in changeSign.
 * - You may not use unary negation in changeSign.
 * - You may not use bit shifting or the exclusive OR operator (^) in xor.
 * - You may use subtraction in powerOf2 ONLY.
 * 
 * Some notes:
 * All of these functions accept ints as parameters because if you pass in a
 * number (which is of type int by default) into a Method accepting a byte, then
 * the Java compiler will complain even if the number would fit into that type.
 *
 * Now, keep in mind the return value is also an int. Please read the comments
 * about how many significant bits to return and make sure that the other bits
 * are not set or else you will not get any points for that test case. For
 * example, if we ask you to return 6 bits and you return "0xFFFFFFFF", you will
 * lose points.
 *
 * Definitions of types:
 * nibble - 4 bits
 * byte   - 8 bits
 * short  - 16 bits
 * int    - 32 bits
 */
public class Operations
{

    /**
     * Reverse the sign of an int.
     * For this method, you may NOT use unary negation (-).
     * Hint: Think 2's Complement.
     *
     * @param num The int to reverse the sign of.
     *
     * @return The int with the sign reversed
     */
    int changeSign(int num)
    {
        return 0;
    }


     /**
     * Get an 8-bit byte from an int.
     *
     * Ints are made of 4 bytes, numbered like so:
     *   |   B3   |   B2   |   B1   |   B0   |
     *
     * For a graphical representation of the bits:
     *
     * bit 31                             bit 0
     *    v                                 v
     *    11011000 00001100 00011111 11011001
     *   +--------+--------+--------+--------+
     *   |   B3   |   B2   |   B1   |   B0   |
     *
     * Examples:
     *     getByte(0x56781234, 0); // => 0x34
     *     getByte(0x56781234, 2); // => 0x78
     *
     * Note: Remember, no multiplication allowed!
     *
     * @param num The int to get a byte from.
     * @param which Determines which byte gets returned - 0 for
     *              least-significant byte, going to 3 for the most significant byte.
     *
     * @return A byte corresponding to the "which" parameter from num.
     */
    int getByte(int num, int which)
    {
        return 0;
    }

    /**
     * Move the bytes of an int so that they are each shifted leftward one slot and
     * the most significant (most leftward) byte becomes the least significant (most rightward)
     * 
     * Visually:
     * 
     *    11111111 10001000 11010111 01010101 
     *   +--------+--------+--------+--------+
     *   |   B3   |   B2   |   B1   |   B0   |
     * 
     *   becomes
     * 
     *    10001000 11010111 01010101 11111111
     *   +--------+--------+--------+--------+
     *   |   B2   |   B1   |   B0   |   B3   |
     * 
     * 
     * 
     * @param num
     * @return The int with the bytes moved leftward circularly
     */
    int circularLeftShiftBytes(int num) 
    {
        return 0;
    }


    /**
     * Place a short within another short.
     * 
     * The short being inserted (s1) should be placed between the two bytes of the other short (s2)
     * 
     * Examples:
     * 
     * insertShort(0xAAAA, 0x2110); => 0xAA2110AA
     * insertShort(0x1278, 0x3456); => 0x12345678
     * 
     * @param s1
     * @param s2
     * @return a 32-bit value containing one short 
     */
    int insertShort(int s1, int s2)
    {
        return 0;
    }

    /**
     * Extract a range of bits from a number.
     *
     * Examples:
     *     bitRange(0x00001234, 0, 4);  // => 0x00000004
     *     bitRange(0x00001234, 4, 8);  // => 0x00000023
     *     bitRange(0x12345678, 4, 22); // => 0x00234567
     *     bitRange(0x55555555, 5, 7);  // => 0x0000002A
     * 
     * Visually:
     * 
     *   bitRange(0x12345678, 4, 22); // => 0x02345678
     * 
     *   0x1  0x2  0x3  0x4  0x5  0x6  0x7  0x8
     *   0000 0010 0011 0100 0101 0110 0111 1000
     *          ^                         ^
     *        bit 25 (4 + 22 - 1)       bit 4
     * 
     *   return 10 0011 0100 0101 0110 0111 => 0x02345678
     * 
     *
     * Note: We will only pass in values 1 to 32 for n.
     *
     * @param num An n-bit 2's complement number.
     * @param s The starting bit to grab
     * @param n The number of bits to return.
     * @return The n-bit number num[s:s+n-1].
     */
    int bitRange(int num, int s, int n)
    {
        return 0;
    }

    /**
     * NOTE: For this method, you may only use &, |, and ~.
     *
     * Perform an exclusive-or on two 32-bit ints.
     *
     * Examples:
     *     xor(0xFF00FF00, 0x00FF00FF); // => 0xFFFFFFFF
     *     xor(0x12345678, 0x87654321); // => 0x95511559
     *
     * @param num1 An int
     * @param num2 Another int
     *
     * @return num1 XOR num2
     */
    int xor(int num1, int num2)
    {
        return 0;
    }

    /**
     * Return true if the given number is a multiple of 2.
     *
     * Examples:
     *     multipleOf2(32);   // => true
     *     multipleOf2(13);   // => false
     *
     * Note: Make sure you handle ALL the cases!
     *
     * @param num a 32-bit int. Since this is an int, it is SIGNED!
     * @return true if num is a multiple of 2, else false.
     */

    boolean multipleOf2(int num)
    {
        return false;
    }

    /**
     * Return true if the given number is a multiple of 64.
     *
     * Examples:
     *     multipleOf64(576); // => true
     *     multipleOf64(19); // => false
     *     multipleOf64(65);  // => false
     *     multipleOf64(-256);  // => true
     *     multipleOf64(0);  // => true
     *
     * Note: Make sure you handle ALL the cases!
     *
     * @param num a 32-bit int. Since this is an int, it is SIGNED!
     * @return true if num is a multiple of 64, else false.
     */

     boolean multipleOf64(int num)
     {
        return false;
     }

    /**
     * Return true if the given number is a power of 2.
     *
     * Examples:
     *     powerOf2(32);   // => true
     *     powerOf2(12);   // => false
     *     powerOf2(1);    // => true
     *     powerOf2(-4);   // => false (a negative power of 2 is not a power of 2)
     *
     * Note: Make sure you handle ALL the cases!
     *
     * Hint: If num is a power of 2:
     *   What is the binary representation of num?
     *   What is the binary representation of num - 1?
     *   How does it differ if num is not a power of 2?
     *
     * @param num a 32-bit int. Since this is an int, it is SIGNED!
     * @return true if num is a multiple of 2, else false.
     */

    boolean powerOf2(int num)
    {
        return false;
    }
}
