#!/bin/bash

# This script is designed for local unix usage.
# ./gui.sh --build

#########
# UTILS #
#########

create_clean_directory(){
  dir_name=$1
  if [ -d "$dir_name" ]; then
    echo "Removing $dir_name"
    rm -rf "$dir_name"
  elif [ -f "$dir_name" ]; then
    echo "File with this name already exists, not a directory."
    exit
  fi
  if mkdir "$dir_name"; then
    echo "Clean directory created: $dir_name"
    return 0
  else
    echo "Creating directory failed: $dir_name"
    return 1
  fi 
}


#########
# SETUP #
#########

# Check if Darwin or Linux
PLATFORM="$(uname -s)"
if [ $PLATFORM != "Darwin" ] && [ $PLATFORM != "Linux" ]; then
  echo "Error: This script can only run on 'Darwin' or 'Linux'. You passed $PLATFORM."
  exit 1
fi

set -e

########
# HELP #
########

if [ "$1" == "--help" ] ; then
cat << EOF
--help               Display this dialog
--install            Install dependencies for your platform [$PLATFORM]
--build              Build the gui 

--test               Test the the existing build in the ./build folder
--test xvfb          [Linux only!] Same as above, but run tests in xvfb

--build-n-test       Build to the ./build folder and run test suites
--build-n-test xvfb  [Linux only!] Same as above, but run tests in xvfb
EOF
fi

###########
# INSTALL #
###########

if [ "$1" == "--install" ] ; then
  if [ "$PLATFORM" == "Darwin" ]; then
    brew install qt5
  fi

  if [ "$PLATFORM" == "Linux" ]; then
    apt-get update && \
    apt-get install -y \
    build-essential \
    qt5-default \
    qtbase5-dev-tools \
    qtdeclarative5-dev \
    qtmultimedia5-dev \
    libqt5charts5 \
    libqt5charts5-dev \
    qml-module-qtcharts \
    libqt5multimedia5-plugins \
    qtquickcontrols2-5-dev \
    qml-module-qtquick-controls \
    qml-module-qtquick-controls2 \
    qtdeclarative5-dev-tools \
    xvfb
  fi
fi

##################
# BUILD AND TEST #
##################

if [ "$1" == "--build-n-test" ] ; then
  if [ "$PLATFORM" == "Darwin" ]; then
    if [ "$2" == "xvfb" ]; then
      echo "The xvfb option is not support on MacOS"
      exit 1
    fi
  fi

  create_clean_directory build
  qmake -unset QMAKEFEATURES
  cd build && qmake .. && make && cd -

  if [ "$PLATFORM" == "Darwin" ]; then
    ./build/ProjectVentilatorGUI.app/Contents/MacOS/ProjectVentilatorGUI -t
  fi

  if [ "$PLATFORM" == "Linux" ]; then
    if [ "$2" == "xvfb" ]; then
      Xvfb :1 &
      DISPLAY=:1 ./build/ProjectVentilatorGUI -t
    else
      ./build/ProjectVentilatorGUI -t
    fi
  fi
fi

##############
# BUILD ONLY #
##############

if [ "$1" == "--build" ] ; then
  create_clean_directory build
  qmake -unset QMAKEFEATURES
  cd build && qmake .. && make && cd -
fi

#############
# TEST ONLY #
#############

if [ "$1" == "--test" ] ; then
  if [ "$PLATFORM" == "Darwin" ]; then
    if [ "$2" == "xvfb" ]; then
      echo "The xvfb option is not support on MacOS"
      exit 1
    else
      ./build/ProjectVentilatorGUI.app/Contents/MacOS/ProjectVentilatorGUI -t
    fi
  fi

  if [ "$PLATFORM" == "Linux" ]; then
    if [ "$2" == "xvfb" ]; then
      Xvfb :1 &
      DISPLAY=:1 ./build/ProjectVentilatorGUI -t
    else
      ./build/ProjectVentilatorGUI -t
    fi
  fi
fi
