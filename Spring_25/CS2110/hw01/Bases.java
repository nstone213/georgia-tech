/**
 * CS 2110 Spring 2025 HW1
 * Part 2 - Coding with bases
 *
 * @author YOUR NAME HERE
 *
 * Global rules for this file:
 * 
 * - You cannot use multiplication, division, and modulus operators
 * - You cannot use nested loops
 * - You cannot declare file-level variables
 * - You cannot use switch statements
 * - You cannot use the unsigned right shift operator (>>>)
 * - You cannot use helper methods, call any other methods, or use recursion.
 * 
 * - You may not use more than 2 conditionals per method. Conditionals are
 *   if-statements, if-else statements, or ternary expressions. The else block
 *   associated with an if-statement does not count toward this sum.
 * - You may not use more than 2 loops per method. This includes
 *   for loops, while loops and do-while loops.
 * - The only Java API methods you are allowed to invoke are:
 *     String.length()
 *     String.charAt()
 * - You may not invoke the above methods from String literals.
 *     Example: "12345".length()
 * - When concatenating numbers with Strings, you may only do so if the number
 *   is a single digit.
 */
public class Bases
{
    /**
     * Convert a String containing ASCII characters (in binary) to an int.
     *
     * You do not need to handle negative numbers. The Strings we will pass in
     * will be valid binary numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: binaryStringToInt("110"); // => 6
     */
    public static int binaryStringToInt(String binary)
    {
        return 0;
    }

    /**
     * Convert a String containing ASCII characters (in decimal) to an int.
     *
     * You do not need to handle negative numbers. The Strings we will pass in
     * will be valid decimal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: decimalStringToInt("46"); // => 46
     */
    public static int decimalStringToInt(String decimal)
    {
        return 0;
    }

    /**
     * Convert a String containing ASCII characters (in octal) to a String containing ASCII characters (in binary).
     * The input String will only contain the numbers 0 through 7.
     *
     * Example: octalStringToBinaryString("52"); // => "101010"
     *
     */

     public static String octalStringToBinaryString(String octal)
     {
        return "";
     }
 
     /**
      * Convert a string containing ASCII characters (in hex) to an int.
      * The input string will only contain numbers and uppercase letters A-F.
      * You do not need to handle negative numbers. The Strings we will pass in will be
      * valid hexadecimal numbers, and able to fit in a 32-bit signed integer.
      *
      * Example: hexStringToInt("A6"); // => 166
      */
     public static int hexStringToInt(String hex)
     {
        return 0;
     }
 
     /**
      * Convert a String containing ASCII characters representing a number in
      * binary into a String containing ASCII characters that represent that same
      * value in hex.
      *
      * The output string should only contain numbers and capital letters.
      * You do not need to handle negative numbers.
      * All binary strings passed in will contain exactly 32 characters.
      * The hex string returned should contain exactly 8 characters.
      *
      * Example: binaryStringToHexString("00001111001100101010011001011100"); // => "0F32A65C"
      * 
      */
     public static String binaryStringToHexString(String binary)
     {
        return "";
     }
 }
 