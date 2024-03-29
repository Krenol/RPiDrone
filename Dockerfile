FROM krenol/cpp_raspbian:latest

ARG PROJECT_DIR=rpidrone
ENV BUILD_TEST=OFF

WORKDIR /data

# copy files
COPY CMakeLists.txt .
COPY ./${PROJECT_DIR} ./${PROJECT_DIR}

#prepare build
RUN mkdir build && cd build && cmake ../ 

#build
RUN cmake --build ./build

COPY ./docker-entrypoint.sh /
ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["start"]