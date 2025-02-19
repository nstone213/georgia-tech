from ensemble_test import autograder
from parameterized import parameterized
import random
import string

random.seed(2110 ^ 123)

class RevStrSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()
        
        self.fillMachine(autograder.core.MemoryFillType.Random, 0x2110 ^ 123)
        self.loadFile("reverseString.asm")
    
    @parameterized.expand([
        "gOOglE", # Example test case
        "Reverse This",
        "CS Twenty One tEN",
        "hELlo",
        "gORGIa tECh",
        "WolleY StekcaJ",
        ''.join(random.choices(string.ascii_letters + ' ', k=100))
    ],
        name_func=lambda fn, n, param: f"{fn.__name__}_{n}"
    )
    def test_reverse_string(self, string):
        """
        (1.2.3) Reverse String Test
        """
        string_addr = self.readMemValue("STRING")
        self.writeString(string_addr, string)

        self.runCode()
        self.assertHalted()

        reversed_str = string[::-1]

        self.assertString(string_addr, reversed_str)

    def test_reverse_string_preserve(self):
        """
        (1.2.3) Reverse String: Preserve Parameters

        This test checks that no important constant values were overwritten during
        the execution of Reverse String.
        """
        string = "gOOglE"
        # get original values
        string_addr = self.readMemValue("STRING")
        # write parameters
        self.writeString(string_addr, string)

        # run
        self.runCode()
        self.assertHalted()
        
        # assert values unchanged
        self.assertMemValue("STRING", string_addr, "Address at mem[STRING] was unexpectedly overwritten during execution")
        
        # check result was actually updated:
        self.assertString(string_addr, string[::-1])
