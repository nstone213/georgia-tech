from ensemble_test import autograder
from parameterized import parameterized
import random

random.seed(2110 ^ 124)

class MinIndexSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()

        self.fillMachine(autograder.core.MemoryFillType.Random, 0x2110 ^ 124)
        self.loadFile("findMinIndex.asm")

    @parameterized.expand([
        [[-1, 2, 7, 3, -8]], # Example test case
        [[1, 2, 3, 4, 5, 6]],
        [[100, 99, 98, 97, 96]],
        [[10, 100, 2, 31, 99]],
        [[2, 8, -11, 32, -28]],
        [[random.randint(-2110, 2110) for i in range(20)]],
        [[random.randint(-2110, 2110) for i in range(100)]]
    ])
    def test_find_min_index(self, array):
        """
        (1.2.4) Find Minimum Index Test
        """
        self.writeMemValue("LENGTH", len(array))

        array_address = self.readMemValue("ARRAY")
        self.writeArray(array_address, array)

        self.runCode()
        self.assertHalted()

        answer_address = self.readMemValue("RESULT")
        self.assertMemValue(answer_address, array.index(min(array)))

    def test_find_min_index_preserve(self):
        """
        (1.2.4) Find Minimum Index: Preserve Parameters

        This test checks that no important constant values were overwritten during
        the execution of Find Minimum Index.
        """
        array = [-1, 2, 7, 3, -8]
        
        # get original values
        orig_array = self.readMemValue("ARRAY")
        orig_result = self.readMemValue("RESULT")

        # write parameters
        self.writeMemValue("LENGTH", len(array))
        self.writeArray(orig_array, array)
        self.writeMemValue(orig_result, 0x221)

        # run
        self.runCode()
        self.assertHalted()

        # assert values unchanged
        self.assertMemValue("ARRAY", orig_array, "Address at mem[ARRAY] was unexpectedly overwritten during execution")
        self.assertMemValue("RESULT", orig_result, "Address at mem[RESULT] was unexpectedly overwritten during execution")
        self.assertMemValue("LENGTH", len(array), "Value at mem[LENGTH] was unexpectedly overwritten during execution")
        
        # check result was actually updated:
        self.assertMemValue(orig_result, 4, "Result should have updated when executing program")
