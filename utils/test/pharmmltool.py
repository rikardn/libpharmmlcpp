import unittest
import subprocess
import re
import tempfile
import shutil

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

    def test_compact_indent(self):
        with tempfile.TemporaryDirectory() as dirpath:
            shutil.copy("testfiles/UseCase1.xml", dirpath)
            filepath = dirpath + '/UseCase1.xml'
            subprocess.run(['../pharmmltool', 'compact', filepath])
            with open(filepath, 'r') as pharmmlfile:
                content = pharmmlfile.read()
                l = content.split("\n")
                self.assertEqual(len(l), 3)
            subprocess.run(['../pharmmltool', 'indent', filepath])
            with open(filepath, 'r') as pharmmlfile:
                content = pharmmlfile.read()
                l = content.split("\n")
                self.assertTrue(len(l) > 3)

    def test_convert(self):
        with tempfile.TemporaryDirectory() as dirpath:
            shutil.copy("testfiles/glucoseKinetics.xml", dirpath)
            filepath = dirpath + "/glucoseKinetics.xml"
            targetpath = dirpath + "/target.xml"
            # 0.8.1 to default 0.9
            output = subprocess.run(["../pharmmltool", "convert", filepath, targetpath], stdout=subprocess.PIPE)
            l = output.stdout.decode('UTF-8').split("\n")
            m = re.search('0\.8\.1', l[1])
            self.assertTrue(m)
            m = re.search('0\.9', l[2])
            self.assertTrue(m)
            output = subprocess.run(["../pharmmltool", "version", targetpath], stdout=subprocess.PIPE)
            l = output.stdout.decode('UTF-8').split("\n")
            self.assertEqual(l[0], '0.9')
            output = subprocess.run(['../pharmmltool', 'validate', targetpath], stdout=subprocess.PIPE)
            l = output.stdout.decode('UTF-8').split("\n")
            self.assertEqual(l[1], 'Validation successful: YES (result: 0)')

            # 0.6.1 to 0.8.1
            shutil.copy("testfiles/Executable_Krippendorff_ModelB.xml", dirpath)
            filepath = dirpath + "/Executable_Krippendorff_ModelB.xml"
            targetpath = dirpath + "/target.xml"
            output = subprocess.run(["../pharmmltool", "convert", filepath, targetpath, '--target-version=0.8.1'], stdout=subprocess.PIPE)
            l = output.stdout.decode('UTF-8').split("\n")
            m = re.search('0\.6\.1', l[1])
            self.assertTrue(m)
            m = re.search('0\.8\.1', l[2])
            self.assertTrue(m)
            output = subprocess.run(["../pharmmltool", "version", targetpath], stdout=subprocess.PIPE)
            l = output.stdout.decode('UTF-8').split("\n")
            self.assertEqual(l[0], '0.8.1')
            output = subprocess.run(['../pharmmltool', 'validate', targetpath], stdout=subprocess.PIPE)
            l = output.stdout.decode('UTF-8').split("\n")
            self.assertEqual(l[1], 'Validation successful: YES (result: 0)')

            # 0.6.1 to 0.9
            output = subprocess.run(["../pharmmltool", "convert", filepath, targetpath, '--target-version=0.9'], stdout=subprocess.PIPE)
            l = output.stdout.decode('UTF-8').split("\n")
            m = re.search('0\.6\.1', l[1])
            self.assertTrue(m)
            m = re.search('0\.9', l[2])
            self.assertTrue(m)
            output = subprocess.run(["../pharmmltool", "version", targetpath], stdout=subprocess.PIPE)
            l = output.stdout.decode('UTF-8').split("\n")
            self.assertEqual(l[0], '0.9')
            output = subprocess.run(['../pharmmltool', 'validate', targetpath], stdout=subprocess.PIPE)
            l = output.stdout.decode('UTF-8').split("\n")
            self.assertEqual(l[1], 'Validation successful: YES (result: 0)')


if __name__ == '__main__':
    unittest.main()
