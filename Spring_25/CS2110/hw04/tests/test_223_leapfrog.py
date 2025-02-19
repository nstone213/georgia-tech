from ensemble_test import autograder

class LeapfrogSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()
        
        self.longMessage = False

        self.fillMachine(autograder.core.MemoryFillType.Random, 0x2110 ^ 223)
        self.loadFile("leapfrog.asm")
    
    def test_leapfrog(self):
        """
        (2.2.3) Leapfrog Test
        """
        start = 0x3000
        step = 0x100
        end = 0xFE00
        
        # Copy the program:
        program = []
        for i in range(start, start + step):
            word = self.readMemValue(i)
            program.append(word)

            if word == 0: break
        
        # Validate program size:
        if len(program) <= 1:
            self.fail("Expected program to be non-empty")
        self.assertEqual(program[-1], 0, "Expected program to be less than 256 words")

        self.runCode()
        # Check for leapfrog effect:
        for i in range(start + step, end, step):
            self.assertArray(i, program, f"Expected program to copy itself into address x{i:04X}")