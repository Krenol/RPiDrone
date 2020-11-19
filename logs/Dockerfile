FROM python:3.9.0-alpine3.12

RUN apk update
RUN apk add make automake gcc g++ subversion python3-dev

WORKDIR /opt

COPY requirements.txt ./

RUN pip3 install -r requirements.txt

COPY visualizer.py ./

CMD ["python3", "visualizer.py"]