from ensemble_test import autograder
from parameterized import parameterized
import random

random.seed(2110 ^ 222)

class MergeSortSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()
        
        self.fillMachine(autograder.core.MemoryFillType.Random, 0x2110 ^ 222)
        self.loadFile("mergeSort.asm")

        self.defineSubroutine("DIVIDE", ["a", "b"])
        self.defineSubroutine("MERGE", ["arr", "buf", "start", "mid", "end"])
        self.defineSubroutine("MERGESORT", ["array", "buffer", "left", "right"])

    @parameterized.expand([
        (0, 5), (4, 4), # test a <= b
        (300, 2), (301, 2), # test with even/odd and b = 2
        (514, 7), (32767, 4), (802, 16), # some random ones
        (3000, 77), (4000, 144) # some bigger b
    ],
        name_func=autograder.parameterized_name
    )
    def test_divide_solution(self, a: int, b: int):
        """
        (2.2.2) Divide: Correct Solution

        This test checks that DIVIDE yielded the correct answer.
        """
        # call the subroutine
        self.callSubroutine("DIVIDE", [a, b])

        # make sure our return value is correct
        self.assertReturned()
        self.assertReturnValue(a // b)

    def test_divide_calling_convention(self):
        """
        (2.2.2) Divide: Calling Convention

        This test checks that standard LC-3 calling convention 
        was correctly followed when calling DIVIDE.
        """
        a = 514
        b = 7
        # call the subroutine
        self.callSubroutine("DIVIDE", [a, b])

        # make sure we came back and didn't break anything
        self.assertReturned()
        self.assertStackCorrect()
        self.assertRegsPreserved()

    @parameterized.expand([
        [[2, 3, 1, 8, 4], 0, 2, 4],
        [[5, 6, 7, 1, 2, 3, 4], 0, 3, 7],
        [[1, 5, 8, 9, 3, 4, 7, 2], 1, 4, 7],
        [[-603, 1666, -1955, -395, 483, -1435, -1243, 1197,  -1187, -859], 2, 5, 7],
        [[-232, 776, -1217, 1541, -114, 86, -1061, -460, -187], 2, 4, 6]
    ])
    def test_merge_solution(self, array: "list[int]", start: int, mid: int, end: int):
        """
        (2.2.2) Merge: Correct Solution

        This test checks that the array produced by MERGE
        is correct.
        """
        array_address = self.readMemValue("ARRAY")
        self.writeMemValue("LENGTH", len(array))
        self.writeArray(array_address, array)
        buffer_address = self.readMemValue("BUF")

        self.callSubroutine("MERGE", [array_address, buffer_address, start, mid, end])
        self.assertReturned()

        # Assert correctly merged:
        merged_array = list(array)
        merged_array[start:end] = sorted(merged_array[start:end])
        self.assertArray(array_address, merged_array)
    
    def test_merge_calling_convention(self):
        """
        (2.2.2) Merge: Calling Convention

        This test checks that standard LC-3 calling convention 
        was correctly followed when calling MERGE.
        """
        array = [1, 5, 8, 9, 3, 4, 7, 2]
        start = 1
        mid = 4
        end = 7

        array_address = self.readMemValue("ARRAY")
        buffer_address = self.readMemValue("BUF")

        # set memory to garbage values to force students to properly handle arguments
        self.writeMemValue("ARRAY", 0xFFFF)
        self.writeMemValue("BUF", 0xFFFF)
        self.writeMemValue("LENGTH", 0xFFFF)
        
        # write arguments
        self.writeArray(array_address, array)
        
        # call the subroutine
        # access violation here may imply that the student is
        # trying to access program values and not loading from arguments
        self.callSubroutine("MERGE", [array_address, buffer_address, start, mid, end])

        # make sure the student followed the calling convention
        self.assertReturned()
        self.assertStackCorrect()
        self.assertRegsPreserved()

    @parameterized.expand([
        [[2, 3, 1, 8, 4]],
        [[1, 2, 3, 4, 5, 6, 7]],
        [[random.randint(-2110, 2110) for _ in range(20)]],
        [[random.randint(-2110, 2110) for _ in range(50)]]
    ])
    def test_merge_sort_solution(self, array: "list[int]"):
        """
        (2.2.2) Merge Sort: Correct Solution

        This test checks that the array produced by MERGESORT
        is correct.
        """
        array_address = self.readMemValue("ARRAY")
        self.writeMemValue("LENGTH", len(array))
        self.writeArray(array_address, array)
        buffer_address = self.readMemValue("BUF")

        self.callSubroutine("MERGESORT", [array_address, buffer_address, 0, len(array)])
        self.assertReturned()

        # Assert merge sort is done recursively:
        end = len(array)
        mid = end // 2
        self.assertSubroutineCalled("MERGESORT", [array_address, buffer_address, 0, mid], directly_called=True)
        self.assertSubroutineCalled("MERGESORT", [array_address, buffer_address, mid, end], directly_called=True)
        self.assertSubroutineCalled("MERGE", [array_address, buffer_address, 0, mid, end], directly_called=True)
        
        # Assert correctly sorted:
        self.assertArray(array_address, sorted(array))
    
    @parameterized.expand([
        [[2, 3, 1, 8, 4]],
        [[1, 2, 3, 4, 5, 6, 7]]
    ])
    def test_merge_sort_calling_convention(self, array: "list[int]"):
        """
        (2.2.2) Merge Sort: Calling Convention

        This test checks that standard LC-3 calling convention 
        was correctly followed when calling MERGESORT.
        """
        array_address = self.readMemValue("ARRAY")
        buffer_address = self.readMemValue("BUF")

        # set memory to garbage values to force students to properly handle arguments
        self.writeMemValue("ARRAY", 0xFFFF)
        self.writeMemValue("BUF", 0xFFFF)
        self.writeMemValue("LENGTH", 0xFFFF)
        
        # write arguments
        self.writeArray(array_address, array)
        
        # call the subroutine
        # access violation here may imply that the student is
        # trying to access program values and not loading from arguments
        self.callSubroutine("MERGESORT", [array_address, buffer_address, 0, len(array)])

        # make sure the student followed the calling convention
        self.assertReturned()
        self.assertStackCorrect()
        self.assertRegsPreserved()

    def test_merge_sort_preserve(self):
        """
        (2.2.2) Merge Sort: Preserve Parameters

        This test checks that no important constant values were overwritten during
        the execution of Merge Sort.
        """
        array = [1, 8, 4, 14, 7]

        array_address = self.readMemValue("ARRAY")
        buffer_address = self.readMemValue("BUF")

        self.writeMemValue("LENGTH", len(array))
        self.writeArray(array_address, array)

        # call the subroutine
        self.callSubroutine("MERGESORT", [array_address, buffer_address, 0, len(array)])

        # make sure we came back and nothing has been broken
        self.assertReturned()
        self.assertStackCorrect()
        self.assertRegsPreserved()

        self.assertMemValue("ARRAY", array_address, "Address at mem[ARRAY] was unexpectedly overwritten during execution.")
        self.assertMemValue("BUF", buffer_address, "Address at mem[BUF] was unexpectedly overwritten during execution.")
        self.assertMemValue("LENGTH", len(array), "Value at LENGTH was unexpectedly overwritten during execution.")

        # Check program changed:
        self.assertArray(array_address, sorted(array), "Result should have updated when executing program")

