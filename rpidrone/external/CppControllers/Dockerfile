ARG IMG_VERSION=20200516

FROM krenol/cpp_raspbian:${IMG_VERSION}

ARG PROJECT_DIR=controllers

WORKDIR /data

# copy files
COPY ./CMakeLists.txt ./
COPY ./${PROJECT_DIR} ./${PROJECT_DIR}

#prepare build
RUN mkdir build && cd build && cmake ../

#build
RUN cd build && cmake --build .