from ensemble_test import autograder
from parameterized import parameterized
import random

random.seed(2110 ^ 122)

class BinStrToIntSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()

        self.fillMachine(autograder.core.MemoryFillType.Random, 0x2110 ^ 122)
        self.loadFile("binaryStringToInt.asm")
    
    @parameterized.expand([
        "11001",
        *(''.join(random.choices('01', k=random.randint(1, 16))) for _ in range(10))
    ],
        name_func=autograder.parameterized_name
    )
    def test_binary_string_to_int(self, string):
        """
        (1.2.2) Binary String To Int Test
        """
        string_address = self.readMemValue("BINARYSTRING")
        self.writeString(string_address, string)

        self.runCode()
        self.assertHalted()

        correct_answer = int(string, 2)
        result_address = self.readMemValue("RESULTADDR")

        self.assertMemValue(result_address, correct_answer)
    
    def test_binary_string_to_int_preserve(self):
        """
        (1.2.2) Binary String To Int: Preserve Parameters

        This test checks that no important constant values were overwritten during
        the execution of Binary String To Int.
        """

        binary_string = "10011"
        answer = int(binary_string, 2)

        # get the original values
        string_address = self.readMemValue("BINARYSTRING")
        result_addr = self.readMemValue("RESULTADDR")

        # write them
        self.writeString(string_address, binary_string)

        # run it
        self.runCode()
        self.assertHalted()

        # assert values unchanged
        self.assertMemValue("BINARYSTRING", string_address, "Address at mem[BINARYSTRING] was unexpectedly overwritten during execution.")
        self.assertMemValue("RESULTADDR", result_addr, "Address at mem[RESULTADDR] was unexpectedly overwritten during execution.")
        
        # validate correct answer
        self.assertMemValue(result_addr, answer, "Result should have updated when executing program")
