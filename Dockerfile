FROM raspbian/stretch:041518

ENV PROJECT_NAME rpidrone

WORKDIR /data

# Update image and get cmake
RUN apt-get update \
    && apt-get install -y \
        build-essential \
        cmake \
        git \
        wget \
        #for bluez
        bluez bluez-tools python-dev python-pip libglib2.0-dev libboost-python-dev libboost-thread-dev libbluetooth-dev \
        #wiringpi
        wiringpi

# copy files
COPY ./${PROJECT_NAME} ./${PROJECT_NAME}

#prepare build
RUN mkdir build && cd build && cmake ../${PROJECT_NAME} 

#build
RUN cd build && cmake --build .