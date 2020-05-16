ARG IMG_VERSION=20200516

FROM krenol/cpp_raspbian:${IMG_VERSION}

ARG PROJECT_DIR=rpidrone

WORKDIR /data

# copy files
COPY ./${PROJECT_DIR} ./${PROJECT_DIR}

#prepare build
RUN mkdir build && cd build && cmake ../${PROJECT_DIR} 

#build
RUN cd build && cmake --build .