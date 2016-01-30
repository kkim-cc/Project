#
# shell script for install json related module & compile & run.
# 
# made by Ki-Duck Kim on Jan.30, 2016
#

echo "Welcom. This shell program will install a few programs."
echo "1. JsonCpp module need python for compile."
echo "   would you want to install python(y/n)?"
read answer

if [ $answer == "y" ] ; then

  sudo apt-get install python

else
  echo "This program will continue without python install."
fi

echo "2. JsonCpp module need Scons for compile."
echo "   would you want to install Scons(y/n)?"
read answer

if [ $answer == "y" ] ; then

  sudo apt-get install scons

else
  echo "This program will continue without Scons install."
fi

echo "3. JsonCpp module need to compile."
echo "   would you want to compile using Scons(y/n)?"
read answer

if [ $answer == "y" ] ; then

  cd jsoncpp-src-0.5.0
  scons platform=linux-gcc
  cd ..

else
  echo "This program will continue without JsonCpp build."
fi

echo "4. CodeChallenge module need to compile."
echo "   would you want to compile CodeChallenge(y/n)?"
read answer

if [ $answer == "y" ] ; then

  rm ./obj/*.o
  make -f makeCode

else
  echo "This program will continue without CodeChallenge build."
fi

echo "CodeChallenge program will be running.."
echo "The result file will be created(./Data/result.txt)"
echo "The log file will be created.(./log.txt)"

./code >& log.txt
