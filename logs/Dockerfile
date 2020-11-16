FROM python:3.9.0-alpine3.12

WORKDIR /opt

COPY requirements.txt visualizer.py ./

RUN pip3 install -r requirements.txt

CMD ["python3", "visualizer.py"]