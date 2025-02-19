from ensemble_test import autograder
from parameterized import parameterized
import math

class IsqrtSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()
        
        self.fillMachine(autograder.core.MemoryFillType.Random, 0x2110 ^ 121)
        self.loadFile("integerSquareRoot.asm")
    
    @parameterized.expand([
        0, 1, 4, 10, 20, 36, 40, 45, 49, 60, 64, 100, 10000, 32767,
        15, 35, 48, 99, 120, 360 # make sure student fails a few more test cases for negating a number wrong
    ],
        name_func=autograder.parameterized_name
    )
    def test_integer_square_root(self, n: int):
        """
        (1.2.1) Integer Square Root Test
        """
        self.writeMemValue("N", n)

        # run test
        self.runCode()
        self.assertHalted()

        # check result
        answer_address = self.readMemValue("RESULT")
        answer = math.isqrt(n)
        self.assertMemValue(answer_address, answer)

    def test_integer_square_root_preserve(self):
        """
        (1.2.1) Integer Square Root: Preserve Parameters

        This test checks that no important constant values were overwritten during
        the execution of Integer Square Root.
        """
        # write/save values
        n = 205
        ans = math.isqrt(n)

        orig_answer_address = self.readMemValue("RESULT")
        self.writeMemValue("N", n)

        # Write a known value into memory to check student tried to store to memory
        self.writeMemValue(orig_answer_address, 801)

        # run test
        self.runCode()
        self.assertHalted()

        # check parameters not overridden
        self.assertMemValue("RESULT", orig_answer_address, "Address at mem[RESULT] was overwritten.")
        self.assertMemValue("N", n, "Parameter N was overwritten.")

        # Make sure the student does not get points for submitting empty file
        self.assertMemValue(orig_answer_address, ans, "Result should have updated when executing program")


