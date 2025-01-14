/**
 * CS 2110 Spring 2025 HW1
 * Part 1 - Coding a bit vector
 *
 * @author YOUR NAME HERE

 * Global rules for this file:
 * - You may not use multiplication, division or modulus in any method.
 * - You may not use more than 2 conditionals per method, and you may only use
 *   them in select methods. Conditionals are if-statements, if-else statements,
 *   or ternary expressions. The else block associated with an if-statement does
 *   not count toward this sum.
 * - You may not use looping constructs in most methods. Looping constructs
 *   include for loops, while loops and do-while loops. See below for exceptions
 * - In those methods that allow looping, you may not use more than 2 looping
 *   constructs, and they may not be nested.
 * - You may not declare any file-level variables.
 * - You may not declare any objects.
 * - You may not use switch statements.
 * - You may not use casting.
 * - You may not use any String methods (ex. length, substring)
 * - You may not use the unsigned right shift operator (>>>)
 * - You may not write any helper methods, or call any method from this or
 *   another file to implement any method. Recursive solutions are not
 *   permitted.
 * - You may only perform addition or subtraction with the number 1.
 *
 * Method-specific rules for this file:
 * - All methods must be done in one line, except for: onesCount, trailingZerosCount.
 * - Looping and conditionals as described above are only allowed for:
 *   onesCount, trailingZerosCount.
 */
public class BitVector
{
    /**
     * 32-bit data initialized to all zeros. Here is what you will be using to
     * represent the Bit Vector. Do not change its scope from private.
     */
    private int bits;

    /** You may not add any more fields to this class other than the given one. */

    /**
     * Sets the bit (sets to 1) pointed to by index.
     * @param index index of which bit to set.
     *              0 for the least significant bit (right most bit).
     *              31 for the most significant bit.
     */
    public void set(int index)
    {

    }

    /**
     * Clears the bit (sets to 0) pointed to by index.
     * @param index index of which bit to set.
     *              0 for the least significant bit (right most bit).
     *              31 for the most significant bit.
     */
    public void clear(int index)
    {

    }

    /**
     * Toggles the bit (sets to the opposite of its current value) pointed to by
     * index.
     * @param index index of which bit to set.
     *              0 for the least significant bit (right most bit).
     *              31 for the most significant bit.
     */
    public void toggle(int index)
    {

    }

    /**
     * Returns true if the bit pointed to by index is currently set.
     * @param index index of which bit to check.
     *              0 for the least significant bit (right-most bit).
     *              31 for the most significant bit.
     * @return true if the bit is set, false if the bit is clear.
     *         If the index is out of range (index >= 32), then return false.
     */
    public boolean isSet(int index)
    {
        return false;
    }

    /**
     * Returns true if the bit pointed to by index is currently clear.
     * @param index index of which bit to check.
     *              0 for the least significant bit (right-most bit).
     *              31 for the most significant bit.
     * @return true if the bit is clear, false if the bit is set.
     *         If the index is out of range (index >= 32), then return true.
     */
    public boolean isClear(int index)
    {
        return false;
    }

     /**
     * Returns true if the bit vector's high-order 16 bits match the low-order 16 bits. 
     *
     * Ex: 01010101010101010101010101010101 -> true
     * Ex: 00111000001110000011100000111000 -> true
     * Ex: 00100100100100100100100100100100 -> false
     * Ex: 01100010000111010110001000011101 -> true
     * @return true if there is a 16-bit pattern, false if not.
     */
    public boolean isPattern()
    {
        return false;
    }

    /**
     * Returns the number of bits currently set (=1) in this bit vector.
     * You may use the ++ operator to increment your counter.
     * @return the number of ones.
     */
    public int onesCount()
    {
        return 0;
    }

     /**
     * Returns the number of zeroes to the right of the least significant 1 in this bit vector.
     * You may use the ++ operator to increment your counter.
     * 
     * Ex: 10110...1001100000 -> 5
     * Ex: 00010...0101001011 -> 0
     * Ex: 01010...0100000000 -> 8
     * @return the number of trailing zeroes.
     */
    public int trailingZerosCount()
    {
        return 0;
    }
}
