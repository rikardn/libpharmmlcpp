#!/usr/bin/python3

import unittest
import subprocess
import re
import tempfile
import shutil
import os

def testfile_path(testfile):
    return os.path.dirname(os.path.realpath(__file__)) + "/testfiles/" + testfile

def exe_path(exe):
    if os.path.isfile("./" + exe):
        return "./" + exe
    else:
        return "../" + exe

class Testpharmmltool(unittest.TestCase):

    def test_usage(self):
        output = subprocess.run([exe_path('sotool')], stdout=subprocess.PIPE)
        l = output.stdout.decode('UTF-8').split("\n")
        m = re.match("Usage", l[0])
        self.assertTrue(m)

    def test_version(self):
        output = subprocess.run([exe_path('sotool'), '--version'], stdout=subprocess.PIPE)
        l = output.stdout.decode('UTF-8').split("\n")
        m = re.match("sotool \d+\.\d+$", l[0])
        self.assertTrue(m)

    def test_so_version(self):
        output = subprocess.run([exe_path('sotool'), 'version', testfile_path('pheno_real.SO.xml')], stdout=subprocess.PIPE)
        l = output.stdout.decode('UTF-8').split("\n")
        self.assertEqual(l[0], '0.3.1')

    def test_validate(self):
        output = subprocess.run([exe_path('sotool'), 'validate', testfile_path('pheno_real.SO.xml')], stdout=subprocess.PIPE)
        l = output.stdout.decode('UTF-8').split("\n")
        self.assertEqual(l[1], 'Validation successful: YES (result: 0)')

    #def test_schema_path(self):
    #    output = subprocess.run([exe_path('pharmmltool'), 'validate', testfile_path('glucoseKinetics.xml'), '--schema-path=../schema/0.8.1'], stdout=subprocess.PIPE)
    #    l = output.stdout.decode('UTF-8').split("\n")
    #    self.assertEqual(l[1], 'Validation successful: YES (result: 0)')

    def test_compact_indent(self):
        with tempfile.TemporaryDirectory() as dirpath:
            shutil.copy(testfile_path("pheno_real.SO.xml"), dirpath)
            filepath = dirpath + '/pheno_real.SO.xml'
            subprocess.run([exe_path('sotool'), 'compact', filepath])
            with open(filepath, 'r') as pharmmlfile:
                content = pharmmlfile.read()
                l = content.split("\n")
                self.assertEqual(len(l), 3)
            subprocess.run([exe_path('sotool'), 'indent', filepath])
            with open(filepath, 'r') as sofile:
                content = sofile.read()
                l = content.split("\n")
                self.assertTrue(len(l) > 3)

if __name__ == '__main__':
    unittest.main()
