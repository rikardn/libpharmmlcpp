import unittest
import subprocess
import re

class Testpharmmltool(unittest.TestCase):

    def test_usage(self):
        output = subprocess.run(['../pharmmltool'], stdout=subprocess.PIPE)
        l = output.stdout.decode('UTF-8').split("\n")
        m = re.match("Usage", l[0])
        self.assertTrue(m)

    def test_version(self):
        output = subprocess.run(['../pharmmltool', '--version'], stdout=subprocess.PIPE)
        l = output.stdout.decode('UTF-8').split("\n")
        m = re.match("pharmmltool \d+\.\d+$", l[0])
        self.assertTrue(m)

    def test_pharmml_version(self):
        output = subprocess.run(['../pharmmltool', 'version', 'testfiles/glucoseKinetics.xml'], stdout=subprocess.PIPE)
        l = output.stdout.decode('UTF-8').split("\n")
        self.assertEqual(l[0], '0.8.1')
        output = subprocess.run(['../pharmmltool', 'version', 'testfiles/UseCase1.xml'], stdout=subprocess.PIPE)
        l = output.stdout.decode('UTF-8').split("\n")
        self.assertEqual(l[0], '0.9')

    def test_validate(self):
        output = subprocess.run(['../pharmmltool', 'validate', 'testfiles/glucoseKinetics.xml'], stdout=subprocess.PIPE)
        l = output.stdout.decode('UTF-8').split("\n")
        self.assertEqual(l[1], 'Validation successful: YES (result: 0)')
        output = subprocess.run(['../pharmmltool', 'validate', 'testfiles/UseCase1.xml'], stdout=subprocess.PIPE)
        l = output.stdout.decode('UTF-8').split("\n")
        self.assertEqual(l[1], 'Validation successful: YES (result: 0)')

    def test_schema_path(self):
        output = subprocess.run(['../pharmmltool', 'validate', 'testfiles/glucoseKinetics.xml', '--schema-path=../schema/0.8.1'], stdout=subprocess.PIPE)
        l = output.stdout.decode('UTF-8').split("\n")
        self.assertEqual(l[1], 'Validation successful: YES (result: 0)')


if __name__ == '__main__':
    unittest.main()
