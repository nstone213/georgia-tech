import string

from ensemble_test import autograder
from parameterized import parameterized

def encrypt(plaintext: str, k: int):
    '''
    Performs a Caesar Cipher on a string with a shift of value k, returns shifted string
    '''
    # The ord() method looked super ugly to me, so we're going to use string translation
    k %= 26

    # create our shifted alphabets
    shifted_lower = string.ascii_lowercase[k:] + string.ascii_lowercase[:k]
    shifted_upper = string.ascii_uppercase[k:] + string.ascii_uppercase[:k]

    # make translation table
    original_table = string.ascii_lowercase + string.ascii_uppercase
    shifted_table = shifted_lower + shifted_upper
    translation_table = str.maketrans(original_table, shifted_table)

    # do the translation
    return plaintext.translate(translation_table)

class CaesarSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()
        
        self.fillMachine(autograder.core.MemoryFillType.Random, 0x2110 ^ 221)
        self.loadFile("caesarCipher.asm")

        # define the subroutines
        self.defineSubroutine("MOD", ["a", "b"])
        self.defineSubroutine("ENCRYPT", ["str", "k"])

    @parameterized.expand([
        (0, 5), (4, 4), # test a <= b
        (300, 2), (301, 2), # test with even/odd and b = 2
        (514, 7), (32767, 4), (802, 16), # some random ones
        (3000, 77), (4000, 144) # some bigger b
    ],
        name_func=autograder.parameterized_name
    )
    def test_mod_solution(self, a: int, b: int):
        """
        (2.2.1) Mod: Correct Solution

        This test checks that MOD yielded the correct answer.
        """
        # call the subroutine
        self.callSubroutine("MOD", [a, b])

        # make sure we have the correct retun
        self.assertReturned()
        self.assertReturnValue(a % b)

    def test_mod_calling_convention(self):
        """
        (2.2.1) Mod: Calling Convention

        This test checks that standard LC-3 calling convention 
        was correctly followed when calling MOD.
        """
        a = 514
        b = 7
        # call the subroutine
        self.callSubroutine("MOD", [a, b])

        # followed calling convention
        self.assertReturned()
        self.assertStackCorrect()
        self.assertRegsPreserved()

    @parameterized.expand([
        ("hello", 7), ("hello", 33), ("jAckEts", 1), # pdf test cases

        # Test all the lowercase letters
        ("abcdefghijklmnopqrstuvwxyz", 5), ("abcdefghijklmnopqrstuvwxyz", 25),
        ("abcdefghijklmnopqrstuvwxyz", 35), ("abcdefghijklmnopqrstuvwxyz", 51),

        # Do that again with all the uppercase letters
        ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 5), ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 25),
        ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 35), ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 51),

        # Other test cases
        ("winning", 1), ("wInNninG", 22), ("NOPEE", 15),
        ("ImLatee", 36), ("TaKingToOloNG", 40),

        # Decipher some names, expected should be Juliette, Henry, Angel, Allyson
        ("Ozqnjyyj", 21), ("Roxbi", 16), ("Dqjho", 49), ("Paanhdc", 37),

        # Decipher some places, expected should be France, Spain, Germany
        ("Senapr", 39), ("Wtemr", 48), ("Ywjesfq", 34)
    ],
        name_func=lambda fn, n, param: f"{fn.__name__}_{n}"
    )
    def test_caesar_shift_solution(self, plaintext: str, k: int):
        """
        (2.2.1) Caesar Shift: Correct Solution

        This test checks that the string produced by ENCRYPT
        is correct.
        """
        # write the test case into memory
        string_addr = self.readMemValue("STRING")
        self.writeString(string_addr, plaintext)
        self.writeMemValue("SHIFT", k)

        # call the subroutine
        self.callSubroutine("ENCRYPT", [string_addr, k])

        # make sure we came back
        self.assertReturned()

        # check the solution
        self.assertString(self.readMemValue("STRING"), encrypt(plaintext, k))

    def test_caesar_shift_calling_convention(self):
        """
        (2.2.1) Caesar Shift: Calling Convention

        This test checks that standard LC-3 calling convention 
        was correctly followed when calling ENCRYPT.
        """
        plaintext = "wInNninG"
        k = 22

        string_addr = self.readMemValue("STRING")
        self.writeString(string_addr, plaintext)

        # set memory to garbage values to force students to properly handle arguments
        self.writeMemValue("STRING", 0xFFFF)
        self.writeMemValue("SHIFT", 0xFFFF)

        # call the subroutine
        # access violation here may imply that the student is
        # trying to access program values and not loading from arguments
        self.callSubroutine("ENCRYPT", [string_addr, k])

        # make sure the student followed the calling convention
        self.assertReturned()
        self.assertStackCorrect()
        self.assertRegsPreserved()

    """
    Check that no important constants were changed during execution
    """
    def test_caesar_shift_preserve(self):
        """
        (2.2.1) Caesar Shift: Preserve Parameters

        This test checks that no important constant values were overwritten during
        the execution of Caesar Shift.
        """
        # write the test case into memory
        test_str = "testedAll"
        test_shift = 9

        string_addr = self.readMemValue("STRING")
        self.writeString(string_addr, test_str)
        self.writeMemValue("SHIFT", test_shift)

        # call the subroutine
        self.callSubroutine("ENCRYPT", [string_addr, 9])

        # make sure we came back and nothing has been broken
        self.assertReturned()
        self.assertStackCorrect()
        self.assertRegsPreserved()

        # make sure nothing was overwritten
        self.assertMemValue("STRING", 0x4000, "Address at mem[STRING] was overwritten during execution")
        self.assertMemValue("SHIFT", test_shift, "Parameter SHIFT was overwritten during execution")

        # still make sure student got the right answer
        self.assertString(string_addr, encrypt(test_str, 9))
