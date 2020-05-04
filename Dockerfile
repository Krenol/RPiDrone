FROM raspbian/stretch:041518

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
COPY ./rpidrone ./rpidrone

#prepare build
RUN mkdir build && cd build && cmake ../rpidrone 

#build
RUN cd build && cmake --build .