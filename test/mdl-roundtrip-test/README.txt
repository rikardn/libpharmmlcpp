MDL roundtrip test
------------------

A quick and dirty Bash script in order to convert iteratively MDL<->PharmML
and deduce which models pharmml2mdl seems to work for.

Execute in this directory with:
./mdl_roundtrip_test.sh

Pre-requisities:
* MDL (*.mdl) testfiles in 'testfiles' directory. Any subdirectory
  organization within this directory works; Script will find all models
  recursively.
* 'converter-toolbox-distribution' directory. Specifically 'convert.sh'
  located at 'converter-toolbox-distribution/converter-toolbox/convert.sh'
  relative to this directory.

Steps performed by script
-------------------------
0. Create folder 'GIT_HASH.LAST_COMMIT_DATE' for all conversions/results
1. Find all MDL files in 'testfiles'
2. Convert all MDL files to PharmML via DDMoRe converter and report individual
   status of conversions
3. Convert all sucessful PharmML files to MDL via pharmml2mdl and report
   individual status of conversions
4. Repeat step 2 from step 3 models
5. Repeat step 3 from step 4 models
6. Diff PharmML conversions (step 2 and 4) and report
7. Diff MDL conversions (step 3 and 5) and report
8. Summarize number of files that made it through each step
9. Output 'results.csv' table file containing results per each MDL testfile

Note that pretty colors are used -- use a modern terminal!

