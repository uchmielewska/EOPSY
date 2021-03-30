#!/bin/bash

echo "Creating directory tree ..."

mkdir TestFile
cd TestFile
mkdir documents
mkdir study
mkdir photos
touch cv.txt
touch cv2
mkdir documents/Asia
touch documents/Asia/les*son1
touch documents/Asia/lesson2
touch documents/lesson0.txt
mkdir documents/Hania
mkdir photos/holidays
mkdir photos/holidays/croatia
touch photos/holidays/croatia/familyPhoto.jpg
mkdir photos/camp
touch photos/camp/myGroup
mkdir study/sem6
touch study/sem6/passingRules
touch study/sem6/labolatory
mkdir study/sem5
mkdir study/'Future_Work'
touch study/'Future_Work'/My*cV

ls -R

echo "**************************************** Correct usage examples ************************************"
echo "================================================ Test 1 ==============================================="
echo "\nRunning command: ../modify -u -r documents photos/camp"
../modify.sh -u -r documents photos/camp

echo "\n--------------------------------------Directory tree after test 1-------------------------------------"
ls -R
echo ""
echo "Press any key for continue testing"
read pause

echo "================================================ Test 2 ==============================================="
echo "\nRunning command: ../modify -rl documents/Asia"
../modify.sh -rl documents/Asia

echo "\n--------------------------------------Directory tree after test 2-------------------------------------"
ls -R
echo ""
echo "Press any key for continue testing"
read pause

echo "================================================ Test 3 ==============================================="
echo "\nRunning command: ../modify.sh -u cv.txt cv2"
../modify.sh -u cv.txt cv2

echo "\n--------------------------------------Directory tree after test 3-------------------------------------"
ls -R
echo ""
echo "Press any key for continue testing"
read pause

echo "================================================ Test 4 ==============================================="
echo "\nRunning command: ../modify 'y/l/Z/' -r study photos"
../modify.sh 'y/l/Z/' -r study photos

echo "\n--------------------------------------Directory tree after test 4-------------------------------------"
ls -R
echo ""
echo "Press any key for continue testing"
read pause

echo "================================================ Test 5 ==============================================="
echo "\nRunning command: ../modify -l CV.txt"
../modify.sh -l CV.txt

echo "\n--------------------------------------Directory tree after test 5-------------------------------------"
ls -R
echo ""
echo "Press any key for continue testing"
read pause

echo "================================================ Test 6 ==============================================="
echo "\nRunning command: ../modify 's/a/HELLO/' -r study"
../modify.sh 's/a/HELLO/' -r  study

echo "\n--------------------------------------Directory tree after test 6-------------------------------------"
ls -R
echo ""
echo "Press any key for continue testing"
read pause


echo "**************************************** Incorrect usage examples ***********************************"
echo "================================================ Test 1 ==============================================="
echo ""
echo "-----After each incorrect usage program prints help-----"
echo ""

echo "\nRunning command: ../modify.sh -ug -r documents"
echo ""
../modify.sh -ug -r documents

echo ""
echo "Press any key for continue testing"
read pause

echo "================================================ Test 2 ==============================================="
echo "\nRunning command: ../modify.sh -u -l documents"
echo ""
../modify.sh -u -l documents

echo ""
echo "Press any key for continue testing"
read pause

echo "================================================ Test 3 ==============================================="
echo "\nRunning command: ../modify.sh -u documents cv.txt"
echo ""
../modify.sh -u documents cv.txt

echo ""
echo "Press any key for continue testing"
read pause

echo "================================================ Test 4 ==============================================="
echo "\nRunning command: ../modify.sh -u -r lablabla"
echo ""
../modify.sh -u -r lablabla

echo ""
echo "Press any key for continue testing"
read pause

echo "================================================ Test 5 ==============================================="
echo "\nRunning command: ../modify.sh -r 's/[A-Z]/f' study"
echo ""
../modify.sh -r 's/[A-Z]/G' study

echo ""
echo "Press any key for continue testing"
read pause

echo "**************************************** End of script ***********************************"
echo "**************************************** TestFile deleted ***********************************"
cd ..
rm -r TestFile
